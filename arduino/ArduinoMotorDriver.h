#ifndef ArduinoMotorDriver_H
#define ArduinoMotorDriver_H

#include <macroHeader.h>
#include "ArduinoSerialConnection.h"
#include <vector>


class ArduinoMotorDriver
{
    GETSET(std::vector<int>,m_buffer,Buffer);
    GETSET(ArduinoSerialConnection,m_arduinoConnection,ArduinoConnection);
    GETSET(std::string,m_serialRegularExpresion,SerialRegularExpresion);
    GETSET(bool,m_reducedSpeed,ReducedSpeed);
    
 private:
    void verify(const int&,
		const std::string&);
    void sendMessage(const std::string&);
    std::string getSerialFileName();

 public:
    
    void actuate();
    ArduinoMotorDriver(std::string);
    ArduinoMotorDriver();
    
    
};

#endif // ArduinoMotorDriver
