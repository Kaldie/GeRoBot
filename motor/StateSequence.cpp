// Copyright [2014] Ruud Cools

#include <macroHeader.h>
#include "./StateSequence.h"
#include "./PinState.h"

StateSequence::StateSequence()
    : StateSequence(100, 0, {})
{}


StateSequence::StateSequence(const int& i_speed,
                             const int& i_numberOfRepetitions,
                             const PinStateVector& i_pinStateVector)
    : m_speed(i_speed),
      m_numberOfRepetitions(i_numberOfRepetitions),
      m_pinStateVector(i_pinStateVector) {
  validate(i_pinStateVector);
}


void StateSequence::validate() const {
  return validate(m_pinStateVector);
}


void StateSequence::validate(
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


std::vector<int> StateSequence::getIntegerSequence() const {
  return getIntegerSequence(m_pinStateVector);
}


std::vector<int> StateSequence::getIntegerSequence(
    const PinStateVector& i_pinStateVector) const  {
  std::vector<int> integerSequence;
  for (PinStateVector::const_iterator itr = i_pinStateVector.begin();
      itr != i_pinStateVector.end();
      itr++) {
    integerSequence.push_back(itr->getNumericValue());
  }
  return integerSequence;
}


bool StateSequence::hasEqualSequence(
    const StateSequence& i_sequence) const {
    if (this->getIntegerSequence() == i_sequence.getIntegerSequence())
    return true;
  else
    return false;
}


bool StateSequence::isEmpty() const {
  if (m_numberOfRepetitions == 0) {
    if (m_pinStateVector.size() == 0) {
      return true;
    }
  }
  return false;
}


bool StateSequence::hasMutualPins(
    const StateSequence& i_pinStateSequence) const {
  bool hasMutualPin = false;
  for (auto itr = i_pinStateSequence.getPinStateVector().begin();
       itr!= i_pinStateSequence.getPinStateVector().end();
       itr++) {
    hasMutualPin |= hasMutualPins(*itr);
  }
  return hasMutualPin;
}


bool StateSequence::hasMutualPins(const PinState& i_pinState) const {
  // If the pin state vector is not yet set, then always no mutual pins!
  if (m_pinStateVector.size() == 0)
      return false;
  
  auto start = m_pinStateVector.back().getPinVector().begin();
  auto end = m_pinStateVector.back().getPinVector().end();

  PinVector firstPinVector = i_pinState.getPinVector();

  for (auto itr = firstPinVector.begin();
       itr != firstPinVector.end();
       itr++) {
    if (std::find(start, end, *itr) != end)
      return true;
  }
  return false;
}


bool StateSequence::appendSequence(const StateSequence i_sequence) {
  // checking parameters!
  if (m_numberOfRepetitions > 1) {
    LOG_DEBUG("Coud not append them, number of repetitions is not 1!");
    return false;
  }

  if (m_numberOfRepetitions != i_sequence.getNumberOfRepetitions()) {
    LOG_DEBUG("Number of repetitions is not equal!");
    return false;
  }

  std::vector<PinState> pinStateVector = i_sequence.getPinStateVector();
  m_pinStateVector.insert(m_pinStateVector.end(),
                          pinStateVector.begin(),
                          pinStateVector.end());
  return true;
}


bool StateSequence::addToSequence(
    const PinState& i_pinState,
    const bool& i_forceAdd /*=false*/) {
  /* gentleAdd is used to make sure you don't make a mistake
     It will make sure if mutual pins are in it does not add
     However, it might be benificial to still add it.
   */
  if (m_numberOfRepetitions > 1) {
    LOG_DEBUG("Number of repetitions is more then 1 thus this will not fly");
    return false;
  }

  bool hasMutualPin = hasMutualPins(i_pinState);
  LOG_DEBUG("Has mutual pins: " << hasMutualPin);
  if (hasMutualPin && !i_forceAdd ) {
    LOG_DEBUG("The added pin states has mutual " <<
              "pins to the allready defined ones!");
    return false;
  }

  if (m_pinStateVector.size() == 0) {
    LOG_DEBUG("Pushing back the first state!");
    m_pinStateVector.push_back(i_pinState);
  } else {
    if (hasMutualPin) {
      LOG_DEBUG("Adding another pin state!");
      m_pinStateVector.push_back(i_pinState);
    } else {
      LOG_DEBUG("Updateing the last pin state!");
      m_pinStateVector.back().update(i_pinState);
    }
  }
  m_numberOfRepetitions = 1;
  return true;
}


bool StateSequence::addToSequence(
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


bool StateSequence::addToSequence(const StateSequence& i_sequence) {
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
              m_numberOfRepetitions << "!=" <<
              i_sequence.getNumberOfRepetitions());
    return false;
  }

  if (i_sequence.getPinStateVector().size() >
      m_pinStateVector.size()) {
    LOG_DEBUG("Cannot merge the two, " <<
              "the right hand side has more pin states then the left");
    return false;
  }

  auto rhsItr = i_sequence.getPinStateVector().begin();
  auto ltsItr = m_pinStateVector.begin();

  for (;
       ltsItr != m_pinStateVector.end() &&
       rhsItr != i_sequence.getPinStateVector().end();
       ltsItr++, rhsItr++) {
    ltsItr->update(*rhsItr);
  }
  return true;
}

bool StateSequence::mergePinStateSequence(
    StateSequence* io_sequence) {
  return StateSequence::mergePinStateSequences(this,
                                                  io_sequence);
}


bool StateSequence::mergePinStateSequences(
    StateSequence* io_firstSequence,
    StateSequence* io_secondSequence) {
  LOG_DEBUG("Merging sequences!");
  // check if we can simple add them
  if (io_firstSequence->addToSequence(*io_secondSequence)) {
    LOG_DEBUG("First sequence was empty or equal to the second " <<
              "or the pins were not mutal and the number of steps were.");
    *io_secondSequence = StateSequence();
    return true;
  }
  // check if both sequence do not define the same pins
  if (io_firstSequence->hasMutualPins(*io_secondSequence)) {
    LOG_DEBUG("The first sequence has the same pins " <<
              "as the second therefor cannot be merged");
    return false;
  }
  // check if the first sequence has more repetitions then the second
  if (io_firstSequence->getNumberOfRepetitions() <
      io_secondSequence->getNumberOfRepetitions()) {
      LOG_DEBUG("The second sequence has more repetitions then the first" <<
                "Using recursive call to fix it");
      return StateSequence::mergePinStateSequences(io_secondSequence,
                                                   io_firstSequence);
  }
  // Merge them!
  LOG_DEBUG("Now we are sure we can merge them!");
  int newFirstSequenceRepetitions =
      io_firstSequence->getNumberOfRepetitions() -
      io_secondSequence->getNumberOfRepetitions();
  if (newFirstSequenceRepetitions <= 0) {
    LOG_DEBUG("first sequence reps: " << io_firstSequence->getNumberOfRepetitions());
    LOG_DEBUG("second sequence reps: " << io_secondSequence->getNumberOfRepetitions());
    LOG_DEBUG("First sequence");
    io_firstSequence->displaySequence();
    LOG_DEBUG("Second sequence");
    io_secondSequence->displaySequence();
    return false;
  }

    if (io_secondSequence->getPinStateVector().size() >
      io_secondSequence->getPinStateVector().size()) {
    LOG_DEBUG("Cannot merge the two, " <<
              "the right hand side has more pin states then the left");
    return false;
  }

  io_firstSequence->setNumberOfRepetitions(
      io_secondSequence->getNumberOfRepetitions());

  io_secondSequence->addToSequence(*io_firstSequence);
  io_firstSequence->setNumberOfRepetitions(newFirstSequenceRepetitions);
  return true;
}


bool StateSequence::
  setStateForSequence(const PinState& i_pinState,
                      const bool& i_extend /*= false*/,
                      const bool& i_overrideSequence /*=false*/) {
  // Set a state for the whole sequence
  // If i_extend,
  //    the state will be set untill pins from i_pinState are defined once
  // When these pins are undefined in following states
  //    The latest state, defined, state will be used to define these pins
  // If i_override, the state will be forced independend on earlier definitions

  // check if not both are set
  if (i_extend and i_overrideSequence)
    LOG_ERROR("Cannot extend and override at the same time!");

  PinState latestPinState = i_pinState;
  bool overrideSequence = i_overrideSequence;
  bool extendSequence = i_extend;
  // if none are set, ensure that there are no mutal pins
  if (!(extendSequence or overrideSequence)) {
    if (hasMutualPins(i_pinState)) {
      LOG_DEBUG("Extend or overide was not selected " <<
                "and has mutual pins," <<
                "could not set the state for the sequence!");
      return false;
    } else {
      extendSequence = false;
      overrideSequence = true;
    }
  }

  for (auto pinStateIterator = m_pinStateVector.begin();
       pinStateIterator != m_pinStateVector.end();
       pinStateIterator++) {
    // if override, override....
    if (overrideSequence) {
      pinStateIterator->update(i_pinState);
    }

    // extending states
    if (extendSequence) {
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


void StateSequence::displaySequence() const {
  LOG_INFO("Speed : " << m_speed);
  LOG_INFO("Number of repititions: " << m_numberOfRepetitions);
  LOG_INFO("Number of steps: " << m_pinStateVector.size());

  for (auto itr = m_pinStateVector.begin();
       itr != m_pinStateVector.end();
       itr++) {
    itr->displayPinState();
  }
}


size_t StateSequence::getSizeOfMessage() const {
  return (sizeof(m_numberOfRepetitions) +
          sizeof(m_speed) +
          m_pinStateVector.size() * sizeof(int));
}


std::vector<int> StateSequence::createArduinoBuffer() const {
  int sizeOfMessage = getSizeOfMessage();
  LOG_DEBUG("Size of message will be: " <<  sizeOfMessage << "bytes.");
  std::vector<int> buffer;
  buffer.push_back(sizeOfMessage);
  buffer.push_back(m_speed);
  buffer.push_back(m_numberOfRepetitions);
  std::vector<int> integerSequence = getIntegerSequence();
  buffer.insert(buffer.end(), integerSequence.begin(), integerSequence.end());

  int realMessageSize = (buffer.size() - 1) * sizeof(*buffer.data());
  if (realMessageSize
      != static_cast<int>(sizeOfMessage))
    LOG_ERROR("Buffer is not correctly created, the size is: " << realMessageSize << "!");

  return buffer;
}


bool StateSequence::condenseSequence() {
  int numberOfSteps = m_pinStateVector.size();
  auto sequencePosition = m_pinStateVector.begin();

  for (int newSequenceSize = 1;
       newSequenceSize <= numberOfSteps/2;
       newSequenceSize++) {
    LOG_DEBUG("Checking size: " << newSequenceSize);
    // test if we can devide the number of steps to the new number of steps
    if (numberOfSteps % newSequenceSize != 0) {
      LOG_DEBUG("New size would not fit the vector!");
      continue;
    }

    // for each part, check if it is the same as the previous
    for (sequencePosition = m_pinStateVector.begin() + newSequenceSize;
         sequencePosition != m_pinStateVector.end();
         sequencePosition += newSequenceSize) {
      if (!std::equal(sequencePosition,
                      sequencePosition + newSequenceSize,
                      m_pinStateVector.begin(),
                      StateSequence::areEqualState)) {
        LOG_DEBUG("Partial vector does not represent the whole vector!");
        break;
      }
    }

    if (sequencePosition == m_pinStateVector.end()) {
      // can be condensed
      m_numberOfRepetitions *= numberOfSteps / newSequenceSize;
      m_pinStateVector.erase(m_pinStateVector.begin() + newSequenceSize,
                             m_pinStateVector.end());
      return true;
    }
  }
  return false;
}


bool StateSequence::areEqualState(const PinState& i_firstState,
                                     const PinState& i_secondState) {
  return i_firstState.getNumericValue() == i_secondState.getNumericValue();
}

