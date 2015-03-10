// Copyright [2014] Ruud Cools
#ifndef ARDUINO_ARDUINOMOTORDRIVER_H_
#define ARDUINO_ARDUINOMOTORDRIVER_H_

#include "./ArduinoSerialConnection.h"


class ArduinoMotorDriver {
    GETSET(std::vector<int>, m_bufferMessage, BufferMessage);
    GETSET(ArduinoSerialConnection, m_arduinoConnection, ArduinoConnection);
    GETSET(std::string, m_serialRegularExpresion, SerialRegularExpresion);
    GETSET(bool, m_reducedSpeed, ReducedSpeed);

 private:
    void sendMessage(const std::string&);
    std::string getSerialFileName();
    void initialiseArduinoConnection();
    void handShake();
 public:
    void actuate(const char* i_buffer,
                 const int i_messageSize);
    bool hasConnection();
    bool resetConnection();
    // Constructors
    explicit ArduinoMotorDriver(std::string);
    ArduinoMotorDriver();
};

#endif  // ARDUINO_ARDUINOMOTORDRIVER_H_

