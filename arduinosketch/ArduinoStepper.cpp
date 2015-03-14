// Copyright [2014] Ruud Cools

#include <string.h>
#include <stdarg.h>
#include <Arduino.h>

// defines
#define BAUD_RATE 115200
#define HAND_SHAKE 200
#define NDEBUG

// debug message
#ifdef NDEBUG
#define DEBUG_MSG_RCS(message) {}
#else
#define DEBUG_MSG_RCS(message) Serial.println(message)
#endif


// Global variables definitions
bool HAS_SHAKEN = false;
int SIZE_OF_INT = 0;
byte* INTEGER_BUFFER = NULL;

// Global step variables
// dynamic allocated memory to store the step array
int* STEP_ARRAY = NULL;
// number of repetitions which need to be done
int NUMBER_OF_REPETITIONS = 0;
// number of steps which are defined in the step array
int NUMBER_OF_STEPS = 0;
int CURRENT_STEP = 0;

// set up functions
extern void setPins(byte i_pinValue);
extern bool handShake();
extern void sendSOSLed();
extern int readIntegerFromSerial();

// example buffer filling
extern void blinkKnightRider();

void disableHeartBeat() {
  cli();  // stop interrupts
  // set timer0 interrupt at 2kHz
  TCCR1A = 0;  // set entire TCCR0A  to 0
  TCCR1B = 0;  // same for TCCR0B
  TCNT1  = 0;  // initialize counter value to 0
  OCR1A  = 0;
  TIMSK1 = 0;
  digitalWrite(13, LOW);
  sei();  // allow interrupts
  //  Serial.flush();
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

  //  TCCR1B |= (1 << CS12) | (1 << CS10);
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


ISR(TIMER1_COMPA_vect) {
  //  Serial.print("Current step: "); Serial.println(CURRENT_STEP,DEC);
  //Serial.print("Value: "); Serial.println(STEP_ARRAY[CURRENT_STEP]);
  setPins((byte)STEP_ARRAY[CURRENT_STEP]);
  /*
    Serial.write(STEP_ARRAY[CURRENT_STEP] + '0');
    Serial.write(CURRENT_STEP + '0');
    Serial.write(NUMBER_OF_REPETITIONS + '0');
    Serial.write(NUMBER_OF_STEPS + '0');
  */
  CURRENT_STEP++;

  if (CURRENT_STEP >= NUMBER_OF_STEPS) {
    NUMBER_OF_REPETITIONS--;
    CURRENT_STEP = 0;
  }

  if (NUMBER_OF_REPETITIONS == 0) {
    disableHeartBeat();
    Serial.println("Finished movement!");
  }
}


void setPins(byte i_byte) {
    // no1 touches the 1st and 2nd pin;
    i_byte &= B11111100;
    PORTD = i_byte;
}


int readIntegerFromSerial() {
  if (Serial.available() < SIZE_OF_INT) {
    return readIntegerFromSerial();
  }

  for (int i = 0 ;
       i < SIZE_OF_INT;
       i++)
    INTEGER_BUFFER[i] = Serial.read();

  int value;
  value = *(reinterpret_cast<int*>(INTEGER_BUFFER));
  return value;
}


void blinkKnightRider() {
  if (STEP_ARRAY != NULL)
    free(STEP_ARRAY);
  
  STEP_ARRAY = reinterpret_cast<int*>(
      malloc(sizeof(*STEP_ARRAY) * 12));
  NUMBER_OF_STEPS = 12;
  NUMBER_OF_REPETITIONS = 5;
  CURRENT_STEP = 0;
  for (int i = 2;
       i < 8;
       i++) {
    STEP_ARRAY[i-2] = 1 << i;
  }

  for (int i = 7;
       i > 1;
       i--) {
    STEP_ARRAY[6-(i-7)] = 1 << i;
  }
  setTimer1Interupt(1);
}


bool handShake() {
  // Hand shake with the controller
  // free possible allocated memory
  if (INTEGER_BUFFER != NULL) {
    free(INTEGER_BUFFER);
    INTEGER_BUFFER = NULL;
  }

  if (STEP_ARRAY != NULL) {
    free(STEP_ARRAY);
    STEP_ARRAY = NULL;
  }

  int intSize = 0;
  bool received = false;
  bool hasIntSize = false;
  byte buffer[1];
  int attempts = 0;

  Serial.write(HAND_SHAKE);
  while (!received & attempts < 10) {
    if (Serial.available() > 0) {
      // receive echo
      Serial.readBytes((char*)buffer, 1);
      if (*buffer == HAND_SHAKE) {
        received = true;
      };
    } else {
        delay(200);
        attempts++;
    }
  }

  if (!received){
    sendSOSLed();
    return false;
  }

  while (!hasIntSize) {
    if (Serial.available() > 0) {
      // read the int size of the controller
      Serial.readBytes((char*)buffer, 1);
      intSize = *buffer;

      if (intSize < 10) {
        // echo it if its ok
        Serial.write(intSize);
      } else {
        sendSOSLed();
      }

      INTEGER_BUFFER = reinterpret_cast<byte*>(malloc(intSize));
      SIZE_OF_INT = intSize;
      *INTEGER_BUFFER = intSize;
      hasIntSize = true;
    }
  }
  Serial.flush();
  return true;
}

void sendSOSLed() {
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
}


void handleMotorMessage() {
  if (STEP_ARRAY != NULL) {
    free(STEP_ARRAY);
    STEP_ARRAY = NULL;
  }

  // need at least 4 integer being
    // send number of bytes
    // Speed
    // number of repetitions
    // and at least 1 step

  // number of bytes in the current message
  int numberOfBytesInMessage = readIntegerFromSerial();
  // number of steps per repetitions;
  NUMBER_OF_STEPS = (numberOfBytesInMessage / SIZE_OF_INT) - 2;
  CURRENT_STEP = 0;

  int speed = readIntegerFromSerial();
  NUMBER_OF_REPETITIONS = readIntegerFromSerial();
  /*
    Serial.print("Speed: "); Serial.println(speed,DEC);
    Serial.print("Number of steps: "); Serial.println(NUMBER_OF_STEPS, DEC);
    Serial.print("Number of repetitions: "); Serial.println(NUMBER_OF_REPETITIONS,DEC);
  */
  
  STEP_ARRAY =  reinterpret_cast<int *>(
      malloc(sizeof(speed) * NUMBER_OF_STEPS));

  //  Serial.print("Step array: ");
  for (int i = 0;
       i < NUMBER_OF_STEPS;
       i++) {
    STEP_ARRAY[i] = readIntegerFromSerial();
    // Serial.print(STEP_ARRAY[i]); Serial.print(" ");
  }
  //  Serial.println(".");
  
  if (Serial.available() != 0)
    sendSOSLed();

  //blinkKnightRider();
  setTimer1Interupt(speed);
}


void setup() {
  // initialize the digital pin as an output.
  setPinOutputs();
  // Open the serial connections
  Serial.begin(BAUD_RATE);
  DEBUG_MSG_RCS("done setting");
}


void loop() {
  // start with a hand shake if that didnt happen yet
  while (!HAS_SHAKEN)
    HAS_SHAKEN = handShake();

  if (Serial.available() > 0) {
    //    Serial.print(Serial.available(),DEC);
    handleMotorMessage();
  } else {

    if (NUMBER_OF_REPETITIONS == 0)
      blinkKnightRider();
    else
    {}
      //      Serial.print("Number of reps: "); Serial.println(NUMBER_OF_REPETITIONS,DEC);
  }
}
