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
  void writeIntegerToBuffer(const int& i_value,
                            char*& i_bufferPosition) const;
 public:
  PinStateSequence();

  PinStateSequence(const int& i_speed,
                   const int& i_numberOfRepetitions,
                   const PinStateVector& i_pinStateVector);

  bool isEmpty() const;

  bool addToSequence(const PinState& i_pinState);
  bool addToSequence(const PinStateVector& i_pinStateVector);
  bool addToSequence(const PinStateSequence& i_otherSequence);

  bool setStateForSequence(const PinState& i_pinState,
                           const bool& i_extent = false,
                           const bool& overrideSequence = false);

  bool setStateForSequence(const PinStateVector& i_pinStateVector,
                           const bool& i_extent = false,
                           const bool& i_overRide = false);

  std::vector<int> getIntegerSequence() const;
  static bool mergePinStateSequences(PinStateSequence* io_firstSequence,
                                     PinStateSequence* io_secondSequence);

  bool mergePinStateSequence(PinStateSequence* io_sequence);

  void displaySequence() const;

  size_t getSizeOfMessage() const;
  void createArduinoBuffer(char*& i_buffer,
                            size_t& i_messageSize) const;
};
#endif  // MOTOR_PINSTATESEQUENCE_H_
