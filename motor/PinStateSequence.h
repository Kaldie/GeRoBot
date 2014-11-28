// Copyright [2014] Ruud Cools

class PinStateSequence
  
  GETSET(int, m_numberOfSteps, NumberOfSteps);
  GETSET(int, m_numberOfRepetitions,NumberOfRepetitions); 
  GETSET(int, m_speed, Speed);  // speed is defined in hecto Hertz (100 hertz so 50 is 5k hertz)
  GETSET(vector<int>,m_integerSequence,IntegerSequence);

 public:
  PinStateSequence();
  PinStateSequence(const int& i_numberOfSteps,
                   const int& i_numberOfRepetitions,
                   const int& i_speed,
                   const vector<int> i_integerSequence);

  static PinStateSequence resolvePinStateSequence(vector<PinState>* i_pinStateVector);
  std::string createArduinoString() const;