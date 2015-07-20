// Copyright [2014] Ruud Cools
#ifndef ARDUINO_ARDUINOMOTORDRIVER_H_
#define ARDUINO_ARDUINOMOTORDRIVER_H_

#include "./ArduinoSerialConnection.h"

class ArduinoMotorDriver {
    GETSET(ArduinoSerialConnection, m_arduinoConnection, ArduinoConnection);
    GETSET(std::string, m_serialRegularExpresion, SerialRegularExpresion);

    enum DriverStatus {UPLOAD = 0,
                       ACTUATE,
                       SERIAL_ECHO_VERBOSE,
                       SERIAL_ECHO,
                       DELETE_FILE,
                       ERROR};

    static const int HAND_SHAKE_VALUE;
    static const int UPLOAD_MODE_VALUE;
    static const int ACTUATE_MODE_VALUE;
    static const int ECHO_MODE_VALUE;
    static const int ECHO_MODE_VERBOSE_VALUE;
    static const int DELETE_FILE_MODE_VALUE;

 private:
    std::string getSerialFileName();
    void initialiseArduinoConnection();
    bool handShake(DriverStatus i_status);
    void createRandomMessages(const int& i_numberOfMessages,
                              std::vector< std::vector<int> > *i_totalVector);

public:
    void upload(const std::vector<int> i_messageVector);
    void actuate();
    bool resetConnection();
    bool sendTestBit();
    void deleteFile();
    void echo();
    bool benchmarkSD(const int& i_numberOfMessages);
    // Constructors
    explicit ArduinoMotorDriver(std::string i_regExpression);
    ArduinoMotorDriver();
};

#endif  // ARDUINO_ARDUINOMOTORDRIVER_H_

