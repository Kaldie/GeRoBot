// Copyright [2014] Ruud Cools

#include <macroHeader.h>
#include "./PinStateSequence.h"
#include "./PinState.h"

PinStateSequence::PinStateSequence()
    : PinStateSequence(50,
                       0,
                       {})
{}


PinStateSequence::PinStateSequence(const int& i_speed,
                                   const int& i_numberOfRepetitions,
                                   const PinStateVector& i_pinStateVector)
    : m_speed(i_speed),
      m_numberOfRepetitions(i_numberOfRepetitions),
      m_pinStateVector(i_pinStateVector) {
  validate(i_pinStateVector);
}


void PinStateSequence::validate() const {
  return validate(m_pinStateVector);
}


void PinStateSequence::validate(
    const PinStateVector& i_pinStateVector) const {

  if (i_pinStateVector.size() <= 0) {
    return;
  } else {
    LOG_DEBUG("Pin state vector size is: "<< i_pinStateVector.size());
  }

  PinVector pinVector = i_pinStateVector[0].getPinVector();
  std::vector<int>::const_iterator pinNumberItr = pinVector.begin();
  std::vector<int>::const_iterator pinNumberBegin = pinVector.begin();
  std::vector<int>::const_iterator pinNumberEnd = pinVector.end();

  for (auto itr = i_pinStateVector.begin();
       itr != i_pinStateVector.end();
       itr++) {
    if (pinVector.size() != itr->getPinVector().size()) {
      LOG_ERROR("Number of pins is not equal over all PinStates");
    }

    for (pinNumberItr = itr->getPinVector().begin();
         pinNumberItr != itr->getPinVector().end();
         pinNumberItr++)
      if (std::find(pinNumberBegin,
                    pinNumberEnd,
                    *pinNumberItr) == pinNumberEnd)
        LOG_ERROR("PinState does not define" <<
                  "the same pins over the whole sequence");
  }
}


std::vector<int> PinStateSequence::getIntegerSequence() const {
  return getIntegerSequence(m_pinStateVector);
}


std::vector<int> PinStateSequence::getIntegerSequence(
    const PinStateVector i_pinStateVector) const  {

  std::vector<int> integerSequence;
  for (PinStateVector::const_iterator itr = i_pinStateVector.begin();
      itr != i_pinStateVector.end();
      itr++) {
    integerSequence.push_back(itr->getNumericValue());
  }
  return integerSequence;
}


bool PinStateSequence::hasEqualSequence(
    const PinStateSequence& i_sequence) const {

  if (this->getIntegerSequence() == i_sequence.getIntegerSequence())
    return true;
  else
    return false;
}


bool PinStateSequence::isEmpty() const {
  if (m_numberOfRepetitions == 0 && m_pinStateVector.size() == 0)  {
    return true;
  } else {
    return false;
  }
}


bool PinStateSequence::hasMutualPins(
    const PinStateSequence& i_pinStateSequence) const {
  return hasMutualPins(i_pinStateSequence.getPinStateVector()[0]);
}


bool PinStateSequence::hasMutualPins(const PinState& i_pinState) const {
  // If the pin state vector is not yet set, then always no mutual pins!
  if (m_pinStateVector.size() == 0)
      return false;
  
  auto start = m_pinStateVector[0].getPinVector().begin();
  auto end = m_pinStateVector[0].getPinVector().end();

  PinVector firstPinVector = i_pinState.getPinVector();

  for (auto itr = firstPinVector.begin();
       itr != firstPinVector.end();
       itr++) {
    if (std::find(start, end, *itr) != end)
      return true;
  }
  return false;
}


bool PinStateSequence::addToSequence(
    const PinState& i_pinState) {

  if (m_numberOfRepetitions > 1) {
    LOG_DEBUG("Number of repetitions is more then 1 thus this will not fly");
    return false;
  }

  if (m_pinStateVector.size() >1) {
    LOG_DEBUG("Number of states in the vector is more then 1. Will not fly!");
    return false;
  }

  if (hasMutualPins(i_pinState)) {
    LOG_DEBUG("The added pin states has mutual " <<
              "pins to the allready defined ones!");
    return false;
  }

  if (m_pinStateVector.size() == 1)
    m_pinStateVector[0].update(i_pinState);
  else
    m_pinStateVector.push_back(i_pinState);

  m_numberOfRepetitions = 1;
  return true;
}


bool PinStateSequence::addToSequence(
    const PinStateVector& i_pinStateVector) {

  if (isEmpty()) {
    LOG_DEBUG("Adding pin state vector to empty pin state sequence");
    //    validate(i_pinStateVector);
    m_pinStateVector.insert(m_pinStateVector.end(),
                            i_pinStateVector.begin(),
                            i_pinStateVector.end());
    m_numberOfRepetitions = 1;
    return true;
  }

  if (m_pinStateVector.size() != i_pinStateVector.size()) {
    LOG_DEBUG("Size is not correct!");
    return false;
  }

  if (getIntegerSequence(m_pinStateVector) ==
      getIntegerSequence(i_pinStateVector)) {
    m_numberOfRepetitions += 1;
    LOG_DEBUG("integer sequence corrispond to this," <<
              "add it, one more repitition");
    return true;
  } else {
    LOG_DEBUG("integer sequence does not corrispond to this!");
    return false;
  }
}


bool PinStateSequence::addToSequence(const PinStateSequence& i_sequence) {
  if (i_sequence.isEmpty()) {
    LOG_DEBUG("Nothing to add, sequence is empty");
    return false;
  }

  if (isEmpty()) {
    LOG_DEBUG("Addig a sequence to an empty sequence");
    *this = i_sequence;
    return true;
  }

  if (hasEqualSequence(i_sequence)) {
    LOG_DEBUG("Has an equal sequence and will be repeated  more times.");
    m_numberOfRepetitions += i_sequence.m_numberOfRepetitions;
    return true;
  }
  
  if (hasMutualPins(i_sequence)) {
    LOG_DEBUG("Could not add sequence, has mutual pins");
    return false;
  }

  if (m_numberOfRepetitions != i_sequence.getNumberOfRepetitions()) {
    LOG_DEBUG("Could not add sequence, number of repetitions is not equal: " <<
              m_numberOfRepetitions << "!=" << i_sequence.getNumberOfRepetitions());
    return false;
  }
  
  auto rhsItr = i_sequence.getPinStateVector().begin();
  auto ltsItr = m_pinStateVector.begin();
  for (;
       ltsItr != m_pinStateVector.end();
       ltsItr++, rhsItr++) {
    LOG_DEBUG("Updateing the pin state vector");
    ltsItr->update(*rhsItr);
  }
  return true;
}

bool PinStateSequence::mergePinStateSequence(
    PinStateSequence* io_sequence) {
  return PinStateSequence::mergePinStateSequences(this,
                                                   io_sequence);
}


bool PinStateSequence::mergePinStateSequences(
    PinStateSequence* io_firstSequence,
    PinStateSequence* io_secondSequence) {

  // check if we can simple add them
  if (io_firstSequence->addToSequence(*io_secondSequence)) {
    LOG_DEBUG("First sequence was empty or equal to the second " <<
              "or the pins were not mutal and the number of steps were.");
    *io_secondSequence = PinStateSequence();
    return true;
  }

  // check if both sequence do not define the same pins
  if (io_firstSequence->hasMutualPins(*io_secondSequence)) {
    LOG_DEBUG("The first sequence has the same pins " <<
              "as the second therefor cannot be merged");
    return false;
  }

  // check if the first sequence has more repetitions then the second
  if (io_firstSequence->getNumberOfRepetitions()<
      io_secondSequence->getNumberOfRepetitions()) {
      LOG_DEBUG("The second sequence has more repetitions then the first" <<
                "Using recursive call to fix it");
      return PinStateSequence::mergePinStateSequences(io_secondSequence,
                                                      io_firstSequence);
  }

  // Merge them!
  LOG_DEBUG("Now we are sure we can merge them!");
  int newFirstSequenceRepetitions =
      io_firstSequence->getNumberOfRepetitions() -
      io_secondSequence->getNumberOfRepetitions();

  if (newFirstSequenceRepetitions <= 0)
    LOG_ERROR("@ this point this could not happen!!");

  io_firstSequence->setNumberOfRepetitions(
      io_secondSequence->getNumberOfRepetitions());

  io_secondSequence->addToSequence(*io_firstSequence);
  io_firstSequence->setNumberOfRepetitions(newFirstSequenceRepetitions);
  return true;
}


bool PinStateSequence::
  setStateForSequence(const PinState& i_pinState,
                      const bool& i_extend /*= false*/,
                      const bool& i_overrideSequence /*=false*/) {
  bool overrideSequence = i_overrideSequence;
  bool extendSequence = i_extend;
  PinState latestPinState = i_pinState;
  
  if (i_extend and i_overrideSequence)
    LOG_ERROR("Cannot extend and override at the same time!");

  if (!i_extend and !i_overrideSequence) {
    if (hasMutualPins(i_pinState)) {
      LOG_DEBUG("Extend or overide was not selected " <<
                "and has mutual pins," <<
                "could not set the state for the sequence!");
      return false;
    } else {
      overrideSequence = true;
      extendSequence = false;
    }
  }

  LOG_DEBUG("Extend: "<< extendSequence << " Override: "<< overrideSequence);
  for (auto pinStateIterator = m_pinStateVector.begin();
       pinStateIterator != m_pinStateVector.end();
       pinStateIterator++) {
    if (overrideSequence) {
      pinStateIterator->update(i_pinState);
    }

    if (i_extend) {
      for (auto pinIterator = latestPinState.getPinVector().begin();
           pinIterator != latestPinState.getPinVector().end();
           pinIterator++) {
        try {
          int pinValue = pinStateIterator->getPinState(*pinIterator);
          LOG_DEBUG("Found: "<< *pinIterator);
          latestPinState.update(*pinIterator, pinValue);
        }
        catch(std::runtime_error) {
          LOG_DEBUG("Not Found: "<< *pinIterator <<
                    "Set: " << latestPinState.getPinState(*pinIterator));
          pinStateIterator->update(*pinIterator,
                                   latestPinState.getPinState(*pinIterator));
        }
      }
    }
  }
  return true;
}


/*
  Could be deleted, dont know yet....
  bool PinStateSequence::
  setStateForSequence(const PinStateVector& i_vector,
                      const bool i_extend /*=false,
                      const bool i_overrideSequence /* =false) {
  if (i_vector.size() != m_pinStateVector.size()) {
    LOG_DEBUG("Could not set the pin state vector, it has the wrong size!");
    return false;
  }

  if (!i_overrideSequence) {
    for (auto pinStateIterator = i_vector.begin();
         pinStateIterator != i_vector.end();
         pinStateIterator++) {
      if (hasMutualPins(*pinStateIterator)) {
        return false;
      }
    }
  }
  
  auto inputPinStateIter = i_vector.begin();
  auto currentPinStateIter = m_pinStateVector.begin();

  for (;
       (inputPinStateIter != i_vector.end() and
        currentPinStateIter != m_pinStateVector.end());
       inputPinStateIter++, currentPinStateIter++) {
    currentPinStateIter->update(*inputPinStateIter);
  }
}
*/

void PinStateSequence::displaySequence() const {
  LOG_INFO("Speed : " << m_speed);
  LOG_INFO("Number of repititions: " << m_numberOfRepetitions);
  LOG_INFO("Number of steps: " << m_pinStateVector.size());

  for (auto itr = m_pinStateVector.begin();
       itr != m_pinStateVector.end();
       itr++) {
    itr->displayPinState();
  }
}


size_t PinStateSequence::getSizeOfMessage() const {
  return (sizeof(m_numberOfRepetitions) +
          sizeof(m_speed) +
          m_pinStateVector.size() * sizeof(std::vector<int>::value_type));
}


void PinStateSequence::createArduinoBuffer(char*& o_buffer,
                                           size_t& o_sizeOfMessage) const {
  // o_buffer will be a byte array which needs to be send to the arduino
  // The fact is it will contain a shit load of 0's
  // Thus cannot use some conversion tricks
  // So raw dynamic memory handling.
  // The caller is responsible for deleting the memory!!
  o_sizeOfMessage = getSizeOfMessage();
  LOG_DEBUG("Size of message will be: " <<  o_sizeOfMessage << "bytes.");
  o_buffer = new char[o_sizeOfMessage+1];

  // someday i will print out this char array make sure stuff doesnt blow up
  o_buffer[o_sizeOfMessage] = '\0';
  char* currentPosition = o_buffer;

  writeIntegerToBuffer(m_numberOfRepetitions, currentPosition);
  LOG_DEBUG("Buffer position after writing number of repetitions " <<
            currentPosition - o_buffer);

  writeIntegerToBuffer(m_speed, currentPosition);
  LOG_DEBUG("Buffer position after writing speed" <<
            currentPosition - o_buffer);

  // set integer sequence
  for (auto itr = m_pinStateVector.begin();
       itr != m_pinStateVector.end();
       itr++) {
    writeIntegerToBuffer(itr->getNumericValue(),
                         currentPosition);
    LOG_DEBUG("Current string: " << o_buffer);
  }

  if ((currentPosition + 1) - o_buffer != static_cast<int>(o_sizeOfMessage))
    LOG_ERROR("Buffer is not correctly created!");
}


void PinStateSequence::writeIntegerToBuffer(const int& i_value,
                                            char*& i_bufferPosition) const {
  memcpy(i_bufferPosition,
         &(i_value),
         sizeof(i_value));
  i_bufferPosition+=sizeof(i_value);
}
