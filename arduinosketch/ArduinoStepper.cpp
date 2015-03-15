// Copyright [2014] Ruud Cools

#include <string.h>
#include <stdarg.h>
#include <Arduino.h>

// defines
#define BAUD_RATE 115200
#define HAND_SHAKE 200

// Global variables definitions
bool HAS_SHAKEN = false;
int SIZE_OF_INT = 0;

// Global buffer to store integer values received from serial connection
byte INTEGER_BUFFER[10];

// Global step variables
int STEP_ARRAY[100];

// number of repetitions which need to be done
int NUMBER_OF_REPETITIONS = 0;
// number of steps which are defined in the step array
int NUMBER_OF_STEPS = 0;
int CURRENT_STEP = 0;

// set up functions
extern void setPins(byte i_pinValue);
extern bool handShake();
extern void sendSOSLed();
extern int readIntegerFromSerial(bool&);

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
  sei();  // allow interrupts
}


void setTimer1Interupt(int i_dekaHertz) {
  // i_dekaHertz is 10* input hertz
  // so we can send 100k hertz with arduino 2 byte int
  cli();  // stop interrupts
  digitalWrite(13, HIGH);

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
  setPins((byte)(STEP_ARRAY[CURRENT_STEP]));
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

  if (NUMBER_OF_REPETITIONS <= 0) {
    disableHeartBeat();
    HAS_SHAKEN = false;
  }
}


void setPins(byte i_byte) {
    // no1 touches the 1st and 2nd pin;
    i_byte &= B11111100;
    PORTD = i_byte;
}


int readIntegerFromSerial(bool& o_hasSucces) {
  int attempts = 0;
  while (Serial.available() < SIZE_OF_INT & attempts < 100) {
    attempts++;
  }

  if (Serial.available() < SIZE_OF_INT) {
    o_hasSucces &= false;
    return 0;
  } else {
    o_hasSucces &= true;
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

  setTimer1Interupt(5);
}


bool handShake() {
  // Hand shake with the controller
  int intSize = 0;
  bool received = false;
  bool hasIntSize = false;
  byte buffer[1];
  int attempts = 0;

  Serial.write(HAND_SHAKE);
  while (!received & attempts < 100) {
    if (Serial.available() > 0) {
      // receive echo
      Serial.readBytes((char*)buffer, 1);
      if (*buffer == HAND_SHAKE) {
        received = true;
      };
    } else {
      attempts++;
    }
  }

  if (!received) {
    return false;
  }

  attempts = 0;
  while( !hasIntSize & attempts < 100) {
    if (Serial.available() > 0) {
      // read the int size of the controller
      Serial.readBytes((char*)buffer, 1);
      intSize = *buffer;

      if (intSize < 10) {
        // echo it if its ok
        Serial.write(intSize);
      }

      SIZE_OF_INT = intSize;
      hasIntSize = true;
    } else {
      attempts++;
    }
  }

  if (!hasIntSize) {
    Serial.write(3);
  }

  if (hasIntSize & received)
    return true;
  else
    return false;
}


void sendSOSLed() {
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
}


bool handleMotorMessage() {
  // add all values received from serial
  // to each other and send them back as a check
  unsigned char crcNumber = 0;

  // readIntegerFromSerial set the input
  // variable to false once it has failed
  bool hasSucces(true);
  
  // number of bytes in the current message
  int numberOfBytesInMessage = readIntegerFromSerial(hasSucces);

  // number of steps per repetitions;
  NUMBER_OF_STEPS = (numberOfBytesInMessage / SIZE_OF_INT) - 2;

  CURRENT_STEP = 0;
  int speed = readIntegerFromSerial(hasSucces);

  NUMBER_OF_REPETITIONS = readIntegerFromSerial(hasSucces);

  // calculate the crc number
  crcNumber += numberOfBytesInMessage;
  crcNumber += speed;
  crcNumber += NUMBER_OF_REPETITIONS;

  for (int i = 0;
       i < NUMBER_OF_STEPS;
       i++) {
    STEP_ARRAY[i] = readIntegerFromSerial(hasSucces);
    crcNumber += STEP_ARRAY[i];
  }

  Serial.write(crcNumber);

  if (hasSucces) {
    setTimer1Interupt(speed);
  } else {
    NUMBER_OF_REPETITIONS = 0;
    HAS_SHAKEN = false;
  }
}


void setup() {
  // initialize the digital pin as an output.
  setPinOutputs();
  // Open the serial connections
  Serial.begin(BAUD_RATE);
}


void loop() {
  // start with a hand shake if that didnt happen yet
  while (!HAS_SHAKEN){
    HAS_SHAKEN = handShake();
  }

  if (HAS_SHAKEN & NUMBER_OF_REPETITIONS <= 0)
    handleMotorMessage();
}
        
