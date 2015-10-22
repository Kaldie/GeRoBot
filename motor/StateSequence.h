// Copyright [2014] Ruud Cools

#ifndef MOTOR_PINSTATESEQUENCE_H_
#define MOTOR_PINSTATESEQUENCE_H_

#include <PinState.h>
typedef std::vector<PinState> PinStateVector;

class StateSequence {
  // speed is defined in deco Hertz (so 50 is 500 hertz)
  GETSET(int, m_speed, Speed);
  GETSET(int, m_numberOfRepetitions, NumberOfRepetitions);
  GETSET(PinStateVector, m_pinStateVector, PinStateVector);
 public:
  /// default constructor
  StateSequence();
  /// fully defined constructor
  StateSequence(const int& i_speed,
                const int& i_numberOfRepetitions,
                const PinStateVector& i_pinStateVector);
  /// returns if the StateSequence is empty
  bool isEmpty()const;
  /// Append StateSequence to this sequence
  bool appendSequence(const StateSequence& i_sequence);
  /**
   * Add the PinState to the last element of the sequence if possible.
   * @param[in] i_pinState PinState that will be added to the sequence
   * @param[in] i_forceAdd Tries to update the last element of the sequence, if Fails add a new element
   */
  bool addToSequence(const PinState& i_pinState,
                     const bool& i_forceAdd = false);
  /// Add several pin states to this StateSequence
  bool addToSequence(const PinStateVector& i_pinStateVector);
  /// add the other sequence to this one
  bool addToSequence(const StateSequence& i_otherSequence);
  /**
   * Set this PinState for all the states in the vector
   * @param[in] i_pinState State which will be used to override stuff
   * @param[in] i_extent will extent the StateSequence without overriding it
   * @param[in] i_overrideSequence will override all entries even if they were previously defind
   */
  bool setStateForSequence(const PinState& i_pinState,
                           const bool& i_extent = false,
                           const bool& i_overrideSequence = false);

  std::vector<int> getIntegerSequence() const;
  static bool mergePinStateSequences(StateSequence* io_firstSequence,
                                     StateSequence* io_secondSequence);

  bool mergePinStateSequence(StateSequence* io_sequence);
  bool condenseSequence();
  void displaySequence() const;
  void exportValue() const;
  std::vector<int> createArduinoBuffer() const;
   private:
  bool hasEqualSequence(const StateSequence& i_sequence) const;
  void validate() const;
  void validate(const PinStateVector& i_pinStateVector) const;

  bool hasMutualPins(const PinState& i_pinState) const;
  bool hasMutualPins(const StateSequence& i_stateSequence) const;

  static bool areEqualState(const PinState& i_firstState,
                            const PinState& i_secondState);

  size_t getSizeOfMessage() const;
};
#endif  // MOTOR_PINSTATESEQUENCE_H_
