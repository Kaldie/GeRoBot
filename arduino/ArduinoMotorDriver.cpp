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
      handShake();
  }
}


bool ArduinoMotorDriver::handShake() {
  int handShake = 200;

  m_arduinoConnection.flushConnection();
  std::string handShakeValue = m_arduinoConnection.serialRead(1);
  LOG_DEBUG("Hand shake value: " << handShakeValue);
  m_arduinoConnection.serialWrite(handShake);

  m_arduinoConnection.serialWrite(static_cast<int>(sizeof(handShake)));
  std::string intSizeValue = m_arduinoConnection.serialRead(1);
  LOG_DEBUG("Echo'd int size: " << intSizeValue);
  return true;
}


void ArduinoMotorDriver::actuate(const char* i_buffer,
                                 const int i_messageSize) {
  if (!m_arduinoConnection.hasConnection()) {
    initialiseArduinoConnection();
    sleep(2);
  }

  if (i_messageSize > 255)
    LOG_ERROR("Message is bigger then 255 chars," <<
              " which cannot be send easely of the serial");

  m_arduinoConnection.serialWrite(i_buffer, i_messageSize);
}


bool ArduinoMotorDriver::sendTestBit() {
  char* position;
  int testInt [7] = {24,1,10,8,4,16, 4};

  int i = 0;
  for (auto itr = testInt;
       i < 7;
       i++, itr++) {
    LOG_DEBUG("reintrp");
    position = reinterpret_cast<char*>(testInt+i);
    LOG_DEBUG("Serial write");
    m_arduinoConnection.serialWrite(position, sizeof(testInt[0]));
    LOG_DEBUG("done write");
  }
  return true;
}


bool ArduinoMotorDriver::resetConnection() {
  m_arduinoConnection.resetConnection();
  return true;
}
