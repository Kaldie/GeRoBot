#ifndef ArduinoSerialConnection_H
#define ArduinoSerialConnection_H

#include <vector>
#include <macroHeader.h>
#include <termios.h>
#include <string>

class ArduinoSerialConnection
{
 private:
    //Properties
    
    GETSET(int,m_deadTimeBetweenMessage,DeadTimeBetweenMessage);
    GETSET(int,m_minimumBytePerMessage,MinimumBytePerMessage);
    GETSET(bool,m_closeHandleAfterMessage,CloseHandleAfterMessage);
    GETSET(bool,m_blockThread,BlockThread);

    speed_t m_baudRate;

    GETSET(int,m_fileHandle,FileHandle);
    GETSET(std::string,m_portName,PortName);

    void openConnection();
    void closeConnection();

 public:

    void setBaudRate(const int&);
    void resetConnection();

    std::string serialRead(const int&);

    void serialWrite(std::string);
    void serialWrite(const int&);

    void serialWrite(char*,const int&);
    void serialWrite(std::vector<int>::iterator&,
		     const int&);

    //Constructors
    ArduinoSerialConnection();

    ArduinoSerialConnection(std::string);

    ArduinoSerialConnection(const std::string&,
			    const int&,
			    const int&,
			    const int&,
			    const bool&,
			    const bool&);
    //Deconstructor
    ~ArduinoSerialConnection();
};    

#endif // ArduinoSerialConnection
