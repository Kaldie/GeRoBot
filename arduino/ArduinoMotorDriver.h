// Copyright [2014] Ruud Cools
#ifndef ARDUINO_ARDUINOMOTORDRIVER_H_
#define ARDUINO_ARDUINOMOTORDRIVER_H_

#include "./ArduinoSerialConnection.h"


class ArduinoMotorDriver {
    GETSET(std::vector<int>, m_buffer, Buffer);
    GETSET(ArduinoSerialConnection, m_arduinoConnection, ArduinoConnection);
    GETSET(std::string, m_serialRegularExpresion, SerialRegularExpresion);
    GETSET(bool, m_reducedSpeed, ReducedSpeed);

 private:
    void verify(const int&,
                const std::string&);

    void sendMessage(const std::string&);
    std::string getSerialFileName();
    void initialiseArduinoConnection();
    void sendSpeedMessage(const bool&);

 public:
    void actuate();
    bool hasConnection();
    
    // Constructors
    explicit ArduinoMotorDriver(std::string);
    ArduinoMotorDriver();
    
};

#endif  // ARDUINO_ARDUINOMOTORDRIVER_H_

