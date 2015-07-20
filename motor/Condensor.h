// Copyright [2015] Ruud Cools

#ifndef MOTOR_CONDENSOR_H_
#define MOTOR_CONDENSOR_H_

class Condensor {
  // Condensor will reduce the sizes of the sequence and sequence vectors
 private:
  PinStateVector m_pinStateVector;
  /*
    sequence describing the integer
    values of the state that will be repititioned once
  */
  std::vector<int> m_integerSequence; 

  /*
    sequence describing the integer states
    of the state sequence just before
    and after the earlier mentioned sequence
  */
  std::vector<int> m_preIntegerSequence;
  std::vector<int> m_postIntegerSequence;

  /*
    the new sequence that will replace the condense state sequences
  */
  StateSequence m_newSequence;

  
  //  Pointer to the sequence vector
  PinStateSequenceVector* m_sequenceVector;
  
  /*
    functions that will find if the given sequence vector
    has corrisponding sequences with previous and sequential sequence.
    These will be used to condense the vector
  */
  int handleForwardCorrispondingSequences();
  int handleBackwardCorrispondingSequences();

  // finds sequence with 1 repitition, sets the pre and post integer sequences
  PinStateSequenceVector::iterator prepareSequences(
      const PinStateSequenceVector::iterator& i_startSequence,
      StateSequence* i_mergedSequence);

  condenseSequence(std::vector<int>* prevSequence,
                   std::vector<int>* postSequence,
                   std::vector<int>* currentSequence);
 
 public:
                   
  bool condenseSequenceVector(PinStateSequenceVector* i_sequenceVector);
};
