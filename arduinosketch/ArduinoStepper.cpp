// Copyright [2014] Ruud Cools

#include <string.h>
#include <stdarg.h>
#include <Arduino.h>
#include <ByteBuffer.h>
// defines
#define BAUD_RATE 115200
#define HAND_SHAKE 200

// States for the machine
enum Status {
  SEND_HAND_SHAKE,
  VERIFY_HAND_SHAKE,
  HANDLE_INT_SIZE,
  RECEIVING_MESSAGE_SIZE,
  RECEIVING_SPEED,
  RECEIVING_REPITITIONS,
  RECEIVING_STEPS,
  SEND_CRC,
  VERIFY_CRC,
  WRITE_IN_BUFFER,
};


// data structure which represent the motor message
struct MotorMessage {
  int numberOfSteps;
  int currentStep;
  int speed;
  int numberOfRepetitions;
  int stepArray[30];
  unsigned char currentCRC;
};


// State which represent the return of the handle functions
enum ReturnState {
  NO_RESPONSE,
  FAIL,
  SUCCES
};

// Global variables
// Size of an received in bytes
int SIZE_OF_INT = 0;

// Buffer needed to write the integers from the serial connection
unsigned char INTEGER_BUFFER[10];

// buffer for motor messages
ByteBuffer<MotorMessage> MOTOR_MESSAGE_BUFFER(20);

// return state of the handle functions
ReturnState RETURN_STATE;

// number of failed attempts to handle a certain state
int ATTEMPTS = 0;

// status of the state machine
Status status = SEND_HAND_SHAKE;

// Is the motor currently running?
bool IS_MOTOR_RUNNING = false;

void sendSOSLed();
void getMessageFromBuffer();


void disableHeartBeat() {
  cli();  // stop interrupts
  // set timer0 interrupt at 2kHz
  TCCR1A = 0;  // set entire TCCR0A  to 0
  TCCR1B = 0;  // same for TCCR0B
  TCNT1  = 0;  // initialize counter value to 0
  OCR1A  = 0;
  TIMSK1 = 0;
  sei();  // allow interrupts
}


void setTimer1Interupt(int i_dekaHertz) {
  // i_dekaHertz is 10* input hertz
  // so we can send 100k hertz with arduino 2 byte int
  cli();  // stop interrupts

  // set timer1 interrupt at 1Hz
  TCCR1A = 0;  // set entire TCCR1A  to 0
  TCCR1B = 0;  // same for TCCR1B
  TCNT1  = 0;  // initialize counter value to 0

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);

  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  long registerValue;
  // prescale factor, to match arduino internal
  int prescaler[] = {0, 1, 8, 64, 256, 1024};
  int i = 0;
  do {
    i++;
    registerValue = (1600000L / (prescaler[i] * i_dekaHertz)) -1L;
  }
  while (registerValue > 65536 and i <= 5);

  if (registerValue > 65536)
    registerValue = 65536;

  OCR1A = registerValue;

  if (B00000001 & i)
    TCCR1B |= 1 << CS10;
  if (B00000010 & i)
    TCCR1B |= 1 << CS11;
  if (B00000100 & i)
    TCCR1B |= 1 << CS12;
  sei();  // allow interrupts
}


void setPinOutputs() {
  // Always hook up the 2 t/m 7 inputs!
  pinMode(13, OUTPUT);
  DDRD = DDRD | B11111100;
}


void setPins(byte i_byte) {
  // no1 touches the 1st and 2nd pin;
  i_byte &= B11111100;
  PORTD = i_byte;
}


ISR(TIMER1_COMPA_vect) {
  MotorMessage* message = MOTOR_MESSAGE_BUFFER.getReadPointer();
  setPins((byte)(
      message->stepArray[message->currentStep]));
  /*
    Serial.write(STEP_ARRAY[CURRENT_STEP] + '0');
    Serial.write(CURRENT_STEP + '0');
    Serial.write(NUMBER_OF_REPETITIONS + '0');
    Serial.write(NUMBER_OF_STEPS + '0');
  */
  message->currentStep++;

  if (message->currentStep >=
      message->numberOfSteps) {
    message->numberOfRepetitions--;
    message->currentStep = 0;
  }

  if (message->numberOfRepetitions <= 0) {
    disableHeartBeat();
    IS_MOTOR_RUNNING = false;
  }
}


void readIntegerFromSerial(int& i_value,
                           ReturnState* i_returnState) {
  if (Serial.available() < SIZE_OF_INT) {
    *i_returnState = NO_RESPONSE;
    return;
  } else {
    *i_returnState = SUCCES;
  }
  
  for (int i = 0;
       i < SIZE_OF_INT;
       i++)
    INTEGER_BUFFER[i] = Serial.read();
  i_value = *(reinterpret_cast<int*>(INTEGER_BUFFER));
}


void blinkKnightRider() {
  if (!MOTOR_MESSAGE_BUFFER.isFull()) {
    MotorMessage* message = MOTOR_MESSAGE_BUFFER.getWritePointer();
    message->numberOfSteps = 12;
    message->numberOfRepetitions = 2;
    message->currentStep = 0;
    message->speed = 1;
    for (int i = 2;
         i < 8;
         i++) {
      message->stepArray[i-2] = 1 << i;
    }

    for (int i = 7;
         i > 1;
         i--) {
      message->stepArray[6-(i-7)] = 1 << i;
    }


    MOTOR_MESSAGE_BUFFER.finishWritePointer();
    IS_MOTOR_RUNNING = true;
    getMessageFromBuffer();
  } else {
    MOTOR_MESSAGE_BUFFER.finishReadPointer();
  }
}


bool sendHandShake(ReturnState* i_returnState) {
  Serial.write(HAND_SHAKE);
  *i_returnState = SUCCES;
}


void handleIntSize(ReturnState* i_returnState) {
  if (Serial.available() < 1) {
    *i_returnState = NO_RESPONSE;
    return;
  }

  // read the int size of the controller
  byte buffer[1];
  int intSize = 0;
  Serial.readBytes((char*)buffer, 1);
  intSize = *buffer;

  if (intSize < 10) {
    // echo it if its ok
    Serial.write(intSize);
    *i_returnState = SUCCES;
    SIZE_OF_INT = intSize;
  } else {
    *i_returnState = FAIL;
  }
}


void sendSOSLed() {
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
}


void verifyResponse(const unsigned char& i_requiredResponse,
                    ReturnState* i_returnState) {
  unsigned char input;
  if (Serial.available() == 0) {
    *i_returnState = NO_RESPONSE;
    return;
  } else {
    Serial.readBytes(reinterpret_cast<char*>(&input),
                     1);
  }

  if (input == i_requiredResponse) {
    *i_returnState = SUCCES;
  } else {
    // input is not equal to required response
    *i_returnState = FAIL;
  }
}


void handleStatus() {
  switch (status) {
    case SEND_HAND_SHAKE: {
      sendHandShake(&RETURN_STATE);
      status = VERIFY_HAND_SHAKE;
      break;
    }

    case VERIFY_HAND_SHAKE: {
      verifyResponse(HAND_SHAKE, &RETURN_STATE);
      if (RETURN_STATE == SUCCES) {
        status = HANDLE_INT_SIZE;
        ATTEMPTS = 0;
      } else if (RETURN_STATE == FAIL) {
        status = SEND_HAND_SHAKE;
      }
      
      if (ATTEMPTS >= 10000) {
        ATTEMPTS = 0;
        status = SEND_HAND_SHAKE;
      }
      ATTEMPTS++;
      break;
    }

    case HANDLE_INT_SIZE: {
      handleIntSize(&RETURN_STATE);
      if (RETURN_STATE == SUCCES) {
        status = RECEIVING_MESSAGE_SIZE;
      } else if (RETURN_STATE == FAIL) {
        status = SEND_HAND_SHAKE;
      }

      break;
    }

    case RECEIVING_MESSAGE_SIZE: {
      int messageSize;
      readIntegerFromSerial(messageSize, &RETURN_STATE);
      if (RETURN_STATE == SUCCES) {
        MOTOR_MESSAGE_BUFFER.getWritePointer()->numberOfSteps =
            (messageSize / SIZE_OF_INT) - 2;
        MOTOR_MESSAGE_BUFFER.getWritePointer()->currentCRC = messageSize;
        status = RECEIVING_SPEED;
      }
      break;
    }

    case RECEIVING_SPEED: {
      readIntegerFromSerial(MOTOR_MESSAGE_BUFFER.getWritePointer()->speed,
                            &RETURN_STATE);
      if (RETURN_STATE == SUCCES) {
        MOTOR_MESSAGE_BUFFER.getWritePointer()
            ->currentCRC += MOTOR_MESSAGE_BUFFER.getWritePointer()->speed;

        status = RECEIVING_REPITITIONS;
      }
      break;
    }

    case RECEIVING_REPITITIONS: {
      readIntegerFromSerial(
          MOTOR_MESSAGE_BUFFER.getWritePointer()->numberOfRepetitions,
          &RETURN_STATE);
      if (RETURN_STATE == SUCCES) {
        MOTOR_MESSAGE_BUFFER.getWritePointer()->currentCRC +=
            MOTOR_MESSAGE_BUFFER.getWritePointer()->numberOfRepetitions;

        status = RECEIVING_STEPS;
        MOTOR_MESSAGE_BUFFER.getWritePointer()->currentStep = 0;
      }
      break;
    }

    case RECEIVING_STEPS: {
      int step;
      readIntegerFromSerial(step, &RETURN_STATE);
      if (RETURN_STATE == SUCCES) {
        MOTOR_MESSAGE_BUFFER.getWritePointer()->stepArray[
            MOTOR_MESSAGE_BUFFER.getWritePointer()->currentStep] = step;

        MOTOR_MESSAGE_BUFFER.getWritePointer()->currentCRC +=
            MOTOR_MESSAGE_BUFFER.getWritePointer()->stepArray[
                MOTOR_MESSAGE_BUFFER.getWritePointer()->currentStep];

        MOTOR_MESSAGE_BUFFER.getWritePointer()->currentStep++;
      }

      if (MOTOR_MESSAGE_BUFFER.getWritePointer()->currentStep >=
          MOTOR_MESSAGE_BUFFER.getWritePointer()->numberOfSteps) {
        status = SEND_CRC;
        MOTOR_MESSAGE_BUFFER.getWritePointer()->currentStep = 0;
      }
      break;
    }

    case SEND_CRC: {
      Serial.write(MOTOR_MESSAGE_BUFFER.getWritePointer()->currentCRC);
      status = VERIFY_CRC;
      RETURN_STATE = SUCCES;
      break;
    }

    case VERIFY_CRC: {
      verifyResponse(MOTOR_MESSAGE_BUFFER.getWritePointer()->currentCRC,
                     &RETURN_STATE);
      if (RETURN_STATE == SUCCES) {
        status = WRITE_IN_BUFFER;
      } else if (RETURN_STATE == FAIL) {
        status = SEND_HAND_SHAKE;
      }
      //      MOTOR_MESSAGE_BUFFER.getWritePointer()->currentCRC = 0;
      break;
    }

    case WRITE_IN_BUFFER: {
      if (!MOTOR_MESSAGE_BUFFER.isFull()) {
        MOTOR_MESSAGE_BUFFER.finishWritePointer();
        RETURN_STATE = SUCCES;
        status = SEND_HAND_SHAKE;
      } else {
        //        MOTOR_MESSAGE_BUFFER.getReadPointer();
        //        MOTOR_MESSAGE_BUFFER.finishReadPointer();
        RETURN_STATE = FAIL;
      }
      break;
    }

    default: {
      // sendSOSLed();
    }
  }  // end switch
}


void getMessageFromBuffer() {
  if (!MOTOR_MESSAGE_BUFFER.isEmpty()) {
    // go to the next message
    MOTOR_MESSAGE_BUFFER.finishReadPointer();
    setTimer1Interupt(MOTOR_MESSAGE_BUFFER.getReadPointer()->speed);
    IS_MOTOR_RUNNING = true;
  }
}


void setup() {
  // initialize the digital pin as an output.
  setPinOutputs();
  // Open the serial connections
  Serial.setTimeout(10);
  Serial.begin(BAUD_RATE);
}


void loop() {
  // Make it run!
  if (!IS_MOTOR_RUNNING) {
    //    blinkKnightRider();
    getMessageFromBuffer();
  }

  // start with a hand shake if that didnt happen yet
  handleStatus();
}

