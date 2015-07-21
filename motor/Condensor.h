// Copyright [2015] Ruud Cools

#ifndef MOTOR_CONDENSOR_H_
#define MOTOR_CONDENSOR_H_

#include <StateSequence.h>

struct CompileSet {
  /*
    integer vector describing the integer states
    of the states sequence just before
    and after the earlier mentioned sequence
    values of the states that will be repititioned once
  */
  std::vector<int> preIntegerSequence;
  std::vector<int> postIntegerSequence;
  std::vector<int> integerSequence;

  // Iterator, which indicates the current investigated state
  PinStateSequenceVector::iterator currentSequence;

  //  Pointer to the sequence vector which need to be reduced
  PinStateSequenceVector* sequenceVector;

  // State sequence which is currently being recompiled
  StateSequence stateSequence;
};


class Condensor {
  // Condensor will reduce the sizes of the sequence and sequence vectors
 private:
  /*
    functions that will find if the given sequence vector
    has corrisponding sequences with previous and sequential sequence.
    These will be used to condense the vector
  */
  static bool handleForwardCorrispondingSequences(CompileSet* i_compileSet);
  static bool handleBackwardCorrispondingSequences(
      CompileSet* i_compileSet,
      PinStateSequenceVector::iterator i_endSequence);

  static void cleanCompileSet(CompileSet* i_compileSet);

  static bool recompile(CompileSet* i_compileSet,
                        PinStateSequenceVector::iterator* i_endSequence);

  
  // finds sequence with 1 repitition, sets the pre and post integer sequences
  static PinStateSequenceVector::iterator prepareSequences(
      CompileSet* i_compileSet);

  bool condenseSequence(std::vector<int>* prevSequence,
                   std::vector<int>* postSequence,
                   std::vector<int>* currentSequence);

  static const int requiredReduction = 3;
  
 public:
  static bool recompileSequenceVector(PinStateSequenceVector* i_sequenceVector);
  static bool internalCondense(PinStateSequenceVector* i_sequenceVector);
  static bool mergeCondense(PinStateSequenceVector* i_sequenceVector,
                     const bool& i_removeFromVector = false);
};


#endif  // MOTOR_CONDENSOR_H_
