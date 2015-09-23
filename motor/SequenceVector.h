// Copyright [2014] Ruud Cools

#ifndef MOTOR_SEQUENCEVECTOR_H_
#define MOTOR_SEQUENCEVECTOR_H_

#include "./StateSequence.h"

typedef std::vector<StateSequence> PinStateSequenceVector;

class SequenceVector {
  GETSET(PinStateSequenceVector,
         m_sequenceVector,
         SequenceVector);

 private:
  StateSequence* getLastSequence();
  bool mergeCondense(const bool& i_removeFromVector);
  bool internalCondense();
  bool recompileCondense();
 public:
  int numberOfSteps() const;
  int numberOfSequences() const;

  void normalise(const bool i_condenseVector = false);
  bool isNormilized() const;
  /**
   * add state to the sequence vector
   * @param[in] i_stateSequence New sequence which will be added to the vector
   * @param[in] i_merge Merge this new sequence with the old if possible
   */
  void appendStateSequence(const StateSequence& i_stateSequence,
                           const bool& i_merge);

  std::vector<StateSequence>::const_iterator begin()
      const { return m_sequenceVector.begin();};
  std::vector<StateSequence>::const_iterator end()
      const { return m_sequenceVector.end();};

  /// Condense the sequence vector using Conderser.
  /// @param[in] i_removeElements remove elements from the vector
  /// turn false for speed, true for memory
  bool condenseVector(bool i_removeElements = false);

  /// Add a pinstate to the last sequence of the vector
  /// @param[in] i_pinState Pin state which will be added to the vector
  /// @param[in] i_forceAdd
  /// If there is 1 repition, add it even if it has mutual pin
  bool addToSequence(const PinState& i_pinState,
                     const bool& i_forceAdd = false) {
    return getLastSequence()->addToSequence(i_pinState,
                                            i_forceAdd);};

  bool addToSequence(const PinStateVector& i_pinStateVector) {
    return getLastSequence()->addToSequence(i_pinStateVector);};

  bool addToSequence(const StateSequence& i_otherSequence) {
    return getLastSequence()->addToSequence(i_otherSequence);};

  void clean();
};

#endif  // MOTOR_SEQUENCEVECTOR_H_
