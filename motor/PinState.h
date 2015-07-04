// Copyright [2014] Ruud Cools

#ifndef MOTOR_PINSTATE_H_
#define MOTOR_PINSTATE_H_

typedef std::vector<int> PinVector;

class PinState {
  GETSET(PinVector, m_pinVector, PinVector);
  GETSET(int, m_numericValue, NumericValue);

 private:
  // represents the integer value of the pins
  const int getNumericPinValue(const int&) const;

 public:
  void setPins(const PinVector&);
  void setPinsToDefault();

  void update(const int& i_pinNumber,
              const int& i_pinValue);
  void update(const PinState&);

  // get the set of a specific pin
  const int getPinState(const int&) const;

  void resetPinState();

  void displayPinState() const;

  // constructors
  PinState();

  // vector of pin numbers which will be set to default value
  explicit PinState(const PinVector&);
};

#endif  // MOTOR_PINSTATE_H_
