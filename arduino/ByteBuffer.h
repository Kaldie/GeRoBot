#ifndef ByteBuffer_h
#define ByteBuffer_h

/*
  Simple circular templated buffer
*/


/*#if defined(ARDUINO) && ARDUINO < 100
#include "WProgram.h"
#else
//#include "Arduino.h"
#endif //arduino header include
*/

template <typename T>
class ByteBuffer {
 private:
  static const int DEFAULT_SIZE = 10;
  T* m_readPosition;
  T* m_writePosition;
  T* m_buffer;
  T* m_endOfBuffer;

  void incrementPosition(T*& i_position);


 public:
  // Constructors
  ByteBuffer();
  explicit ByteBuffer(const int& i_size);

  // Deconstructor
  ~ByteBuffer();

  T* getReadPointer();
  bool finishReadPointer();

  T* getWritePointer();
  bool finishWritePointer();

  const bool isFull();
  const bool isEmpty();
};

// Constructors
template <typename T>
ByteBuffer<T>::ByteBuffer()
: ByteBuffer(DEFAULT_SIZE)
{}


template <typename T>
ByteBuffer<T>::ByteBuffer(const int& i_size) {
  m_buffer = (T*)malloc(sizeof(T) * i_size);
  memset(m_buffer, 0, sizeof(T) * i_size);
  m_endOfBuffer = m_buffer + i_size;
  m_readPosition = m_buffer;
  m_writePosition = m_buffer + 1;
}

// Deconstructor
template <typename T>
ByteBuffer<T>::~ByteBuffer() {
    free(m_buffer);
}

template <typename T>
void ByteBuffer<T>::incrementPosition(T*& i_position) {
  i_position++;
  if (m_endOfBuffer == i_position)
    i_position = m_buffer;
}


// Actual methods
template <typename T>
const bool ByteBuffer<T>::isFull() {
  T* testPointer =  m_writePosition;
  incrementPosition(testPointer);
  return testPointer == m_readPosition;
}


template <typename T>
const bool ByteBuffer<T>::isEmpty() {
  T* testPointer =  m_readPosition;
  incrementPosition(testPointer);
  return testPointer == m_writePosition;
}


template<typename T>
T* ByteBuffer<T>::getReadPointer() {
  return m_readPosition;
}


template<typename T>
bool ByteBuffer<T>::finishReadPointer() {
  if (isEmpty()) {
    return false;
  } else {
    incrementPosition(m_readPosition);
    return true;
  }
}


template<typename T>
T* ByteBuffer<T>::getWritePointer() {
  return m_writePosition;
}


template<typename T>
bool ByteBuffer<T>::finishWritePointer() {
  if (isFull()) {
    return false;
  } else {
    incrementPosition(m_writePosition);
    return true;
  }
}

#endif  // ARDUINOSKETCH_BYTEBUFFER_H_

