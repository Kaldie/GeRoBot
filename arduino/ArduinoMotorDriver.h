// Copyright [2014] Ruud Cools
#ifndef ARDUINO_ARDUINOMOTORDRIVER_H_
#define ARDUINO_ARDUINOMOTORDRIVER_H_

#include <SequenceVector.h>
#include "./ArduinoSerialConnection.h"

\
class ArduinoMotorDriver {
    GETSET(ArduinoSerialConnection, m_arduinoConnection, ArduinoConnection);
    GETSET(std::string, m_serialRegularExpresion, SerialRegularExpresion);

    enum DriverStatus {UPLOAD = 0,
                       ACTUATE,
                       SERIAL_ECHO_VERBOSE,
                       SERIAL_ECHO,
                       DELETE_FILE,
                       ERROR};

 private:
    static const int HAND_SHAKE_VALUE;
    static const int ENDSTOP_HIT_VALUE;
    static const int UPLOAD_MODE_VALUE;
    static const int ACTUATE_MODE_VALUE;
    static const int ECHO_MODE_VALUE;
    static const int ECHO_MODE_VERBOSE_VALUE;
    static const int DELETE_FILE_MODE_VALUE;

    void initialiseArduinoConnection();
    bool handShake(DriverStatus i_status);
    void createRandomMessages(const int& i_numberOfMessages,
                              std::vector< std::vector<int> > *i_totalVector);

public:
    /// Upload the message to the arduino
    void upload(const std::vector<int> i_messageVector);
    /**
     * Send the command the arduino that it can unlock go in normal mode.
     * It returns the pin settings at which it currently is
     */
    int resolveEndStopHit();
    /// Send an actuate command to the arduino
    void actuate();
    /// Send a test message to the arduino
    bool sendTestBit();
    /// Command the arduino to delete the step file
    void deleteFile();
    /// Request that the arduino display its information of messages
    void echo();
    /**
     * Brief Check if there is a connection,
     * if i_makeConnection is true, 1 attempt is made to connect to the driver
     */
    bool hasConnection(const bool& i_makeConnection = false);
    /// Benchmark the SD card on the system. Testing purpose often
    bool benchmarkSD(const int& i_numberOfMessages);
    bool benchmarkCurrentRobot(const SequenceVector& i_sequenceVector);
    // Default constructors
    ArduinoMotorDriver();
    /// Constructor with path to serial
    explicit ArduinoMotorDriver(std::string i_regExpression);
};

#endif  // ARDUINO_ARDUINOMOTORDRIVER_H_

