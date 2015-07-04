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
  // all consequtive sequences with 1 repetitions will be grouped in 1 sequence
  // after this is done try to find repitions
  bool condenseVector(bool i_removeElements = false);
  StateSequence& getLastSequence();
  //  StateSequence& getFirstSequence();

 public:
  int numberOfSteps() const;
  int numberOfSequences() const;

  void normalise(const bool i_condenseVector = false);
  bool isNormilized() const;
  void appendStateSequence(const StateSequence&,
                           const bool&);

  std::vector<StateSequence>::const_iterator begin() const { return m_sequenceVector.begin();};
  std::vector<StateSequence>::const_iterator end() const { return m_sequenceVector.end();};
  
  const bool addToSequence(const PinState& i_pinState)
    {return getLastSequence().addToSequence(i_pinState);};

  const bool addToSequence(const PinStateVector& i_pinStateVector)
    {return getLastSequence().addToSequence(i_pinStateVector);};

  const bool addToSequence(const StateSequence& i_otherSequence)
    {return getLastSequence().addToSequence(i_otherSequence);};

  void clean();
};

#endif  // MOTOR_SEQUENCEVECTOR_H_
