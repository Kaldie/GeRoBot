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
}


bool ArduinoMotorDriver::handShake() {
  int handShake = 200;
  int handShakeValue = m_arduinoConnection.serialRead(1);

  if(handShake != handShakeValue) {
    LOG_DEBUG("Bad Hand shake!!!!");
    return ArduinoMotorDriver::handShake();
  }
    
  LOG_DEBUG("Hand shake value: " << handShakeValue);
  m_arduinoConnection.serialWrite(handShake);

  m_arduinoConnection.serialWrite(static_cast<int>(sizeof(handShake)));
  int intSizeValue = m_arduinoConnection.serialRead(1);
  
  LOG_DEBUG("Echo'd int size: " << intSizeValue);
  return true;
}


void ArduinoMotorDriver::actuate(const char* i_buffer,
                                 const int i_messageSize) {
  if (!m_arduinoConnection.hasConnection()) {
    initialiseArduinoConnection();
  }

  if (i_messageSize > 255)
    LOG_ERROR("Message is bigger then 255 chars," <<
              " which cannot be send easely of the serial");
  handShake();
  m_arduinoConnection.serialWrite(i_buffer, i_messageSize);
}


bool ArduinoMotorDriver::sendTestBit() {
  handShake();

  char* position;
  int testInt [] = {24,2,1,4,8,16,8};

  int i = 0;
  for (auto itr = testInt;
       i < 7;
       i++, itr++) {
    position = reinterpret_cast<char*>(testInt+i);
    m_arduinoConnection.serialWrite(position, sizeof(testInt[0]));
  }

  int crcBit = m_arduinoConnection.serialRead(1);
  LOG_INFO((int)crcBit);
  return true;
}


bool ArduinoMotorDriver::resetConnection() {
  m_arduinoConnection.resetConnection();
  return true;
}
