// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <StateSequence.h>
#include <PinState.h>
#include <SequenceVector.h>
#include <Condensor.h>

int SequenceVector::numberOfSteps() const {
  LOG_DEBUG("Counting number of steps!");
  int numberOfSteps= 0;
  for (auto sequence = m_sequenceVector.begin();
       sequence != m_sequenceVector.end();
       sequence++) {
    numberOfSteps += sequence->getNumberOfRepetitions()*
        sequence->getPinStateVector().size();
  }
  return numberOfSteps;
}


int SequenceVector::numberOfSequences() const {
  LOG_DEBUG("Printing number of sequences!" << m_sequenceVector.size());
  return m_sequenceVector.size();
}


void SequenceVector::normalise(const bool i_condenseVector /* = false*/) {
  LOG_DEBUG("Normalising vector!");
  if (numberOfSequences() == 0) {
    LOG_DEBUG("Could not normalize vector, No sequences defined.");
    return;
  }

  PinState thisPinState = m_sequenceVector.begin()->getPinStateVector().front();
  for (auto currentSequence  = m_sequenceVector.begin();
       currentSequence != m_sequenceVector.end();
       currentSequence++) {
    currentSequence->setStateForSequence(thisPinState,
                                         true,    // extent
                                         false);  // override
    if (currentSequence->getPinStateVector().size() > 0) {
      thisPinState = currentSequence->getPinStateVector().back();
    }
    
    if (currentSequence != m_sequenceVector.end()) {
      thisPinState = *currentSequence->getPinStateVector().begin();
    } else {
      break;
    }
  }

  if (i_condenseVector)
    condenseVector();
}


bool SequenceVector::isNormilized() const {
  LOG_DEBUG("Determing if vector is normalized!");
  PinState previousPinState =
      m_sequenceVector.front().getPinStateVector()[0];
  // Sequence loop
  for (auto stateSequenceIterator = m_sequenceVector.begin();
       stateSequenceIterator != m_sequenceVector.end();
       stateSequenceIterator++) {
    // PinState loop
    for (auto pinStateIterator =
             stateSequenceIterator->getPinStateVector().begin();
         pinStateIterator !=
             stateSequenceIterator->getPinStateVector().end();
         pinStateIterator++) {
      // Pin loop
      for (auto pinIterator  = previousPinState.getPinVector().begin();
           pinIterator != previousPinState.getPinVector().end();
           pinIterator++) {
        try {
          pinStateIterator->getPinState(*pinIterator);
        } catch(std::runtime_error exception) {
          return false;
        }  // end try catch block
      }  // end pin loop
      previousPinState = *pinStateIterator;
    }  // end pin state loop
  }  // end pin state sequence loop
  return true;
}


bool SequenceVector::condenseVector(
    const bool i_removeFromVector /* = false*/) {
  bool internalyCondesed = Condensor::internalCondense(&m_sequenceVector);
  bool mergedCondensed = Condensor::mergeCondense(&m_sequenceVector,
                                                  i_removeFromVector);
  bool recompileCondensed = Condensor::recompileSequenceVector(&m_sequenceVector);
  // let it be known that at least it is a bit shorter!
  return internalyCondesed | mergedCondensed | recompileCondensed;
}


bool SequenceVector::internalCondense() {
  bool internalyCondesed = false;
  for (auto sequenceIterator = m_sequenceVector.begin();
       sequenceIterator != m_sequenceVector.end();
       sequenceIterator++) {
    internalyCondesed |= sequenceIterator->condenseSequence();
  }
  return internalyCondesed;
}


bool SequenceVector::mergeCondense(const bool& i_removeFromVector) {
  bool mergedCondensed = false;
  LOG_DEBUG("Condensing vector by merging state sequences!");
  // merge all sequential sequence with 1 repetition.
  auto sequence = m_sequenceVector.begin();
  auto nextSequence = sequence+1;

  while ((nextSequence != m_sequenceVector.end() &&
          sequence != m_sequenceVector.end())) {
    LOG_DEBUG("attempting merge condense!");
    // add the previous sequence to the next
    if (nextSequence->addToSequence(*sequence)) {
      // it has worked at least once
      mergedCondensed |= true;
      // if it worked determine how to handle the old one
      if (i_removeFromVector) {
        // remove it from the vector, slow but lowers memory
        // the returned sequence is next sequence
        sequence = m_sequenceVector.erase(sequence);
      } else {
        // make it emtpy, slightly faster
        // bit tho
        // due to the fact that we need to clear the pinStateSequenceVector
        sequence->setNumberOfRepetitions(0);
        sequence->setPinStateVector(PinStateVector());
        // update sequence
        ++sequence;
      }
    } else {
      ++sequence;
    }
    // get the next sequence of the vector
    nextSequence = sequence+1;
  }
  return mergedCondensed;
}


bool SequenceVector::recompileCondense() {
  return true;
}


void SequenceVector::clean() {
  // check if it is not empty
  if (numberOfSequences() == 0) {
    return;
  }
  // Make sure that the last state represents all pins
  LOG_DEBUG("Normalising");
  normalise();

  PinStateSequenceVector emptyPinStateSequenceVector;
  StateSequence lastSequence;
  PinState lastPinState =
      m_sequenceVector.back().getPinStateVector().back();

  lastSequence.addToSequence(lastPinState);
  lastSequence.setNumberOfRepetitions(0);
  emptyPinStateSequenceVector.push_back(lastSequence);
#ifndef NDEBUG
    lastSequence.displaySequence();
#endif
    m_sequenceVector.swap(emptyPinStateSequenceVector);
}


void SequenceVector::appendStateSequence(
    const StateSequence& i_newStateSequence,
    const bool& i_merge) {
  if (i_newStateSequence.isEmpty())
    return;

  if (m_sequenceVector.size() == 0) {
    LOG_DEBUG("First pin sequence");
    m_sequenceVector.push_back(i_newStateSequence);
    return;
  }

  if (m_sequenceVector.back().addToSequence(i_newStateSequence)) {
    LOG_DEBUG("Adding the new state sequence to the previous one!");
    return;
  }

  LOG_DEBUG("Added new state sequence to vector");
  if (i_merge) {
    StateSequence stateSequence = i_newStateSequence;
    StateSequence::
        mergePinStateSequences(&m_sequenceVector.back(),
                               &stateSequence);
    m_sequenceVector.push_back(stateSequence);
    return;
  }
  m_sequenceVector.push_back(i_newStateSequence);
}


StateSequence* SequenceVector::getLastSequence() {
  if (m_sequenceVector.size() == 0) {
    m_sequenceVector.push_back(StateSequence());
  }
  return &(*(m_sequenceVector.end()-1));
}
