// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include "./ArduinoSerialConnection.h"

ArduinoSerialConnection::ArduinoSerialConnection()
    : m_deadTimeBetweenMessage(0),
      m_minimumBytePerMessage(0),
      m_closeHandleAfterMessage(false),
      m_blockThread(false),
      m_fileHandle(-1),
      m_portName(""),
      m_baudRate(B115200)
{}


ArduinoSerialConnection::ArduinoSerialConnection(std::string i_fileName)
    : m_deadTimeBetweenMessage(0),
      m_minimumBytePerMessage(0),
      m_closeHandleAfterMessage(false),
      m_blockThread(false),
      m_fileHandle(-1),
      m_portName(i_fileName),
      m_baudRate(B115200)
{}



ArduinoSerialConnection::ArduinoSerialConnection(
    const std::string& i_portName,
    const int& i_minimumBytePerMessage,
    const int& i_deadTimeBetweenMessage,
    const int& i_baudRate,
    const bool& i_closeAfterMessage,
    const bool& i_blockThread)
    : m_deadTimeBetweenMessage(i_deadTimeBetweenMessage),
      m_minimumBytePerMessage(i_minimumBytePerMessage),
      m_closeHandleAfterMessage(i_closeAfterMessage),
      m_blockThread(i_blockThread),
      m_fileHandle(-1),
      m_portName(i_portName),
      m_baudRate(B115200) {

  setBaudRate(i_baudRate);
}


void ArduinoSerialConnection::setBaudRate(const int& i_baudRate) {
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

bool ArduinoSerialConnection::hasConnection() {
  if (m_fileHandle != -1)
    return true;
  else
    return false;
}


void ArduinoSerialConnection::resetConnection() {
    closeConnection();
    openConnection();
}


void ArduinoSerialConnection::openConnection() {
  LOG_DEBUG("Opening serial connection!!");
  setPortName(resolvePortName());
  struct termios tio;
  // make sure everthing is set to 0
  memset(&tio, 0, sizeof(tio));

  // 8N1
  tio.c_cflag &= ~PARENB;  // No parity detection
  tio.c_cflag &= ~CSTOPB;  // 1 stop bit
  tio.c_cflag &= ~CSIZE;  //  apply mask before setting the size of the byte
  tio.c_cflag &= ~CRTSCTS;  // no flow control
  tio.c_cflag =  CS8|CREAD|CLOCAL;  // 8n1, see termios.h for more information

  tio.c_cc[VMIN] = m_minimumBytePerMessage;
  tio.c_cc[VTIME] = m_deadTimeBetweenMessage;

  tio.c_iflag &= ~(IXON | IXOFF | IXANY);  // turn off s/w flow ctrl

  tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // make raw
  tio.c_oflag &= ~OPOST;  // make raw

  cfsetospeed(&tio, m_baudRate);  // 115200 baud
  cfsetispeed(&tio, m_baudRate);  // 115200 baud

  if (m_blockThread)
    m_fileHandle = open(getPortName().c_str(), O_RDWR | O_NOCTTY);
  else
    m_fileHandle = open(getPortName().c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

  LOG_INFO("Opening file to port: '" << getPortName() << "'");

  if (m_fileHandle == -1) {
    LOG_ERROR("Unable to open port'" << getPortName() << "'");
    throw 100;
  }

  if (tcsetattr(m_fileHandle, TCSAFLUSH, &tio)) {
    LOG_ERROR("Couldn't set term attributes");
    throw 100;
  }
  tcflush(m_fileHandle, TCIOFLUSH);
}


void ArduinoSerialConnection::serialWrite(std::string i_string) {
    if (m_fileHandle == -1)
      openConnection();

    LOG_INFO("Writing: " << i_string);
    write(m_fileHandle, i_string.c_str(), i_string.length());

    if (m_closeHandleAfterMessage)
      closeConnection();
}

void ArduinoSerialConnection::serialWrite(const int& i_integer) {
  if (m_fileHandle == -1)
    openConnection();

  LOG_INFO("Writing: " << i_integer);
  write(m_fileHandle, (unsigned char*)(&i_integer), 1);

  if (m_closeHandleAfterMessage)
    closeConnection();
}


void ArduinoSerialConnection::serialWrite(const unsigned char* i_pointer,
                                          const int& i_numberOfWrites) {
  if (m_fileHandle == -1)
    openConnection();
  if (i_numberOfWrites == sizeof(int)) {
    int x = *(int*)(i_pointer);
    LOG_DEBUG("Writing: "<< x << " as integer");
  } else if (i_numberOfWrites == sizeof(char))
    LOG_DEBUG("Writing: "<< *i_pointer);

  write(m_fileHandle, i_pointer, i_numberOfWrites);

  if (m_closeHandleAfterMessage)
    closeConnection();
}


void ArduinoSerialConnection::serialWrite(std::vector<int>::iterator& io_iterator,
                                          const int& i_numberOfWrites) {
  LOG_INFO("Number of writes: " << i_numberOfWrites);
  char* buffer = new char[i_numberOfWrites];

  for (int i = 0;
       i < i_numberOfWrites;
       i++) {
    buffer[i]=static_cast<char>(*io_iterator);
    io_iterator++;
  }
  write(m_fileHandle,
        buffer,
        i_numberOfWrites);

  if (m_closeHandleAfterMessage)
    closeConnection();
  delete[] buffer;
}


std::string ArduinoSerialConnection::serialReadString() {
  // Read string will use a none-blocking thread
  // to read the data from the serial connection
  if (m_fileHandle == -1)
    openConnection();

  //  m_blockThread = false;
  //  m_closeHandleAfterMessage = false;
  
  //  openConnection();
  int arrayLength(2);
  char* result = new char[arrayLength];
  std::stringstream stringStream;
  int numberOfBytes;
  //  LOG_DEBUG("Start reading string from serial connection");
  do {
    memset(result, '\0', sizeof(*result) * arrayLength);
    try {
      numberOfBytes  = rawSerialRead(arrayLength-1,
                                     reinterpret_cast<unsigned char*>(result));
    } catch(const std::runtime_error& e) {
      LOG_DEBUG("No string read from serial connection!");
      break;
    }

    for (int i = 0;
         i < numberOfBytes;
         i++) {
      if (result[i] != 13 and result[i] != 10) {
        stringStream.put(result[i]);
      }
    }
  } while (result[numberOfBytes-1] != 10);
  
  delete[] result;
  return stringStream.str();
}


const int ArduinoSerialConnection::
rawSerialRead(const int& i_numberOfBytes,
              unsigned char* buffer) {
  int numberOfFails = 0;
  int bytes_read = 0;
  int currentRead = 0;
  if (m_fileHandle == -1)
    openConnection();

  for (int i = 0; i < i_numberOfBytes; i++) {
    /* Reads ttyO port, stores data into byte_in. */
    currentRead = read(m_fileHandle, reinterpret_cast<void*>(buffer + i),
                       1);
    if (currentRead == -1) {
      i--;
      numberOfFails++;
      //      LOG_DEBUG("FAILED to read the bit, will try again!");
      // block the processor for a bit to with for arduino sending
    } else {
      bytes_read += currentRead;
      numberOfFails = 0;
      //   LOG_INFO("read (as int): "<< static_cast<int>(buffer[i]) << " ");
    }

    if (numberOfFails > 100000) {
      LOG_ERROR("Failed to read out the bit!");
    }
  }

  if (m_closeHandleAfterMessage) {
    closeConnection();
  }
  return bytes_read;
}


int ArduinoSerialConnection::serialRead(const int& i_numberOfBytes) {
  int output;
  if (i_numberOfBytes > static_cast<int>(sizeof(output))) {
    LOG_ERROR("Size should smaller or equal to int size");
  }
  unsigned char* result = new unsigned char[i_numberOfBytes];
  int numberOfBytes = rawSerialRead(i_numberOfBytes, result);
  for (int i = 0;
       i < numberOfBytes;
       i++) {
    output = result[i] << (i*4);
  }
  delete[] result;
  return output;
}

bool ArduinoSerialConnection::flushConnection() {
  int hasFlushed = tcflush(m_fileHandle, TCIOFLUSH);
  if (!hasFlushed)
    return true;
  else
    return false;
}


std::string ArduinoSerialConnection::resolvePortName() const {
    glob_t globbuf;
    glob(getPortName().c_str(), 0, NULL, &globbuf);
    if (globbuf.gl_pathc == 0) {
      LOG_ERROR("Could not find any devices using: '" <<
                getPortName() << "' as expression!");
    } else if (globbuf.gl_pathc > 1) {
      LOG_ERROR("Multiple paths found, please refine regular expression: "
                <<getPortName());
    }

    std::string thisPath = globbuf.gl_pathv[0] ;
    globfree(&globbuf);
    return thisPath;
}
