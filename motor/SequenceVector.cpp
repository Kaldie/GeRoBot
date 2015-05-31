// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <PinStateSequence.h>
#include <PinState.h>
#include "./SequenceVector.h"


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
  LOG_DEBUG("Printing number of sequences!");
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
        true, false);
  }

  if (i_condenseVector)
    condenseVector();
}


bool SequenceVector::isNormilized() const {
  LOG_DEBUG("Determing if vector is normalized!");
  PinState previousPinState =
      m_sequenceVector.front().getPinStateVector()[0];
  // Sequence loop
  for (auto pinStateSequenceIterator = m_sequenceVector.begin();
       pinStateSequenceIterator != m_sequenceVector.end();
       pinStateSequenceIterator++) {
    // PinState loop
    for (auto pinStateIterator =
             pinStateSequenceIterator->getPinStateVector().begin();
         pinStateIterator !=
             pinStateSequenceIterator->getPinStateVector().end();
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

  LOG_DEBUG("Condensing vector!");
  // merge all sequential sequence with 1 repetition.
  for (auto sequence = m_sequenceVector.end(),
           begin = m_sequenceVector.begin();
       sequence != begin;
       sequence--) {
    if (sequence->getNumberOfRepetitions() == 1) {
      if ((sequence+1)->addToSequence(*sequence)) {
        if (i_removeFromVector)
          sequence = m_sequenceVector.erase(sequence);
        else
          sequence->setNumberOfRepetitions(0);
      }
    }
  }
  return false;
}
