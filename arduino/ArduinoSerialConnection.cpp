// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include "./ArduinoSerialConnection.h"

ArduinoSerialConnection::ArduinoSerialConnection()
    : m_deadTimeBetweenMessage(0),
      m_minimumBytePerMessage(0),
      m_closeHandleAfterMessage(true),
      m_blockThread(false),
      m_baudRate(B115200),
      m_fileHandle(-1),
      m_portName("")
{}


ArduinoSerialConnection::ArduinoSerialConnection(std::string i_fileName)
    : m_deadTimeBetweenMessage(0),
      m_minimumBytePerMessage(0),
      m_closeHandleAfterMessage(true),
      m_blockThread(false),
      m_baudRate(B115200),
      m_fileHandle(-1),
      m_portName(i_fileName)
{}



ArduinoSerialConnection::ArduinoSerialConnection(const std::string& i_portName,
                                                 const int& i_minimumBytePerMessage,
						 const int& i_deadTimeBetweenMessage,
						 const int& i_baudRate,
						 const bool& i_closeAfterMessage,
						 const bool& i_blockThread):
    m_deadTimeBetweenMessage(i_deadTimeBetweenMessage),
    m_minimumBytePerMessage(i_minimumBytePerMessage),
    m_closeHandleAfterMessage(i_closeAfterMessage),
    m_blockThread(i_blockThread),
    m_baudRate(B115200),
    m_fileHandle(-1),
    m_portName(i_portName) {
    setBaudRate(i_baudRate);
}



void ArduinoSerialConnection::setBaudRate(const int& i_baudRate){
    /*
      Possible baud rates:
      300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, or 115200.
    */
  if (i_baudRate == 300)
    m_baudRate = B300;
  else if (i_baudRate == 600)
    m_baudRate = B600;
  else if (i_baudRate == 1200)
    m_baudRate = B1200;
  else if (i_baudRate == 4800)
    m_baudRate = B4800;
  else if (i_baudRate == 19200)
    m_baudRate = B19200;
  else if (i_baudRate == 115200)
    m_baudRate = B115200;
  else
    LOG_ERROR("Baud rate: "<< i_baudRate << " is not available!!");
}

ArduinoSerialConnection::~ArduinoSerialConnection() {
  closeConnection();
}


void ArduinoSerialConnection::closeConnection() {
  if (m_fileHandle != -1) {
    LOG_INFO("Closing connection!");
    close(m_fileHandle);
    m_fileHandle=-1;
  }
}


void ArduinoSerialConnection::resetConnection()
{
    closeConnection();
    openConnection();
}


void ArduinoSerialConnection::openConnection()
{

    struct termios tio;
    //make sure everthing is set to 0
    memset(&tio,0,sizeof(tio));

    // 8N1
    tio.c_cflag &= ~PARENB;// No parity detection
    tio.c_cflag &= ~CSTOPB;// 1 stop bit
    tio.c_cflag &= ~CSIZE;//  apply mask before setting the size of the byte
    tio.c_cflag &= ~CRTSCTS;// no flow control
    tio.c_cflag =  CS8|CREAD|CLOCAL; // 8n1, see termios.h for more information
    
    tio.c_cc[VMIN]=m_minimumBytePerMessage;
    tio.c_cc[VTIME]=m_deadTimeBetweenMessage; 
    
    tio.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    tio.c_oflag &= ~OPOST; // make raw

    cfsetospeed(&tio,m_baudRate); // 115200 baud
    cfsetispeed(&tio,m_baudRate); // 115200 baud

    if(m_blockThread)
			m_fileHandle=open(getPortName().c_str(), O_RDWR|O_NOCTTY |O_NDELAY);  
    else
			m_fileHandle=open(getPortName().c_str(), O_RDWR|O_NOCTTY);  

    LOG_INFO("Opening file to port: '"<< getPortName()<<"'");

    if (m_fileHandle == -1)  
	{

	    LOG_ERROR("Unable to open port'"<< getPortName()<<"'");
	    throw 100;
	}


    if(tcsetattr(m_fileHandle,TCSAFLUSH,&tio) )
	{
	    LOG_ERROR("Couldn't set term attributes");
	    throw 100;
	}
}


void ArduinoSerialConnection::serialWrite(std::string i_string)
{
    if(m_fileHandle==1)
	openConnection();

    LOG_INFO("Writing: "<<i_string);
    write(m_fileHandle,i_string.c_str(),i_string.length());

    if(m_closeHandleAfterMessage)
	closeConnection();
}

void ArduinoSerialConnection::serialWrite(const int& i_integer)
{
    if(m_fileHandle==-1)
	openConnection();

    LOG_INFO("Writing: "<<i_integer);
    write(m_fileHandle,(char*)&i_integer,1);

    if(m_closeHandleAfterMessage)
	closeConnection();
}


void ArduinoSerialConnection::serialWrite(char* i_pointer,const int& i_numberOfWrites)
{
    write(m_fileHandle,i_pointer,i_numberOfWrites);
    i_pointer+=i_numberOfWrites;

    if(m_closeHandleAfterMessage)
	closeConnection();
}


void ArduinoSerialConnection::serialWrite(std::vector<int>::iterator& io_iterator,
					  const int& i_numberOfWrites)
{
    LOG_INFO("Number of writes: "<<i_numberOfWrites);
    char* buffer = new char[i_numberOfWrites];
    
    for(int i=0;
	i<i_numberOfWrites;
	i++)
	{
	    buffer[i]=static_cast<char>(*io_iterator);
	    io_iterator++;
	}
    write(m_fileHandle,
	  buffer,
	  i_numberOfWrites);

    if(m_closeHandleAfterMessage)
	closeConnection();
    delete[] buffer;
}


std::string ArduinoSerialConnection::serialRead(const int& i_numberOfBytes)
{
    if(m_fileHandle==-1)
	openConnection();
   
    char* byte_in= new char[i_numberOfBytes+1];
    memset(byte_in,i_numberOfBytes+1,sizeof(char));
    char* currentByte=byte_in;
    //    int outputByte;
    int bytes_read;
    uint numberOfFails=0;
    
    //    std::cout<<"start sending!"<<std::endl;
    for (int i = 0; i < i_numberOfBytes; i++)
	{
	     /* Reads ttyO port, stores data into byte_in. */
	    bytes_read = read(m_fileHandle,currentByte,1);
	    
	    if(bytes_read==-1)
		{
		    i--;
		    numberOfFails++;
		    //block the processor for a bit to with for arduino sending
		    usleep(10);
		    //   std::cout<<"Couldnt read...someone was not ready yet...."<<std::endl;
		}
	    else
		{
		    numberOfFails=0;
		    LOG_INFO("read: "<< (int)*currentByte<<" ");
		    currentByte++;
				    
		}


	    if(numberOfFails>1000)
		{
		    LOG_ERROR("Failed to read out the bit!");
		    numberOfFails=0;
		    currentByte++;
		    continue;
		}
	}
    byte_in[i_numberOfBytes]=0;

    std::string output(byte_in);
    delete[] byte_in;

    if(m_closeHandleAfterMessage)
	closeConnection();

    return output;
}
