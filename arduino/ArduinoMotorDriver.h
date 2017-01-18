// Copyright [2014] Ruud Cools
#ifndef ARDUINO_ARDUINOMOTORDRIVER_H_
#define ARDUINO_ARDUINOMOTORDRIVER_H_

#include <SequenceVector.h>
#include "./ArduinoSerialConnection.h"

\
class ArduinoMotorDriver {
 private:
  typedef std::tuple<int,int> RangeTuple;
  
  GETSET(ArduinoSerialConnection, m_arduinoConnection, ArduinoConnection);
  GETSET(std::string, m_serialRegularExpresion, SerialRegularExpresion);

  // the range in which the joint pins can reside (INLCUSSIVE)
  GETSET(RangeTuple, m_jointPinRange, JointPinRange);

  // the range in which the end stop pins can reside
  GETSET(RangeTuple, m_stopPinRange, StopPinRange);

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
  static const char* EndStopHasBeenHitMessage;
  /// Upload the message to the arduino
  void upload(const std::vector<int> i_messageVector);
  /**
   * Send the command the arduino that it can unlock go in normal mode.
   * It returns the pin settings at which it currently is
   */
  void resolveEndStopHit(int* o_jointValue,
			 int* o_stopValue);
  /// Determine if the motor is ready for more
  bool sendsHandShake(const bool& i_blocks);
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

