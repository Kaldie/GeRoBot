// Copyright [2014] Ruud Cools

#include <Arduino.h>
#include <ByteBuffer.h>
#include <SdFat.h>

// defines
#define BAUD_RATE 115200
#define HAND_SHAKE_VALUE 200
#define ACTUATE_MODE_VALUE 100
#define ECHO_MODE_VERBOSE_VALUE 150
#define ECHO_MODE_VALUE 149
#define DELETE_FILE_MODE_VALUE 140
#define ERROR_MODE 244
#define CHIP_SELECT 10

// States for the machine
enum Status {
  SEND_HAND_SHAKE,
  VERIFY_HAND_SHAKE,
  MODE_SELECT,
  RECEIVING_MESSAGE_SIZE,
  RECEIVING_SPEED,
  RECEIVING_REPITITIONS,
  RECEIVING_STEPS,
  SEND_CRC,
  VERIFY_CRC,
  WRITE_IN_BUFFER,
  ACTUATE_PRE_MODE,
  ACTUATE_MODE,
  ACTUATE_POST_MODE,
  ECHO_MODE_VERBOSE,
  ECHO_MODE,
  DELETE_FILE_MODE
};


// data structure which represent the motor message
struct MotorMessage {
  int numberOfSteps;
  int currentStep;
  int speed;
  int numberOfRepetitions;
  int stepArray[10];
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
int SIZE_OF_INT = 4;

// Buffer needed to write the integers from the serial connection
unsigned char INTEGER_BUFFER[4];

// buffer for motor messages
ByteBuffer<MotorMessage> MOTOR_MESSAGE_BUFFER(25);

// return state of the handle functions
ReturnState RETURN_STATE;

// number of failed attempts to handle a certain state
int ATTEMPTS = 0;

// status of the state machine
Status ARDUINO_STATUS = SEND_HAND_SHAKE;

// Is the motor currently running?
bool IS_MOTOR_RUNNING = false;

// SD module
SdFat SD;

// File on the SD card on which the steps are written
SdFile stepFile;

// File name on the sd card
char STEP_FILE_NAME[] = "STEP.YZS";

// Number of MotorMessages currently read
unsigned long CURRENT_READ_MESSAGE_ON_SD = 0;
unsigned long CURRENT_WRITE_MESSAGE_ON_SD = 0;

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
  IS_MOTOR_RUNNING = true;
  sei();  // allow interrupts
}


void setPinOutputs() {
  // for SD card
  pinMode(CHIP_SELECT, OUTPUT);
  // Always hook up the 2 t/m 7 inputs!
  DDRD = DDRD | B11111100;
}


void setPins(byte i_byte) {
  // no1 touches the 1st and 2nd pin;
  i_byte &= B11111100;
  PORTD = i_byte;
}


ISR(TIMER1_COMPA_vect) {
  // get the read pointer
  MotorMessage* message = MOTOR_MESSAGE_BUFFER.getReadPointer();
  /// put the current step on the pins
  setPins((byte)(
      message->stepArray[message->currentStep]));
  // advance the current step for the next iteration
  message->currentStep++;
  // if this step exeeds the number if steps in this message
  if (message->currentStep >=
      message->numberOfSteps) {
    // reduce the number of reps and set current step to 0
    message->numberOfRepetitions--;
    message->currentStep = 0;
  }
  // if the number of reps is 0 or less
  if (message->numberOfRepetitions <= 0) {
    // disable the heart beat
    disableHeartBeat();
    // tell the buffer we're done with this message
    MOTOR_MESSAGE_BUFFER.finishReadPointer();
    // if the buffer is not empty
    if (!MOTOR_MESSAGE_BUFFER.isEmpty()) {
      // start with the next one
      setTimer1Interupt(MOTOR_MESSAGE_BUFFER.getReadPointer()->speed);
    } else {
      // else let it be known that we are not running anymore
      IS_MOTOR_RUNNING = false;
    }
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
       i++) {
    INTEGER_BUFFER[i] = Serial.read();
  }
  i_value = *(reinterpret_cast<int*>(INTEGER_BUFFER));
}


void sendHandShake(ReturnState* i_returnState) {
  Serial.write(HAND_SHAKE_VALUE);
  *i_returnState = SUCCES;
}


void handleModeSelect(ReturnState* i_returnState) {
  if (Serial.available() < 1) {
    *i_returnState = NO_RESPONSE;
    return;
  }

  // read the int size of the controller
  byte buffer[1];
  Serial.readBytes((char*)buffer, 1);

  if (*buffer < 10) {
    // It should be an int size
    SIZE_OF_INT = *buffer;

    // echo int size
    Serial.write(*buffer);

    // Set return status to succes
    *i_returnState = SUCCES;

    // Will be receiving message over serial now!
     ARDUINO_STATUS = RECEIVING_MESSAGE_SIZE;
  } else {
    // Something else is intended
    switch (*buffer) {
      case  ACTUATE_MODE_VALUE : {
        Serial.write(*buffer);
        *i_returnState = SUCCES;
        ARDUINO_STATUS = ACTUATE_PRE_MODE;
        break;
      }
      case DELETE_FILE_MODE_VALUE : {
        Serial.write(*buffer);
        *i_returnState = SUCCES;
        ARDUINO_STATUS = DELETE_FILE_MODE;
        break;
      }
      case ECHO_MODE_VERBOSE_VALUE : {
        Serial.write(*buffer);
        *i_returnState = SUCCES;
        ARDUINO_STATUS = ECHO_MODE_VERBOSE;
        break;
      }
      case ECHO_MODE_VALUE : {
        Serial.write(*buffer);
        *i_returnState = SUCCES;
        ARDUINO_STATUS = ECHO_MODE;
        break;
      }
      case ERROR_MODE : {
        *buffer = 0;
        Serial.write(*buffer);
        *i_returnState = FAIL;
        break;
      }
      default : {
        ARDUINO_STATUS = SEND_HAND_SHAKE;
        break;
      }
    }  // end switch case
  }  // end if statement
}

void sendSOSLed() {
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
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


bool writeMotorMessageBufferToSD() {

  /// Get the number of message which are in continues memory
  int numberOfMessageToRead = MOTOR_MESSAGE_BUFFER.inlineReadElements();
  /// If there are none, return false
  if (numberOfMessageToRead <= 0) {
    return false;
  }
  bool isAllreadyOpen = stepFile.isOpen();
  if (!isAllreadyOpen) {
    // if it not yet opened open it
    if (!stepFile.open(STEP_FILE_NAME, O_WRITE | O_CREAT)) {
      SD.errorHalt("opening file for write failed");
    }
    // and set the position correct so we don't overwrite important stuff
  }
  stepFile.seekSet(CURRENT_WRITE_MESSAGE_ON_SD * sizeof(MotorMessage));
  // Write it
  stepFile.write((void*)MOTOR_MESSAGE_BUFFER.getReadPointer(),
                 sizeof(MotorMessage) * numberOfMessageToRead);
  // notify we have 1 more message written on the SD card
  CURRENT_WRITE_MESSAGE_ON_SD += numberOfMessageToRead;
  MOTOR_MESSAGE_BUFFER.finishReadPointers(numberOfMessageToRead);
  // if it was not yet opened
  if (!isAllreadyOpen) {
    // close it
    stepFile.close();
  }
  return true;
}


bool readMotorMessagesFromSD() {
  bool isOpen = stepFile.isOpen();
  int numberOfMessageToRead = MOTOR_MESSAGE_BUFFER.inlineWriteElements();
  if ((CURRENT_WRITE_MESSAGE_ON_SD - CURRENT_READ_MESSAGE_ON_SD) <
      numberOfMessageToRead) {
    numberOfMessageToRead =
      CURRENT_WRITE_MESSAGE_ON_SD - CURRENT_READ_MESSAGE_ON_SD;
  }
  /// check if there are messages to read
  if (numberOfMessageToRead == 0) {
    return false;
  }
  /// Open the file
  if (!isOpen) {
    if (!stepFile.open(STEP_FILE_NAME, O_READ)) {
      SD.errorHalt("opening file for read failed");
    }
  }
  /// set the position to the "the read" message
  stepFile.seekSet(CURRENT_READ_MESSAGE_ON_SD * sizeof(MotorMessage));
  /// read the bytes and store them at the buffer
  stepFile.read((void*)MOTOR_MESSAGE_BUFFER.getWritePointer(),
                sizeof(MotorMessage) * numberOfMessageToRead);
  /// notify the buffer that messages are written and finished
  MOTOR_MESSAGE_BUFFER.finishWritePointers(numberOfMessageToRead);
  /// Notify that there are messages read from the SD
  CURRENT_READ_MESSAGE_ON_SD += numberOfMessageToRead;
  // if the file was previously not open close it now
  if (!isOpen) {
    // close the file
    stepFile.close();
  }
  /// return if something is read
  return true;
}


const int getNumberOfMessagesOnSD() {
  long totalNumberOfBytesRead;
  // Open the file if it is not yet opened
  if (!stepFile.isOpen()) {
    if (!stepFile.open(STEP_FILE_NAME, O_READ)) {
      SD.errorHalt("opening file for read failed");
   }
  }
  // get the size of the file
  totalNumberOfBytesRead = stepFile.fileSize();
  stepFile.close();
  // dived it to the size of a motor message
  return totalNumberOfBytesRead / sizeof(MotorMessage);
}


void printMessageToSerial(MotorMessage* i_messagePointer,
                          bool i_verbose = true) {
  if (!i_verbose) {
    byte numberOfBytesSend = 0;
    numberOfBytesSend += sizeof(i_messagePointer->speed);
    numberOfBytesSend += sizeof(i_messagePointer->numberOfRepetitions);
    numberOfBytesSend += sizeof(i_messagePointer->stepArray[0]) *
        i_messagePointer->numberOfSteps;

    Serial.write(numberOfBytesSend);
    Serial.write(reinterpret_cast<byte*>(&i_messagePointer->speed),
                 sizeof(i_messagePointer->speed));
    Serial.write(reinterpret_cast<byte*>(&i_messagePointer->numberOfRepetitions),
                 sizeof(i_messagePointer->numberOfRepetitions));
    for (int i = 0;
         i < i_messagePointer->numberOfSteps;
         i++) {
      Serial.write(reinterpret_cast<byte*>(&i_messagePointer->stepArray[i]),
                   sizeof(i_messagePointer->stepArray[i]));
    }
    return;
  } else {
    Serial.print("Number of steps: ");
    Serial.println(i_messagePointer->numberOfSteps);
    Serial.print("Speed: ");
    Serial.println(i_messagePointer->speed);
    Serial.print("Number of repititions: ");
    Serial.println(i_messagePointer->numberOfRepetitions);

    Serial.print("Steps: ");
    Serial.print(i_messagePointer->stepArray[0]);
    for (int i = 1;
         i < i_messagePointer->numberOfSteps;
         i++) {
      Serial.print(", ");
      Serial.print(i_messagePointer->stepArray[i]);

    }
    Serial.println("\n-----------------------");
    return;
  }
}

void handleStatus() {
  switch (ARDUINO_STATUS) {
  case SEND_HAND_SHAKE: {
    sendHandShake(&RETURN_STATE);
    ARDUINO_STATUS = VERIFY_HAND_SHAKE;
    break;
  }
  case VERIFY_HAND_SHAKE: {
    verifyResponse(HAND_SHAKE_VALUE, &RETURN_STATE);
    if (RETURN_STATE == SUCCES) {
      ARDUINO_STATUS = MODE_SELECT;
      ATTEMPTS = 0;
    } else if (RETURN_STATE == FAIL) {
      ARDUINO_STATUS = SEND_HAND_SHAKE;
    }

    if (ATTEMPTS >= 10000) {
      ATTEMPTS = 0;
      ARDUINO_STATUS = SEND_HAND_SHAKE;
    }
    ATTEMPTS++;
    break;
  }

  case MODE_SELECT: {
    handleModeSelect(&RETURN_STATE);
    if (RETURN_STATE == FAIL) {
      ARDUINO_STATUS = SEND_HAND_SHAKE;
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
      ARDUINO_STATUS = RECEIVING_SPEED;
    }
    break;
  }
  case RECEIVING_SPEED: {
    readIntegerFromSerial(MOTOR_MESSAGE_BUFFER.getWritePointer()->speed,
                          &RETURN_STATE);
    if (RETURN_STATE == SUCCES) {
      MOTOR_MESSAGE_BUFFER.getWritePointer()
        ->currentCRC += MOTOR_MESSAGE_BUFFER.getWritePointer()->speed;

      ARDUINO_STATUS = RECEIVING_REPITITIONS;
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

      ARDUINO_STATUS = RECEIVING_STEPS;
      MOTOR_MESSAGE_BUFFER.getWritePointer()->currentStep = 0;
    }
    break;
  }
  case RECEIVING_STEPS: {
    int step;
    MotorMessage* currentMessage = MOTOR_MESSAGE_BUFFER.getWritePointer();
    readIntegerFromSerial(step, &RETURN_STATE);
    if (RETURN_STATE == SUCCES) {
      currentMessage->stepArray
        [currentMessage->currentStep] = step;
      currentMessage->currentCRC +=
        currentMessage->stepArray[currentMessage->currentStep];
      currentMessage->currentStep++;
    }

    if (currentMessage->currentStep >= currentMessage->numberOfSteps) {
      ARDUINO_STATUS = SEND_CRC;
      currentMessage->currentStep = 0;
    }
    break;
  }

  case SEND_CRC: {
    Serial.write(MOTOR_MESSAGE_BUFFER.getWritePointer()->currentCRC);
    ARDUINO_STATUS = VERIFY_CRC;
    RETURN_STATE = SUCCES;
    break;
  }

  case VERIFY_CRC: {
    verifyResponse(MOTOR_MESSAGE_BUFFER.getWritePointer()->currentCRC,
                   &RETURN_STATE);
    if (RETURN_STATE == SUCCES) {
      ARDUINO_STATUS = WRITE_IN_BUFFER;
    } else if (RETURN_STATE == FAIL) {
      ARDUINO_STATUS = SEND_HAND_SHAKE;
    }
    break;
  }

  case WRITE_IN_BUFFER: {
    MOTOR_MESSAGE_BUFFER.finishWritePointer();
    if (MOTOR_MESSAGE_BUFFER.isFull()) {
      writeMotorMessageBufferToSD();
    }
    RETURN_STATE = SUCCES;
    ARDUINO_STATUS = SEND_HAND_SHAKE;
    break;
  }

  case DELETE_FILE_MODE : {
    SD.remove(STEP_FILE_NAME);
    ARDUINO_STATUS = SEND_HAND_SHAKE;
    break;
  }

  case ECHO_MODE_VERBOSE: {
    MotorMessage* currentMessage;
    Serial.println("Echoing messages on sd card!");
    while(writeMotorMessageBufferToSD()) {};
    // if no action has been undertaken,
    // do all!
    if (CURRENT_READ_MESSAGE_ON_SD == 0 and
        CURRENT_WRITE_MESSAGE_ON_SD == 0) {
      CURRENT_WRITE_MESSAGE_ON_SD = getNumberOfMessagesOnSD();
    }
    // clean the read buffer before starts
    while (MOTOR_MESSAGE_BUFFER.finishReadPointer()) {}
    // continue spamming until it breaks
    while (readMotorMessagesFromSD()) {
      while (!MOTOR_MESSAGE_BUFFER.isEmpty()) {
        currentMessage = MOTOR_MESSAGE_BUFFER.getReadPointer();
        printMessageToSerial(currentMessage,
                             true);
        MOTOR_MESSAGE_BUFFER.finishReadPointer();
      }
    }
    CURRENT_READ_MESSAGE_ON_SD = 0;
    ARDUINO_STATUS = SEND_HAND_SHAKE;
    break;
  }

  case ECHO_MODE: {
    // write the last message tot the buffer
    while(writeMotorMessageBufferToSD()) {};
    // if no action has been undertaken,
    // do all!
    if (CURRENT_READ_MESSAGE_ON_SD == 0 and
        CURRENT_WRITE_MESSAGE_ON_SD == 0) {
      CURRENT_WRITE_MESSAGE_ON_SD = getNumberOfMessagesOnSD();
    }
    // continue spamming until it breaks
    int i = 0;
    bool xx = true;
    while (readMotorMessagesFromSD() || xx) {
      xx = false;
      while (!MOTOR_MESSAGE_BUFFER.isEmpty() && i < 8) {
        xx = true;
        printMessageToSerial(MOTOR_MESSAGE_BUFFER.getReadPointer(),
                             false);
        MOTOR_MESSAGE_BUFFER.finishReadPointer();
        ++i;
      }
      i = 0;
    }
    CURRENT_READ_MESSAGE_ON_SD = 0;
    CURRENT_WRITE_MESSAGE_ON_SD = 0;
    ARDUINO_STATUS = SEND_HAND_SHAKE;
    break;
  }

  case ACTUATE_PRE_MODE : {
    // cleaning the buffer and storing the last message on sd-card
    while(writeMotorMessageBufferToSD()) {};
    // if no read or write actions have been undertaking during this session:
    // do all on the file!
    if ((CURRENT_READ_MESSAGE_ON_SD == 0) and
        (CURRENT_WRITE_MESSAGE_ON_SD == 0)) {
      CURRENT_WRITE_MESSAGE_ON_SD = getNumberOfMessagesOnSD();
    }
    // open the step file
      if (!stepFile.open(STEP_FILE_NAME, O_READ)) {
      SD.errorHalt("opening file for read failed");
    }
    // Tell the arduino we are actuating
    ARDUINO_STATUS = ACTUATE_MODE;
    break;
  }

  case ACTUATE_MODE : {
    if (MOTOR_MESSAGE_BUFFER.emptyElements() >= 15) {
      bool hasFoundMessageOnSD = readMotorMessagesFromSD();
      /*
        if there are now new message read from SD
        and the motor is not running
        The final and last step will activate in the next if statement
        So we can start sending hand shakes again
      */
      if (!hasFoundMessageOnSD and
          MOTOR_MESSAGE_BUFFER.isEmpty() and
          !IS_MOTOR_RUNNING ) {
        ARDUINO_STATUS = ACTUATE_POST_MODE;
      }
    }
    /* DEBUG STATEMENTS TO FIGURE OUT IF SOMETHING IS WRONG HERE!
      while (!MOTOR_MESSAGE_BUFFER.isEmpty()) {
        printMessageToSerial(MOTOR_MESSAGE_BUFFER.getReadPointer(),
                             false);
        MOTOR_MESSAGE_BUFFER.finishReadPointer();
       }
    */

    if (!IS_MOTOR_RUNNING and !MOTOR_MESSAGE_BUFFER.isEmpty()) {
      setTimer1Interupt(MOTOR_MESSAGE_BUFFER.getReadPointer()->speed);
    }
    break;
  }
  case ACTUATE_POST_MODE : {
    // "Reset" the SD card
    CURRENT_WRITE_MESSAGE_ON_SD = 0;
    CURRENT_READ_MESSAGE_ON_SD = 0;
    // the stepFile is still opened, close it
    stepFile.close();
    // Reset the state machine
    ARDUINO_STATUS = SEND_HAND_SHAKE;
    break;
  }
  default: {
    break;
  }
  }  // end switch
}


void setup() {
  // initialize the digital pin as an output.
  setPinOutputs();
  // Open the serial connections
  Serial.setTimeout(10);
  Serial.begin(BAUD_RATE);
  // see if the card is present and can be initialized:
  if (!SD.begin(CHIP_SELECT, SPI_FULL_SPEED)) {
    SD.initErrorHalt("Could not open the SD card!");
  }
}


void loop() {
  // start with a hand shake if that didnt happen yet
  handleStatus();
}

