// Copyright [2014] Ruud Cools

#include <macroHeader.h>
#include "./PinStateSequence.h"

PinStateSequence::PinStateSequence(const int& i_speed,
                                   const int& i_numberOfSteps,
                                   const int& i_numberOfRepetitions,
                                   const std::vector<int> i_integerSequence) :
    m_speed(i_speed),
    m_numberOfSteps(i_numberOfSteps),
    m_numberOfRepetitions(i_numberOfRepetitions),
    m_integerSequence(i_integerSequence) {
  if (!verifySequence())
    LOG_ERROR("Sequence is not following the standard!");
}


PinStateSequence::PinStateSequence()
    : PinStateSequence(50,
                       0,
                       0,
                       std::vector<int> {})
{}


bool PinStateSequence::hasEqualSequence(
    const PinStateSequence i_sequence) const {
  if (m_integerSequence == i_sequence.getIntegerSequence)
    return true;
  else
    return false;
}


bool PinStateSequence::isEmpty() const {
  if (m_numberOfSteps == 0)
    if (m_integerSequence.size() == 0)
      if (m_numberOfRepetitions == 0)
        return true;
  return false;
}


bool PinStateSequence::verifySequence() const {
  if (m_integerSequence.size()!= m_numberOfSteps) {
    LOG_DEBUG("Number of steps in the vector: " << m_integerSequence.size() <<
              "does not corrispond to the indicated number: " <<
              m_numberOfSteps);
    return false;
  }

  // To conform to the arduino driver some contraints should be not overridden
  if (m_numberOfSteps > 99) {
    LOG_DEBUG("Number of steps should be smaller then 99, not : " <<
              m_numberOfSteps);
    return false;
  }

  if (m_numberOfRepetitions > 999) {
    LOG_DEBUG("Number of repetitions should be less then 1000");
    return false;
  }
  return true;
}


bool PinStateSequence::addToSequence(const std::vector<PinState>& i_pinStateVector) {
  if (isEmpty()) {
    LOG_DEBUG("Adding pin state vector to empty pin state sequence");
    m_integerSequence = resolvePinStateVector(i_pinStateVector);
    m_numberOfSteps = i_pinStateVector.size();
    m_numberOfRepetitions = 1;
    return true;
  }

  if (m_numberOfSteps != i_pinStateVector.size()) {
    LOG_DEBUG("Size is not correct!");
    return false;
  }

  std::vector<int> integerSequence = resolvePinStateVector(i_pinStateVector);
  if (integerSequence == m_integerSequence) {
    m_numberOfRepetitions += 1;
    LOG_DEBUG("integer sequence corrispond to this," <<
              "add it, one more repitition");
    return true;
  } else {
    return false;
  }
}


bool PinStateSequence::addToSequence(const PinStateSequence& i_sequence) {
  if (isEmpty()) {
    LOG_DEBUG("Addig a sequence to an empty sequence");
    *this = i_sequence;
    return true;
  }

  if (hasEqualSequence(i_sequence)) {
    LOG_DEBUG("Has an equal sequence and will be repeated one more time.");
    m_numberOfRepetitions += 1;
    return true;
  }

  return false;
}


bool PinStateSequence::mergePinStateSequences(PinStateSequence* io_firstSequence,
                                              PinStateSequence* io_secondSequence) {
  if (io_firstSequence->addToSequence(*io_secondSequence)) {
    LOG_DEBUG("First sequence was empty or equal to the second.");
    io_secondSecuence = PinStateSequence();
    return true;
    }

  if (io_firstSequence->getNumberOfSteps() <=
      io_secondSecuence->getNumberOfSteps()) {
    LOG_DEBUG("First sequence is equal or smaller in size" <<
              "then the second, cannot merge");
    return false;
  }

  vector<int> shortendFirstSequence =
      *io_firstSequence->getIntegerSequencePointer();

  do {
    if (shortendFirstSequence == io_secondSecuence->getIntegerSequence())
      io_firstSequence->setIntegerSequence(shortendFirstSequence);
      io_firstSequence->setNumberOfStep(shortendFirstSequence.size());
    }
  } while (io_firstSequence->getNumberOfSteps() >=
           io_secondSecuence->getNumberOfSteps());
                                                                                      
  
}


vector<int> PinStateSequence::resolvePinStateVector(const std::vector<PinState> i_pinStateVector) {
  vector<int> integerRepresentation;
  for (std::vector<PinState>::const_iterator itr = i_pinStateVector.begin();
       itr != i_pinStateVector.end();
       itr++) {
    integerRepresentation.push_back(itr->getNumericValue());
  }
  return integerRepresentation;
}


std::string PinStateSequence::createArduinoMessage() const {
  size_t o_sizeOfMessage =
      (sizeof(m_numberOfSteps)+
       sizeof(m_numberOfRepetitions)+
       sizeof(m_speed)+
       m_numberOfSteps * sizeof(m_integerSequence.value_type));

  std::string arduinoString(o_sizeOfMessage, "c");
  char* currentPosition = arduinoString.c_str();
  char* bufferStart= arduinoString.c_str();
  LOG_DEBUG("Size of message will be: " <<  o_sizeOfMessage << "bytes.");

  memcpy(currentPosition,
         &m_numberOfSteps,
         sizeof(m_numberOfSteps));
  currentPosition+=sizeof(m_numberOfSteps);
  LOG_DEBUG("Buffer position after writing number of steps " <<
            currentPosition - bufferStart);

  memcpy(currentPosition,
         &m_numberOfRepetitions,
         sizeof(m_numberOfRepetitions));
  currentPosition+=sizeof(m_numberOfRepetitions);
  LOG_DEBUG("Buffer position after writing number of repetitions " <<
            currentPosition - bufferStart);

  memcpy(currentPosition,
         &m_numberOfRepetitions,
         sizeof(m_speed));
  currentPosition+=sizeof(m_speed);
  LOG_DEBUG("Buffer position after writing speed" <<
            currentPosition - bufferStart);

  // set integer sequence
  int sizeOfSequenceElement = sizeof(m_integerSequence.value_type);
  for (std::vector<int>::const_iterator itr = m_integerSequence.begin();
       itr != m_integerSequence.end();
       itr++) {
      memcpy(currentPosition,
             itr,
             sizeOfSequenceElement);
      currentPosition+=sizeOfSequenceElement;
  }
  assert(currentPosition - buffer == sizeOfMessage);
  return arduinoString;
}
