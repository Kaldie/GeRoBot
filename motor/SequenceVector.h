// Copyright [2014] Ruud Cools

#ifndef MOTOR_SEQUENCEVECTOR_H_
#define MOTOR_SEQUENCEVECTOR_H_

typedef std::vector<StateSequence> PinStateSequenceVector;
class StateSequence;

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
  void appendStateSequence(const StateSequence&,
                           const bool&);

  std::vector<StateSequence>::const_iterator begin()
      const { return m_sequenceVector.begin();};
  std::vector<StateSequence>::const_iterator end()
      const { return m_sequenceVector.end();};

  // all consequtive sequences with 1 repetitions will be grouped in 1 sequence
  // after this is done try to find repitions
  bool condenseVector(bool i_removeElements = false);

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
