// Copyright [2014] Ruud Cools
#ifndef ARDUINO_ARDUINOSERIALCONNECTION_H_
#define ARDUINO_ARDUINOSERIALCONNECTION_H_

class ArduinoSerialConnection {
 private:
  // Properties
  GETSET(int, m_deadTimeBetweenMessage, DeadTimeBetweenMessage);
  GETSET(int, m_minimumBytePerMessage, MinimumBytePerMessage);
  GETSET(bool, m_closeHandleAfterMessage, CloseHandleAfterMessage);
  GETSET(bool, m_blockThread, BlockThread);
  GETSET(int, m_fileHandle, FileHandle);
  GETSET(std::string, m_portName, PortName);
  GET(speed_t, m_baudRate, BaudRate);

  void openConnection();
  void closeConnection();
  std::string resolvePortName() const;
  int rawSerialRead(const int& i_numberOfBytesReading,
                         unsigned char*);
    
 public:
  void setBaudRate(const int&);
  void resetConnection();
  bool hasConnection() const;

  int serialRead(const int& i_numberOfBytes);
  //  std::string serialRead(const int& i_numberOfBytes);
  std::string serialReadString();
  void serialWrite(std::string);
  void serialWrite(const int&);

  void serialWrite(const unsigned char*, const int&);
  void serialWrite(std::vector<int>::iterator&, const int&);

  bool flushConnection() const;
  // Constructors
  ArduinoSerialConnection();

  explicit ArduinoSerialConnection(std::string);

  ArduinoSerialConnection(const std::string&,
                          const int&,
                          const int&,
                          const int&,
                          const bool&,
                          const bool&);
  // Deconstructor
  ~ArduinoSerialConnection();
};

#endif  // ARDUINO_ARDUINOSERIALCONNECTION_H_
