// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <PinStateSequence.h>
#include "./ArduinoMotorDriver.h"

const int ArduinoMotorDriver::HAND_SHAKE_VALUE = 200;
const int ArduinoMotorDriver::ACTUATE_MODE_VALUE = 100;
const int ArduinoMotorDriver::ECHO_MODE = 150;
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
    case ArduinoMotorDriver::ECHO : {
      modeValue = ArduinoMotorDriver::ACTUATE_MODE_VALUE;
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


bool ArduinoMotorDriver::deleteFile() {
     if (!m_arduinoConnection.hasConnection()) {
      initialiseArduinoConnection();
    }
    while (!handShake(ArduinoMotorDriver::DELETE_FILE)) {}
}

bool ArduinoMotorDriver::deleteFile() {
     if (!m_arduinoConnection.hasConnection()) {
      initialiseArduinoConnection();
    }
    while (!handShake(ArduinoMotorDriver::ECHO)) {}
}
