// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include "./ArduinoMotorDriver.h"

const int ArduinoMotorDriver::HAND_SHAKE_VALUE = 200;
const int ArduinoMotorDriver::ACTUATE_MODE_VALUE = 100;
const int ArduinoMotorDriver::ECHO_MODE_VERBOSE_VALUE = 150;
const int ArduinoMotorDriver::ECHO_MODE_VALUE = 149;
const int ArduinoMotorDriver::DELETE_FILE_MODE_VALUE = 140;
const int ArduinoMotorDriver::UPLOAD_MODE_VALUE =
    sizeof(ArduinoMotorDriver::HAND_SHAKE_VALUE);


ArduinoMotorDriver::ArduinoMotorDriver()
    : m_serialRegularExpresion("lalal") {
}


ArduinoMotorDriver::ArduinoMotorDriver(std::string i_serialRegularExpression)
    : m_serialRegularExpresion(i_serialRegularExpression) {
}


void ArduinoMotorDriver::initialiseArduinoConnection() {
  LOG_DEBUG("Initialising Arduino Connection");
  LOG_INFO("Set regular expression" << m_serialRegularExpresion);
  m_arduinoConnection.setPortName(m_serialRegularExpresion);
  m_arduinoConnection.setBaudRate(115200);
  m_arduinoConnection.setBlockThread(true);
  m_arduinoConnection.setMinimumBytePerMessage(1);
  m_arduinoConnection.setCloseHandleAfterMessage(false);
  if (!m_arduinoConnection.hasConnection()) {
      m_arduinoConnection.openConnection();
  } else {
    m_arduinoConnection.resetConnection();
  }
  m_arduinoConnection.flushConnection();
}


bool ArduinoMotorDriver::handShake(ArduinoMotorDriver::DriverStatus i_status) {
  if (!m_arduinoConnection.hasConnection())
    initialiseArduinoConnection();

  int receivedHandShake, receivedModeValue = 10000;
  int attempts, modeValue = 0;

  receivedHandShake = m_arduinoConnection.serialRead(1);
  LOG_DEBUG("Hand shake value: " << ArduinoMotorDriver::HAND_SHAKE_VALUE <<
            ", received value: " << receivedHandShake);

  if (ArduinoMotorDriver::HAND_SHAKE_VALUE != receivedHandShake) {
    return false;
  }

  m_arduinoConnection.serialWrite(ArduinoMotorDriver::HAND_SHAKE_VALUE);

  switch (i_status) {
    case ArduinoMotorDriver::UPLOAD : {
      modeValue = ArduinoMotorDriver::UPLOAD_MODE_VALUE;
      break;
    }
    case ArduinoMotorDriver::ACTUATE : {
      modeValue = ArduinoMotorDriver::ACTUATE_MODE_VALUE;
      break;
    }
    case ArduinoMotorDriver::SERIAL_ECHO : {
      modeValue = ArduinoMotorDriver::ECHO_MODE_VALUE;
      break;
    }
    case ArduinoMotorDriver::SERIAL_ECHO_VERBOSE : {
      modeValue = ArduinoMotorDriver::ECHO_MODE_VERBOSE_VALUE;
      break;
    }
    case ArduinoMotorDriver::DELETE_FILE : {
      modeValue = ArduinoMotorDriver::DELETE_FILE_MODE_VALUE;
      break;
    }
    case ArduinoMotorDriver::ERROR : {
      LOG_ERROR("An error has occured. Reset the driver!");
      break;
    }
    default : {
      LOG_ERROR("Mode is not set properly!");
      break;
    }
  }

  m_arduinoConnection.serialWrite(modeValue);
  receivedModeValue = m_arduinoConnection.serialRead(1);
  while (modeValue != receivedModeValue && attempts < 5) {
    receivedModeValue = m_arduinoConnection.serialRead(1);
    attempts++;
  }

  LOG_DEBUG("Selected mode value: " << modeValue <<
            " ,received mode value: " << receivedModeValue);

  if (receivedModeValue != modeValue) {
    return false;
  }
  return true;
}


void ArduinoMotorDriver::upload(const std::vector<int> i_messageVector) {
  if (!m_arduinoConnection.hasConnection()) {
    initialiseArduinoConnection();
  }

  unsigned char calculatedCRC(0);
  unsigned char receivedCRC;

  if (i_messageVector.size() * sizeof(*i_messageVector.data()) > 255)
    LOG_ERROR("Message is bigger then 255 chars," <<
              " which cannot be send easely of the serial");

  // Shake it like a polaroid picture, at least untill it works
  while (!handShake(ArduinoMotorDriver::UPLOAD)) {}

  for (auto itr = i_messageVector.begin();
       itr != i_messageVector.end();
       itr++) {
    m_arduinoConnection.serialWrite(
        reinterpret_cast<const unsigned char*>(&(*itr)),
        sizeof(*itr));

    calculatedCRC += *itr;
  }

  receivedCRC = m_arduinoConnection.serialRead(1);
  m_arduinoConnection.serialWrite(calculatedCRC);
  LOG_INFO("read: " << static_cast<int>(receivedCRC)
           << ", Calc: " << static_cast<int>(calculatedCRC));

  if (receivedCRC != calculatedCRC)
    LOG_ERROR("fail!");
}


void ArduinoMotorDriver::actuate() {
    if (!m_arduinoConnection.hasConnection()) {
      initialiseArduinoConnection();
    }
    while (!handShake(ArduinoMotorDriver::ACTUATE)) {}
}

bool ArduinoMotorDriver::sendTestBit() {
  while (!handShake(ArduinoMotorDriver::UPLOAD)) {
    LOG_DEBUG("Hand shake went bad!");
    std::cout << 'x';
  }

  unsigned char* position;
  int testInt [] = {16,10000,100,0,4,0,8};
  unsigned char crc = 0;
  for (int i = 0;
       i < 5;
       i++) {
    position = reinterpret_cast<unsigned char*>(testInt+i);
    m_arduinoConnection.serialWrite(position, sizeof(testInt[0]));
    crc += testInt[i];
  }

  unsigned char crcBit = m_arduinoConnection.serialRead(1);
  m_arduinoConnection.serialWrite(crc);
  LOG_INFO("read: " << static_cast<int>(crcBit)
           << "Calc: " << static_cast<int>(crc));

  if (crcBit != crc) {
    LOG_DEBUG("fail!");
    return sendTestBit();
  }
  return true;
}


void ArduinoMotorDriver::deleteFile() {
     if (!m_arduinoConnection.hasConnection()) {
      initialiseArduinoConnection();
    }
    while (!handShake(ArduinoMotorDriver::DELETE_FILE)) {}
}

void ArduinoMotorDriver::echo() {
     if (!m_arduinoConnection.hasConnection()) {
      initialiseArduinoConnection();
    }

     while (!handShake(ArduinoMotorDriver::SERIAL_ECHO_VERBOSE)) {}
    
    std::string currentString;
    do {
      currentString = m_arduinoConnection.serialReadString();
      LOG_INFO(currentString);
    } while (currentString != "");
}

bool ArduinoMotorDriver::benchmarkSD(const int& i_numberOfMessages) {
  std::vector<std::vector<int>> totalVector;
  std::vector<int> thisMessage;
  int numberOfBytes;
  createRandomMessages(i_numberOfMessages,
                       &totalVector);
  for (auto message = totalVector.begin();
       message != totalVector.end();
       message++) {
    upload(*message);
  }
  while (!handShake(ArduinoMotorDriver::SERIAL_ECHO));
  //  return true;

  int i = 0;
  for (auto message = totalVector.begin();
       message != totalVector.end();
       message++) {
    LOG_DEBUG("Verifying message: " << i);
    thisMessage = *message;
    int expectedSize = (thisMessage.size()-1) * 2;
    numberOfBytes = m_arduinoConnection.serialRead(1);
    if (numberOfBytes !=  expectedSize) {
      LOG_DEBUG("Expected number of bytes: " << expectedSize);
      LOG_DEBUG("Number of bytes send by arduino: " << numberOfBytes);
      //      LOG_ERROR("Number of send bytes is not what expected!");
    }
    
    int value;
    for (int i = 0;
         i < numberOfBytes/2;
         i++) {
      value = m_arduinoConnection.serialRead(2);
      if (thisMessage[i+1] != value) {
        LOG_DEBUG("Requested: " << thisMessage[i+1]);
        LOG_DEBUG("Given: " << value);
        //        LOG_ERROR("Failed benchmark!");
      }
    }
    i++;
  }
  return true;
}

void ArduinoMotorDriver::createRandomMessages(const int& i_numberOfMessages,
                                              std::vector< std::vector<int> >* i_totalMessagePointer) {
  unsigned int seed = 10;
  for (int i= 0;
       i < i_numberOfMessages;
       i++) {
    std::vector<int> messageVector;
    int numberOfSteps = rand_r(&seed) % 3 + 1;
    messageVector.push_back(sizeof(numberOfSteps) * 2 +
                            sizeof(numberOfSteps) * numberOfSteps);
    messageVector.push_back(rand_r(&seed) % 10000);  // speed
    messageVector.push_back(rand_r(&seed) % 50+1);  // number of repititions

    LOG_DEBUG("Number of steps: " << numberOfSteps);
    LOG_DEBUG("Message size: " << messageVector[0]);
    LOG_DEBUG("Speed: " << messageVector[1]);
    LOG_DEBUG("Repitions: " << messageVector[2]);
    for (int j = 0;
         j < numberOfSteps;
         j++) {
      messageVector.push_back(rand_r(&seed) % 125 + 3);
    }
    i_totalMessagePointer->push_back(messageVector);
  }
}
