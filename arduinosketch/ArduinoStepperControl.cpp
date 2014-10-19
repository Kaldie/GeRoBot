#include <string.h>
#include <stdarg.h>
#include <Arduino.h>
#include "ByteBuffer.h"

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

//Buffer definitions and initialisation
#define BUFFER_SIZE  100
#define BAUD_RATE 115200

#define NDEBUG

#ifdef NDEBUG
#define DEBUG_MSG_RCS(message) message
#else
#define DEBUG_MSG_RCS(message) Serial.println(message)
#endif

ByteBuffer<int> byteBuffer(BUFFER_SIZE);
char readBuffer[BUFFER_SIZE];
//Defining if we're currently actuating
bool isActuating=false;

int numberOfStepsWithoutData=0;
int defaultPinValue=4;
// virtual functions
extern void blink(int);
extern void setPins(byte);

//example buffer filling
void makeKnightRiderBuffer();

//Timer interupt control functions
void setTimer0Interupt(int, int);
void setTimer1Interupt(int, int);

//Heart beat constrol functions
void setHeartBeatTo10();
void setHeartBeatTo1K();
void setHeartBeatTo5K();
void setHeartBeatTo10K();
void setHeartBeatTo25K();
void disableHeartBeat();

bool verifyPoisonedPill(char*);

void setTimer1Interupt(int i_TCCR1B,int i_OCR1A)
{
    cli();//stop interrupts

    //set timer1 interrupt at 1Hz
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    // set compare
    //match register for 1hz increments
    OCR1A = i_OCR1A;//i_OCR1A;// Default = 15624 => (16*10^6) / (1*1024) - 1 (must be <65536) => 1 Hz

    if(!i_TCCR1B)
	{
	    // turn on CTC mode
	    TCCR1B |= (1 << WGM12);
	    // Set CS12 and CS10 bits for 1024 prescaler
	    TCCR1B |= (1 << CS12) | (1 << CS10);  
	}
    else
	TCCR1B=i_TCCR1B;

    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    sei();//allow interrupts
    
}

void setTimer0Interupt(int i_TCCR0B,int i_OCR0A)
{
    cli();//stop interrupts

    //set timer0 interrupt at 2kHz
    TCCR0A = 0;// set entire TCCR0A register to 0
    TCCR0B = 0;// same for TCCR0B
    TCNT0  = 0;//initialize counter value to 0
    // set compare match register for 2khz increments
    OCR0A = i_OCR0A;// = (16*10^6) / (2000*64) - 1 (must be <256)
    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00);   
    // enable timer compare interrupt


    if(!i_TCCR0B)
	{
	    // turn on CTC mode
	    TCCR0B |= (1 << WGM02);
	    // Set CS12 and CS10 bits for 1024 prescaler
	    TCCR0B |= (1 << CS02) | (1 << CS00);  
	}
    else
	TCCR0B=i_TCCR0B;

    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);

    sei();//allow interrupts
    
}


void disableHeartBeat()
{
    cli();//stop interrupts
    
    //set timer0 interrupt at 2kHz
    TCCR1A = 0;// set entire TCCR0A register to 0
    TCCR1B = 0;// same for TCCR0B
    TCNT1  = 0;//initialize counter value to 0
    OCR1A  = 0;
    TIMSK1 = 0;
    isActuating=false;
    sei(); //allow interrupts
    Serial.flush();
}


bool verifyPoisonedPill(byte i_buffer)
{
    if((int)i_buffer==3)
	{
	    DEBUG_MSG_RCS("Posionous pill detected!!"); 
	    return true;
	}
    else
	{
	    DEBUG_MSG_RCS("Posionous not pill detected!!"); 
	    return false;
	}
}


void setHeartBeatTo1K()
{
    int tccr1b=0;
    tccr1b |= (1 << WGM12);// turn on CTC mode
    tccr1b|=(1<<CS11) | (1<<CS10);//prescale factor of 64
    int compareValue=249;//= (16*10^6)/(1*10^3*64) -1
    setTimer1Interupt(tccr1b,compareValue);
}


void setHeartBeatTo10K()
{
    int tccr1b=0;
    tccr1b |= (1 << WGM12);// turn on CTC mode
    tccr1b|=(1<<CS11) | (1<<CS10);//prescale factor of 64
    int compareValue=24;//= (16*10^6)/(5*10^4) -1
    setTimer1Interupt(tccr1b,compareValue);
}

void setHeartBeatToMax()
{
    int tccr1b=0;
    tccr1b |= (1 << WGM12);// turn on CTC mode
    tccr1b|=(1<<CS11) | (1<<CS10);//prescale factor of 64
    int compareValue=22;//= (16*10^6)/(5*10^4) -1
    setTimer1Interupt(tccr1b,compareValue);
}

void setHeartBeatTo5K()
{
    int tccr1b=0;
    tccr1b |= (1 << WGM12);// turn on CTC mode
    tccr1b|=(1<<CS11) | (1<<CS10);//prescale factor of 64
    int compareValue=149;//= (16*10^6)/(5*10^4) -1
    setTimer1Interupt(tccr1b,compareValue);
}


void setHeartBeatTo10()
{
    int tccr1b=0;
    tccr1b |= (1 << WGM12);// turn on CTC mode
    tccr1b|=(1<<CS12) | (1<<CS10);//prescale factor of 64
    int compareValue=1559;
    setTimer1Interupt(tccr1b,compareValue);
}


void setPinOutputs()
{
    //Always hook up the 2 t/m 7 inputs!
    DDRD = DDRD | B11111100;
}


ISR(TIMER1_COMPA_vect)
{
    int availableElements=byteBuffer.getAvailableElements();

    if(availableElements<=15)
	{
	    int serialAvailableBytes= Serial.available();
	    
	    if(serialAvailableBytes>0)
		{
		    int numberOfReads;
		    int availableStorage=byteBuffer.getAvailableStorage();
		    if(serialAvailableBytes>=availableStorage)
			numberOfReads=availableStorage;
		    else
			numberOfReads=serialAvailableBytes;

		    Serial.write(byteBuffer.getAvailableStorage());
		    DEBUG_MSG_RCS("Available Storage: ");
		    DEBUG_MSG_RCS(byteBuffer.getAvailableStorage());

		    DEBUG_MSG_RCS("Number of reads: ");
		    DEBUG_MSG_RCS(numberOfReads);
		    

		    Serial.readBytes(readBuffer,numberOfReads);
		    for(int i=0;
			i<numberOfReads;
			i++)
			{
			    byteBuffer.set(readBuffer[i]);
			}
		}
	}
    
    if(byteBuffer.getAvailableElements()>0)
	blink(1);
    //    DEBUG_MSG_RCS("Available storage: ");
    //    byteBuffer.getAvailableStorage();
}


ISR(TIMER0_COMPA_vect)
{
    
    //    setPins(byteBuffer.get());
    //    Serial.write(1);
}


void setPins(byte i_byte)
{
    //    Serial.write(i_byte);
    i_byte &= B11111100;
    PORTD=i_byte;
    
}


void blink(int i_numberOfBlinks)
{
    byte thisByte;
    for(int i=0;i<i_numberOfBlinks;i++)
	{
	    thisByte=byteBuffer.get();
	    //digitalWrite(13,LOW);
	    if(verifyPoisonedPill(thisByte))
		{
		    setPins(defaultPinValue);
		    disableHeartBeat();
		    break;
		}
	    
	    setPins(thisByte);
	}
}


void makeKnightRiderBuffer()
{
    
    for(int i=0;i<12;i++)
	{
	    if(i<7)
		byteBuffer.set(1<<i+1);
	    else
		{
		    int k=1<<6;
		    byteBuffer.set(k>>i%(6+1));
		}
	}

}


const int readMessage(char* i_buffer,int i_numberOfChars)
{
    int numberOfBytesAvailable=Serial.available();
    int readBytes=0;
    // Don't read unless there you know there is data
    DEBUG_MSG_RCS("Serial data available: ");

    char b[10]={};
    sprintf(b,"%d",numberOfBytesAvailable);

    DEBUG_MSG_RCS(b);
    
    if(numberOfBytesAvailable > 0)
	{
	    readBytes= Serial.readBytes(i_buffer,i_numberOfChars);
	    i_buffer[readBytes]=0;
	}
    
    else
	return 0;

}


void parseCommand()
{
    char command[4]={};
    readMessage(command,3);
    if(strcmp(command,"Set")==0)
	{

	    DEBUG_MSG_RCS("Going into setting mode!");
	    char payLoad[25];
	    if(readMessage(payLoad,24))
		{

		    char* attribute=strtok(payLoad,":")+1;
		    char* value=strtok(NULL,":"); 
	    
		    DEBUG_MSG_RCS(attribute);
		    DEBUG_MSG_RCS(value);
		    
		    //	    strncpy(attribute,payLoad,(value-1)-payLoad);
		    
		    if(strcmp(attribute,"BaudRate")==0)
			{
			    DEBUG_MSG_RCS("Serial connection cannot be changed without resetting the arduino!");
			    DEBUG_MSG_RCS("Set connection speed to:");
			    DEBUG_MSG_RCS(char(BAUD_RATE));
			}
		    else if(strcmp(attribute,"Blink")==0)
			{
			    DEBUG_MSG_RCS("Blinking!");
			    byteBuffer.set(atoi(value));
			    blink(1);
			}
		    else if(strcmp(attribute,"DefaultPinValue")==0)
			{
			    DEBUG_MSG_RCS("Setting default pin value");
			    defaultPinValue=(atoi(value));
			}
		    
		    else if(strcmp(attribute,"Actuate")==0)
			{
			    if(strcmp(value,"Normal")==0)
				{
				    DEBUG_MSG_RCS("Setting actuation to 1K");
				    isActuating=true;
				    setHeartBeatToMax();
				}
			    else if(strcmp(value,"Debug")==0)
				{
				    isActuating=true;
				    DEBUG_MSG_RCS("Setting actuation to 10");
				    setHeartBeatTo10();
				}
			}
		    else
			DEBUG_MSG_RCS("yay!");
		    
		    DEBUG_MSG_RCS("Done in setting mode!");
		}
	}
    else
	DEBUG_MSG_RCS(command);

    while( Serial.read() != -1 );
}


void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  Serial.begin(BAUD_RATE);
  setPinOutputs();
  DEBUG_MSG_RCS("done setting");
}


void loop()
{
    if(Serial.available()>=3 and not isActuating)
	parseCommand();
    
}
