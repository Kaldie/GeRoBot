// Copyright [2014] Ruud Cools

#ifndef MOTOR_PINSTATESEQUENCE_H_
#define MOTOR_PINSTATESEQUENCE_H_

class PinStateSequence {
  // speed is defined in hecto Hertz (10 hertz so 50 is 5k hertz)
  GETSET(int, m_speed, Speed);
  GETSET(int, m_numberOfRepetitions, NumberOfRepetitions);
  GET(PinStateVector, m_pinStateVector, PinStateVector);

 private:
  bool hasEqualSequence(const PinStateSequence& i_sequence) const;
  void validate() const;
  void validate(const PinStateVector& i_pinStateVector) const;

  bool hasMutualPins(const PinState& i_pinState) const;
  bool hasMutualPins(const PinStateSequence& i_otherSequence) const;

  std::vector<int> getIntegerSequence(
      const PinStateVector i_pinStateVector) const;


  static bool areEqualState(const PinState& i_firstState,
                            const PinState& i_secondState);

 public:
  PinStateSequence();

  PinStateSequence(const int& i_speed,
                   const int& i_numberOfRepetitions,
                   const PinStateVector& i_pinStateVector);

  bool isEmpty() const;

  bool appendSequence(const PinStateSequence i_sequence);
  
  bool addToSequence(const PinState& i_pinState);
  bool addToSequence(const PinStateVector& i_pinStateVector);
  bool addToSequence(const PinStateSequence& i_otherSequence);

  bool setStateForSequence(const PinState& i_pinState,
                           const bool& i_extent = false,
                           const bool& overrideSequence = false);

  /*
    bool setStateForSequence(const PinStateVector& i_pinStateVector,
                           const bool& i_extent = false,
                           const bool& i_overRide = false);
  */
  
  std::vector<int> getIntegerSequence() const;
  static bool mergePinStateSequences(PinStateSequence* io_firstSequence,
                                     PinStateSequence* io_secondSequence);

  bool mergePinStateSequence(PinStateSequence* io_sequence);
  bool condenseSequence();
  void displaySequence() const;

  size_t getSizeOfMessage() const;
  std::vector<int> createArduinoBuffer() const;
};
#endif  // MOTOR_PINSTATESEQUENCE_H_
