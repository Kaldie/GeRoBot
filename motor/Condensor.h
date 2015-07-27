// Copyright [2015] Ruud Cools

#ifndef MOTOR_CONDENSOR_H_
#define MOTOR_CONDENSOR_H_

#include <StateSequence.h>

namespace Condensor {
  // Condensor will reduce the sizes of the sequence and sequence vectors
  /*
    Condensor is a namespace which is all about condensing the sequence vector,
    The recompile method needs alot of info that is passed along several methods
    Therefore a data struct is designed to pass it happely along as a pointer
    making the methods nice and short/ readable
  */
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
    PinStateSequenceVector::iterator endSequence;

    //  Pointer to the sequence vector which need to be reduced
    PinStateSequenceVector* sequenceVector;

    // State sequence which is currently being recompiled
    StateSequence stateSequence;
  };

  /*
    The find reoccurence method outputs this tupple:
    notify following methods of the results
  */
  typedef
      std::tuple<
      std::vector<int>::const_iterator,  // start of the integer sequence
      std::vector<int>::const_iterator,  // end of the integer sequence
      std::vector<int>::const_iterator>  // end which the last occurence ends
      RecurrenceResult;

  typedef std::vector<RecurrenceResult> RecurrenceVector;

  /*
    functions that will find if the given sequence vector
    has corrisponding sequences with previous and sequential sequence.
    These will be used to condense the vector
  */
  bool handleForwardCorrispondingSequences(CompileSet* i_compileSet);

  bool handleBackwardCorrispondingSequences(
      CompileSet* i_compileSet);

  void cleanCompileSet(CompileSet* i_compileSet);

  bool recompile(CompileSet* i_compileSet);

  // finds sequence with 1 repitition, sets the pre and post integer sequences
  void prepareSequences(
      CompileSet* i_compileSet);

  // find sequence which are repeated in the sequence
  std::vector<RecurrenceResult> findRecurrence(
      const std::vector<int>& i_vector);

  // for the repition which has the most effect to replace
  Condensor::RecurrenceVector::const_iterator findMaximumEffectRecurence(
          const Condensor::RecurrenceVector& i_recurrenceVector,
          const std::vector<int>& i_integerSequence);

  // recompile the investigated sequence if it makes sense
  bool handleRecurence(
      const Condensor::RecurrenceVector::const_iterator& maximumEffectRecurence,
      CompileSet* i_compileSet);

  void splitSequence(CompileSet* i_compileSet,
                     StateSequence* i_stateSequence,
                     const std::vector<int>::const_iterator& i_split,
                     bool i_leaveStart);

  // insertion position relative to current sequence
  void addSequenceToVector(CompileSet* i_compileSet,
                           const int& insertionPosition,
                           StateSequence* i_newSequence);

  void updateCurrentState(CompileSet* i_compileSet,
                          const RecurrenceResult& maximumEffectRecurence);

  const int statePenalty = 3;

  bool recompileSequenceVector(PinStateSequenceVector* i_sequenceVector);
  bool internalCondense(PinStateSequenceVector* i_sequenceVector);
  bool mergeCondense(PinStateSequenceVector* i_sequenceVector,
                     const bool& i_removeFromVector = false);
};


#endif  // MOTOR_CONDENSOR_H_
