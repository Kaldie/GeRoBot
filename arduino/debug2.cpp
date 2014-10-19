#include <string>
#include <stdio.h>
#include <iostream>
#include "ArduinoSerialConnection.h"
#include "ArduinoMotorDriver.h"

int main()
{
    ArduinoMotorDriver driver;
    driver.setSerialRegularExpresion("/dev/ttyUSB*");
    driver.setReducedSpeed(false);
    std::vector<int> bitVector={4,8,12,16,20,24,28,32,36,40,4,8,12,16,20,24,28,32,36,40,4,8,12,16,20,24,28,32,36,40,255};
    std::vector<int> bufferVector;

    for(int i=0;
	i<100;
	i++)
	bufferVector.insert(bufferVector.end(),bitVector.begin(),bitVector.end());

    LOG_INFO("Lenght of buffer vector:"<<bufferVector.size());
    driver.setBuffer(bufferVector);
    driver.actuate();
    /*    ArduinoSerialConnection connection=ArduinoSerialConnection("/dev/ttyUSB1");
    connection
    int numbers[]={1,2,3,4,5,6,7,8,9,10};
    //    char output[255];
    std::string stringValue;

    for(int j=0;j<100;j++)
	{
	    for(int i=0;i<=9;i++)
		{
		    //		    stringValue=std::to_string(numbers[i]);
		    //	    std::cout<<"String value: "<<stringValue<<std::endl;
		    //		    connection.serialWrite(numbers[i]);
		    //		    usleep(1000);
		    //	    connection.serialWrite("\n");
		    connection.serialRead(1);

		}
	}
    */ 
}

