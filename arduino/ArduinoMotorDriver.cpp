// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <PinStateSequence.h>
#include "./ArduinoMotorDriver.h"


ArduinoMotorDriver::ArduinoMotorDriver()
    : m_serialRegularExpresion("lalal") {
  initialiseArduinoConnection();
}


ArduinoMotorDriver::ArduinoMotorDriver(std::string i_serialRegularExpression)
    : m_serialRegularExpresion(i_serialRegularExpression) {
  initialiseArduinoConnection();
}


void ArduinoMotorDriver::sendMessage(const std::string& i_message) {
    m_arduinoConnection.serialWrite(i_message);
}


void ArduinoMotorDriver::initialiseArduinoConnection() {
  LOG_DEBUG("Initialising Arduino Connection");
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


bool ArduinoMotorDriver::handShake() {
  int receivedHandShake = 10000;
  int defaultHandShakeValue = 200;
  int defaultIntSizeValue = static_cast<int>(sizeof(defaultHandShakeValue));
  int receivedIntSizeValue = 10000;
  int attempts = 0;

  receivedHandShake = m_arduinoConnection.serialRead(1);

  if (defaultHandShakeValue != receivedHandShake){
    LOG_DEBUG("Hand shake value: " << defaultHandShakeValue <<
              "received value: " << receivedHandShake);
    LOG_ERROR("Bad hand shake");
    return false;
  }

  m_arduinoConnection.serialWrite(defaultHandShakeValue);
  m_arduinoConnection.serialWrite(defaultIntSizeValue);

  attempts = 0;

  while ((receivedIntSizeValue != defaultIntSizeValue) & (attempts < 100) ){
    receivedIntSizeValue = m_arduinoConnection.serialRead(1);
    attempts++;
  }
  
  if (receivedIntSizeValue != defaultIntSizeValue) {
    LOG_DEBUG("Int size value: " << defaultIntSizeValue <<
              "received value: " << receivedIntSizeValue);
    return false;
  }
  
  return true;
}


void ArduinoMotorDriver::actuate(const std::vector<int> i_messageVector) {
  if (!m_arduinoConnection.hasConnection()) {
    initialiseArduinoConnection();
  }

  unsigned char calculatedCRC(0);
  unsigned char receivedCRC;
  
  if (i_messageVector.size()*sizeof(*i_messageVector.data()) > 255)
    LOG_ERROR("Message is bigger then 255 chars," <<
              " which cannot be send easely of the serial");

  while(!handShake()) {}

  for (auto itr = i_messageVector.begin();
       itr != i_messageVector.end();
       itr++) {
    m_arduinoConnection.serialWrite(reinterpret_cast<const unsigned char*>(&(*itr)),
                                    sizeof(*itr));
    calculatedCRC += *itr;
  }

  receivedCRC = m_arduinoConnection.serialRead(1);
  m_arduinoConnection.serialWrite(calculatedCRC);
  LOG_INFO("read: " << static_cast<int>(receivedCRC)
           << "Calc: " << static_cast<int>(calculatedCRC));
  
  if (receivedCRC != calculatedCRC)
    LOG_ERROR("fail!");

}


bool ArduinoMotorDriver::sendTestBit() {
  while(!handShake()) {
    LOG_DEBUG("Hand shake went bad!");
  }

  unsigned char* position;
  int testInt [] = {144,360,1,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  unsigned char crc = 0;
  for (int i = 0;
       i < 37;
       i++) {
    position = reinterpret_cast<unsigned char*>(testInt+i);
    m_arduinoConnection.serialWrite(position, sizeof(testInt[0]));
    crc += testInt[i];
  }

  unsigned char crcBit = m_arduinoConnection.serialRead(1);
  m_arduinoConnection.serialWrite(crc);
  LOG_INFO("read: " << static_cast<int>(crcBit)
           << "Calc: " << static_cast<int>(crc));
  
  if (crcBit != crc)
    LOG_ERROR("fail!");
  return true;
}


bool ArduinoMotorDriver::resetConnection() {
  m_arduinoConnection.resetConnection();
  return true;
}
