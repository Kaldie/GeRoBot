// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <StateSequence.h>
#include <PinState.h>
#include <SequenceVector.h>


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
  if (numberOfSequences() < 2) {
    LOG_DEBUG("Could not normalize vector, not enough sequences.");
    return;
  }

  for (auto currentSequence = m_sequenceVector.begin() + 1;
       currentSequence !=  m_sequenceVector.end();
       currentSequence++) {
    currentSequence->setStateForSequence(
        (currentSequence-1)->getPinStateVector().back(),
        true,    // extent
        false);  // override
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
  bool hasCondensed = false;
  LOG_DEBUG("Condensing vector!");
  // merge all sequential sequence with 1 repetition.
  for (auto sequence = m_sequenceVector.end(),
           begin = m_sequenceVector.begin();
       sequence != begin;
       sequence--) {
    if (sequence->getNumberOfRepetitions() == 1) {
      if ((sequence-1)->addToSequence(*sequence)) {
        if (i_removeFromVector) {
          sequence = m_sequenceVector.erase(sequence);
        } else {
          sequence->setNumberOfRepetitions(0);
        }
        hasCondensed = true;
      }
    }
  }
  return hasCondensed;
}


void SequenceVector::clean() {
  // check if it is not empty
  if (numberOfSequences() == 0) {
    return;
  }
  // Make sure that the last state represents all pins
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
    m_sequenceVector.push_back(i_newStateSequence);
    return;
  }

  if (m_sequenceVector.back().addToSequence(i_newStateSequence))
    return;

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


StateSequence& SequenceVector::getLastSequence() {
  if (m_sequenceVector.size() == 0) {
    m_sequenceVector.push_back(StateSequence());
  }

  return *(m_sequenceVector.end()-1);
}
