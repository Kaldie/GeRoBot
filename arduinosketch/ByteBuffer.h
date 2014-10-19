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
class ByteBuffer
{
 private:

    static const  int DEFAULT_SIZE=10;
     int m_readPosition;
     int m_writePosition;
     int m_availableStorage;
     int m_size;
    T* m_buffer;

    void incrementPosition( int&);


 public:

    //Constructors
    ByteBuffer();
    ByteBuffer(const int& i_size);
    
    //Deconstructor
    ~ByteBuffer();

    //Actual methods
    void setSize(const int&);

    const T get();
    void set(const T& i_value);
    void set(const T*,const int&);
    const  int getAvailableElements();
    const  int getAvailableStorage();
    
};

//Constructors
template <typename T>
ByteBuffer<T>::ByteBuffer():m_readPosition(0),m_writePosition(0),m_availableStorage(DEFAULT_SIZE),m_size(DEFAULT_SIZE)
{
    m_buffer = (T*)malloc(sizeof(T)*m_size);
}

    
template <typename T>
ByteBuffer<T>::ByteBuffer(const int& i_size):m_readPosition(0),m_writePosition(0),m_availableStorage(i_size),m_size(i_size)
{
    m_buffer = (T*)malloc(sizeof(T)*m_size);
}

//Deconstructor
template <typename T>
ByteBuffer<T>::~ByteBuffer()
{
    free(m_buffer);
}

template <typename T>
void ByteBuffer<T>::incrementPosition( int& i_position)
{
    if(i_position+1==m_size)
	i_position=0;
    else
	i_position++;
}


template <typename T>
void ByteBuffer<T>::setSize(const int& i_size)
{
    m_size=i_size;
    m_availableStorage=i_size;
    free(m_buffer);
    m_buffer = (T*)malloc(sizeof(T)*m_size);
}


//Actual methods
template <typename T>
const  int ByteBuffer<T>::getAvailableStorage()
{
    return m_availableStorage;
}


template <typename T>
const int ByteBuffer<T>::getAvailableElements()
{
    //    Serial.print("Size and storage: ");
    /*    Serial.print(m_size);
    Serial.print(", ");
    Serial.print(m_availableStorage);
    Serial.print(", ");
    Serial.println(m_size-m_availableStorage);
    */
    return (m_size-m_availableStorage);
}


template <typename T>
const T ByteBuffer<T>::get()
{
    //    int availableElements=getAvailableElements();
    if (getAvailableElements() > 0 )
	{
	    //	    Serial.print("Reading position: ");
	    //	    Serial.println(m_readPosition);
	    //	    Serial.print(" which is filled with: ");
	    
	    T output=(*(m_buffer+m_readPosition));
	    incrementPosition(m_readPosition);
	    m_availableStorage++;
	    //	    Serial.write(output);

	    //   Serial.print(m_readPosition);
	    //Serial.print(" ");
	    //	    Serial.println(m_writePosition);
	    //	    Serial.print("Available Elements: ");
	    //	    Serial.println(getAvailableElements());
	    return output;
	}
    else
	{
	    Serial.print("Available Elements: ");
	    Serial.println(getAvailableElements());
	    return T();
	}
}


template <typename T>
void ByteBuffer<T>::set(const T& i_value)
{
    if(getAvailableStorage()>0)
	{
	    /*
	      Serial.print("Writing: ");
	      Serial.print(i_value);
	      Serial.print(".  Writing @: ");
	      Serial.println(m_writePosition);
	    */
	    m_buffer[m_writePosition]=i_value;
	    //Serial.print("now in buffer: ");
	    //	    Serial.println(* (m_buffer+m_writePosition));
	    incrementPosition(m_writePosition);
	    m_availableStorage--;
	}

    //    Serial.print("AvailableStorage: ");
    //    Serial.println(getAvailableStorage());
    
}

template <typename T>
void ByteBuffer<T>::set(const T* i_valuePointer,const int& i_numberOfPoints)
{
    for(int i=0;
	i<i_numberOfPoints;
	i++)
	{
	    set(*(i_valuePointer+i));
	}
}


#endif // guard header

