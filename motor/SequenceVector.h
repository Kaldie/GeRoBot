// Copyright [2014] Ruud Cools

#ifndef MOTOR_SEQUENCEVECTOR_H_
#define MOTOR_SEQUENCEVECTOR_H_

#include <StateSequence.h>

class SequenceVector {
  GETSET(PinStateSequenceVector,
         m_sequenceVector,
         SequenceVector);

 private:
  // all consequtive sequences with 1 repetitions will be grouped in 1 sequence
  // after this is done try to find repitions
  bool condenseVector(bool i_removeElements = false);

 public:
  int numberOfSteps() const;
  int numberOfSequences() const;
  
  void normalise(const bool i_condenseVector = false);
  bool isNormilized() const;
};

#endif  // MOTOR_SEQUENCEVECTOR_H_
