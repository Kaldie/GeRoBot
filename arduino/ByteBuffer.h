#ifndef ByteBuffer_h
#define ByteBuffer_h

/*
  Simple circular templated buffer
*/
template <typename T>
class ByteBuffer {
 private:
  T* m_readPosition;
  T* m_writePosition;
  T* m_buffer;
  T* m_endOfBuffer;
  int m_itemCount;
  void incrementPosition(T*& i_position);
  ByteBuffer();


 public:
  // Constructors
  explicit ByteBuffer(const int& i_size);

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
template <typename T>
ByteBuffer<T>::ByteBuffer(const int& i_size) {
  m_buffer = (T*)malloc(sizeof(T) * i_size);
  memset(m_buffer, 0, sizeof(T) * i_size);
  m_endOfBuffer = m_buffer + i_size;
  m_readPosition = m_buffer;
  m_writePosition = m_buffer;
  m_itemCount = 0;
}

// Deconstructor
template <typename T>
ByteBuffer<T>::~ByteBuffer() {
    free(m_buffer);
}

template <typename T>
void ByteBuffer<T>::incrementPosition(T*& i_position) {
  i_position++;
  if (i_position >= m_endOfBuffer)
    i_position = m_buffer;
}


template <typename T>
bool ByteBuffer<T>::isFull() {
  return (m_endOfBuffer - m_buffer) <= m_itemCount;
}


template <typename T>
bool ByteBuffer<T>::isEmpty() {
  return m_itemCount == 0;
}


template <typename T>
int ByteBuffer<T>::emptyElements() {
  return (m_endOfBuffer - m_buffer) - m_itemCount;
}


template <typename T>
int ByteBuffer<T>::inlineWriteElements() {
  if (isFull()) {
    return 0;
  }
  return m_endOfBuffer - m_writePosition;
}


template <typename T>
int ByteBuffer<T>::inlineReadElements() {
  if (isEmpty()) {
    return 0;
  }

  if (m_writePosition <= m_readPosition)
    return m_endOfBuffer - m_readPosition;
  else
    return m_writePosition - m_readPosition;
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
    --m_itemCount;
    return true;
  }
}


template<typename T>
bool  ByteBuffer<T>::finishReadPointers(const int& i_finishes) {
  bool result = true;
  for (int i = 0;
       i < i_finishes && result;
       ++i) {
    result &= finishReadPointer();
  }
  return result;
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
    ++m_itemCount;
    incrementPosition(m_writePosition);
    return true;
  }
}


template<typename T>
bool  ByteBuffer<T>::finishWritePointers(const int& i_finishes) {
  bool result = true;
  for (int i = 0;
       i < i_finishes && result;
       ++i) {
    result &= finishWritePointer();
  }
  return result;
}
#endif  // ARDUINOSKETCH_BYTEBUFFER_H_
