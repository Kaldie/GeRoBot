// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include "./ArduinoMotorDriver.h"


ArduinoMotorDriver::ArduinoMotorDriver()
    : m_serialRegularExpresion("lalal")
{}


ArduinoMotorDriver::ArduinoMotorDriver(std::string i_serialRegularExpression)
    : m_serialRegularExpresion(i_serialRegularExpression)
{}


void ArduinoMotorDriver::verify(const int& i_input,
                                const std::string& i_assignmentString) {
  LOG_INFO(((i_input == 1)? "true" : "false"));
  if (false) {
    LOG_ERROR("'" << i_assignmentString << "', has not succeed!");
    throw 100;
  }
}

void ArduinoMotorDriver::sendMessage(const std::string& i_message) {
    m_arduinoConnection.serialWrite(i_message);
}


void ArduinoMotorDriver::initialiseArduinoConnection() {
  LOG_DEBUG("Initialising Arduino Connection");
  m_arduinoConnection.setPortName(getSerialFileName());
  m_arduinoConnection.setBaudRate(115200);
  m_arduinoConnection.setBlockThread(false);
  m_arduinoConnection.setMinimumBytePerMessage(1);
  m_arduinoConnection.setCloseHandleAfterMessage(false);
}


void ArduinoMotorDriver::sendSpeedMessage(const bool& i_reducedSpeed) {
  if (m_reducedSpeed) {
    sendMessage("Set DefaultPinValue:255");
    sleep(2);
    sendMessage("Set Actuate:Debug");
  } else {
    sendMessage("Set DefaultPinValue:255");
    sleep(2);
    sendMessage("Set Actuate:Normal");
  }
}


void ArduinoMotorDriver::actuate() {
  initialiseArduinoConnection();
  m_arduinoConnection.resetConnection();
  sleep(2);
  sendSpeedMessage(m_reducedSpeed);
  sleep(2);

  LOG_INFO("Lenght of the vector is:" << m_buffer.size());
  int numberOfBytesSend = 0;
  std::vector<int>::iterator itr = m_buffer.begin();
  int sendNumberOfBytes = 5;

  while (itr != m_buffer.end()) {
    if (std::distance(itr, m_buffer.end()) < sendNumberOfBytes)
      sendNumberOfBytes = std::distance(itr, m_buffer.end());

    LOG_INFO("Number of bytes that are going to be send: "
              << sendNumberOfBytes);

    if (sendNumberOfBytes>0) {
      m_arduinoConnection.serialWrite(itr, sendNumberOfBytes);
      numberOfBytesSend+=sendNumberOfBytes;
      LOG_INFO("Total number of bytes send: " << numberOfBytesSend);
      LOG_INFO("Send: " <<
               static_cast<double>(numberOfBytesSend)/m_buffer.size()*100.0
               << "% of the total track");
    }

    std::string serialOutput = m_arduinoConnection.serialRead(1);
    LOG_DEBUG("Read: " << serialOutput);
    sendNumberOfBytes = static_cast<int>(serialOutput.c_str()[0])/2;
    LOG_DEBUG("Converted to int: " << sendNumberOfBytes);
  }

  m_arduinoConnection.serialWrite(4+8+16+32+64+128);
  usleep(100);
  m_arduinoConnection.serialWrite(3);
}


std::string ArduinoMotorDriver::getSerialFileName() {
    glob_t globbuf;
    glob(m_serialRegularExpresion.c_str(), 0, NULL, &globbuf);
    if (globbuf.gl_pathc == 0) {
      LOG_ERROR("Could not find any devices using: '" <<
                m_serialRegularExpresion << "' as expression!");
      throw 100;
    } else if (globbuf.gl_pathc > 1) {
      LOG_ERROR("Multiple paths found, please refine regular expression: "
                <<m_serialRegularExpresion);
      throw 100;
    }

    std::string thisPath;
    for (size_t i = 0; i < globbuf.gl_pathc; i++) {
      thisPath = globbuf.gl_pathv[i];
      LOG_INFO(thisPath);
    }

    globfree(&globbuf);
    return thisPath;
}

bool ArduinoMotorDriver::hasConnection() {
  LOG_DEBUG("reached here!");
  try {
    LOG_DEBUG("checking if there is an connection available @: "
              << getSerialFileName());
    initialiseArduinoConnection();
    m_arduinoConnection.resetConnection();
    return true;
  } catch(std::runtime_error) {
    return false;
  }
}
