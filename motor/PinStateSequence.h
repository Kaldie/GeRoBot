#ifndef MOTOR_PINSTATESEQUENCE_H_
#define MOTOR_PINSTATESEQUENCE_H_

// Copyright [2014] Ruud Cools

class PinStateSequence {
  GETSET(int, m_numberOfSteps, NumberOfSteps);
  GETSET(int, m_numberOfRepetitions, NumberOfRepetitions);
  // speed is defined in hecto Hertz (100 hertz so 50 is 5k hertz)
  GETSET(int, m_speed, Speed);
  GETSET(std::vector<int>, m_integerSequence, IntegerSequence);

 private:
  vector<int>* getIntegerSequencePointer() {return &m_integerSequence;}
  
 public:
  PinStateSequence();
  PinStateSequence(const int& i_numberOfSteps,
                   const int& i_numberOfRepetitions,
                   const int& i_speed,
                   const std::vector<int> i_integerSequence);

  static PinStateSequence resolvePinStateSequence(std::vector<PinState>* i_pinStateVector);
  std::string createArduinoString() const;
};
#endif  // MOTOR_PINSTATESEQUENCE_H_
