// Copyright [2014] Ruud Cools

#ifndef MOTOR_SEQUENCEVECTOR_H_
#define MOTOR_SEQUENCEVECTOR_H_

#include <PinStateSequence.h>

class SequenceVector {
  GETSET(std::vector<PinStateSequence>, m_sequenceVector, SequenceVector);

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
