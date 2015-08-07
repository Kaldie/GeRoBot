// Copyright [2015] Ruud Cools

#ifndef MOTOR_CONDENSOR_H_
#define MOTOR_CONDENSOR_H_

#include <StateSequence.h>
/**
 * Condensor will reduce the sizes of the sequence and sequence vectors.
 * Condensor is a namespace which is all about condensing the sequence vector,
 * The recompile method needs alot of info that is passed along several methods
 * Therefore a data struct is designed to pass it happely along as a pointer
 * making the methods nice and short/ readable
 */
namespace Condensor {
  /**
   * of the states sequence just before
   * and after the earlier mentioned sequence
   * values of the states that will be repititioned once
   */
  struct CompileSet {
    /// integer vector describing the integer
    /// states before the current StateSequence
    std::vector<int> preIntegerSequence;

    /// integer vector describing the integer states
    /// after the current StateSequence
    std::vector<int> postIntegerSequence;

    /// integer vector describing the integer states the current StateSequence
    std::vector<int> integerSequence;

    /// Iterator, which indicates the current investigated state
    PinStateSequenceVector::iterator currentSequence;
    /// Iterator, which indicates the first none investigate state
    PinStateSequenceVector::iterator endSequence;

    ///  Pointer to the sequence vector which need to be reduced
    PinStateSequenceVector* sequenceVector;

    /// State sequence which is currently being recompiled
    StateSequence stateSequence;
  };

  /**
   * Description of the recurrence that have been found.
   * Tuple that will be passed around to handle recurences
   * The three iterators are: start of the sequence
   * @param i_recurenceBegin start of the recurence
   * @param i_recurenceEnd end of the recurence (exclusive)
   * @param i_sequenceEnd the end of the sequence which is composed of multiple i_recurenceBegin->i_recurenceEnd
   */
  typedef
      std::tuple<
      std::vector<int>::const_iterator,
      std::vector<int>::const_iterator,
      std::vector<int>::const_iterator>
      RecurrenceResult;

  /// Vector of RecurenceResults
  typedef std::vector<RecurrenceResult> RecurrenceVector;

  /**
   * Determine if the current StateSequence can be
   * reduced by finding recurences in the following
   */
  bool handleForwardCorrispondingSequences(CompileSet* i_compileSet);

  /**
   * Determine if the current StateSequence can be
   * reduced by finding recurences in the previous
   */
  bool handleBackwardCorrispondingSequences(
      CompileSet* i_compileSet);


  /// Reset the compile set
  void cleanCompileSet(CompileSet* i_compileSet);

  /**
   * Reorders the current StateSequence to shorten it.
   * First it will find recurrences using Condensor::findRecurrence <br>
   * After this it will select the recurence which will
   * effect the length the most: Condensor::maximumEffectRecurence<br>
   * Finaly the sequence is recompile using Condensor::handleRecurence.
   */
  bool recompile(CompileSet* i_compileSet);

  /// finds sequence with 1 repitition, sets the pre and post integer sequences
  void prepareSequences(
      CompileSet* i_compileSet);

  /// find sequence which are repeated in the sequence
  std::vector<RecurrenceResult> findRecurrence(
      const std::vector<int>& i_vector);

  /// for the repition which has the most effect to replace
  Condensor::RecurrenceVector::const_iterator findMaximumEffectRecurence(
          const Condensor::RecurrenceVector& i_recurrenceVector,
          const std::vector<int>& i_integerSequence);


  /// recompile the investigated sequence if it makes sense
  bool handleRecurence(
      const Condensor::RecurrenceVector::const_iterator& maximumEffectRecurence,
      CompileSet* i_compileSet);

  /**
   * Split the StateSequence and add it the the SequenceVector
   * @param[in] i_compileSet pointer to the current set that needs to be shortened
   * @param[in] i_stateSequence Sequence that needs to be split
   * @param[in] i_split Iterator where the sequence will be split
   * @param[in] i_leaveStart switch the determines
   * if the start is left in the current sequence or the end
   */
  void splitSequence(CompileSet* i_compileSet,
                     StateSequence* i_stateSequence,
                     const std::vector<int>::const_iterator& i_split,
                     bool i_leaveStart);

  /// insertion position relative to current sequence
  void addSequenceToVector(CompileSet* i_compileSet,
                           const int& insertionPosition,
                           StateSequence* i_newSequence);

  /**
   * Update the current state using the maximum effect recurence.
   * Using the previously defined maximum effect
   * occurence we will recompile the current StateSequence. <br>
   */
   void updateCurrentState(CompileSet* i_compileSet,
                          const RecurrenceResult& maximumEffectRecurence);

  const int statePenalty = 3;

  /**
   * Function which tries to reduce the sequence vector.
   * This is attempted using 2 methods. <br>
   * External recompile: using the sequence before and after.
   * the current to find corrisponding
   * sequence which can be added to those. <br>
   * This is handle in Condensor::handleForwardCorrispondingSequences and
   * Condensor::handleBackwardCorrispondingSequences <br>
   * Internal recompile: using repititions
   * that occure inside the current sequence
   * which is handle in Condensor::recompile
   */
  bool recompileSequenceVector(PinStateSequenceVector* i_sequenceVector);

  /**
   * Reduce the size of the vector by checking if
   * there are full recurences in the StateSequences themself.
   */
  bool internalCondense(PinStateSequenceVector* i_sequenceVector);

  /**
   * Reduce the size of the vector by
   * checking if it fits in the previous StateSequence.
   */
  bool mergeCondense(PinStateSequenceVector* i_sequenceVector,
                     const bool& i_removeFromVector = false);
};


#endif  // MOTOR_CONDENSOR_H_
