#ifndef ByteBuffer_h
#define ByteBuffer_h

/*
  Simple circular templated buffer
*/
template <typename T, int N>
class ByteBuffer {
 private:
  T* m_readPosition;
  T* m_writePosition;
  T m_buffer [N];
  T* m_endOfBuffer;
  int m_itemCount;
  void incrementPosition(T*& i_position);



 public:
  ByteBuffer();
  // Deconstructor
  ~ByteBuffer();
  /// Get the element that is currently available to be read
  T* getReadPointer();
  /// Let the buffer known that the current read element is ready to be re-written
  bool finishReadPointer();
  /**
   * Let the buffer know that several elements are ready to be written
   * @param[in] i_finishes Number of elements that are ready to be written
   */
  bool finishReadPointers(const int& i_finishes);
  /// Get the element that is currently available to be written
  T* getWritePointer();
  /// Let the buffer know that the current written element is ready to be read
  bool finishWritePointer();
  /**
   * Let the buffer know that several elements are ready to be read
   * @param[in] i_finishes Number of elements that are ready
   */
  bool finishWritePointers(const int& i_finishes);
  /// returns true if no elements are currently ready to be written
  bool isFull();
  /// returns true if no elements are currently ready to be read
  bool isEmpty();
  /// return the number of elements that are empty
  int emptyElements();
  /// return the number of elements that could be written continiusly
  int inlineWriteElements();
  /// return the number of elements that could be read continiusly
  int inlineReadElements();
};


// Constructors
template<typename T, int N>
  ByteBuffer<T, N>::ByteBuffer() {
  //  m_buffer = (T*)malloc(sizeof(T) * N);
  memset(m_buffer, 0, sizeof(T) * N);
  m_endOfBuffer = m_buffer + N;
  m_readPosition = m_buffer;
  m_writePosition = m_buffer;
  m_itemCount = 0;
}

// Deconstructor
template<typename T, int N>
ByteBuffer<T, N>::~ByteBuffer() {
    free(m_buffer);
}

template<typename T, int N>
void ByteBuffer<T, N>::incrementPosition(T*& i_position) {
  i_position++;
  if (i_position >= m_endOfBuffer)
    i_position = m_buffer;
}


template<typename T, int N>
bool ByteBuffer<T, N>::isFull() {
  return (m_endOfBuffer - m_buffer) <= m_itemCount;
}


template<typename T, int N>
bool ByteBuffer<T, N>::isEmpty() {
  return m_itemCount == 0;
}


template<typename T, int N>
int ByteBuffer<T, N>::emptyElements() {
  return (m_endOfBuffer - m_buffer) - m_itemCount;
}


template<typename T, int N>
int ByteBuffer<T, N>::inlineWriteElements() {
  if (isFull()) {
    return 0;
  }
  if (m_writePosition < m_readPosition) {
    return m_readPosition - m_writePosition;
  } else {
    return m_endOfBuffer - m_writePosition;
  }
}


template<typename T, int N>
int ByteBuffer<T, N>::inlineReadElements() {
  if (isEmpty()) {
    return 0;
  }

  if (m_writePosition <= m_readPosition)
    return m_endOfBuffer - m_readPosition;
  else
    return m_writePosition - m_readPosition;
}


template<typename T, int N>
T* ByteBuffer<T, N>::getReadPointer() {
  return m_readPosition;
}


template<typename T, int N>
bool ByteBuffer<T, N>::finishReadPointer() {
  if (isEmpty()) {
    return false;
  } else {
    incrementPosition(m_readPosition);
    --m_itemCount;
    return true;
  }
}


template<typename T, int N>
bool  ByteBuffer<T, N>::finishReadPointers(const int& i_finishes) {
  bool result = true;
  for (int i = 0;
       i < i_finishes;
       ++i) {
    result &= finishReadPointer();
  }
  return result;
}


template<typename T, int N>
T* ByteBuffer<T, N>::getWritePointer() {
  return m_writePosition;
}


template<typename T, int N>
bool ByteBuffer<T, N>::finishWritePointer() {
  if (isFull()) {
    return false;
  } else {
    ++m_itemCount;
    incrementPosition(m_writePosition);
    return true;
  }
}


template<typename T, int N>
bool  ByteBuffer<T, N>::finishWritePointers(const int& i_finishes) {
  bool result = true;
  for (int i = 0;
       i < i_finishes;
       ++i) {
    result &= finishWritePointer();
  }
  return result;
}
#endif  // ARDUINOSKETCH_BYTEBUFFER_H_
