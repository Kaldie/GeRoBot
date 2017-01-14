// Copyright [2014] Ruud Cools

#ifndef MOTOR_PINSTATE_H_
#define MOTOR_PINSTATE_H_

typedef std::vector<int> PinVector;

class PinState {
  // Vector of pins included
  GETSET(PinVector, m_pinVector, PinVector);
  // Numeric value of this state
  GETSET(unsigned int, m_numericValue, NumericValue);

 private:
  // represents the integer value of a pin
  int getNumericPinValue(const int&) const;

 public:
  /// Set
  void setPins(const PinVector&);
  void setPinsToDefault();

  void update(const int& i_pinNumber,
              const int& i_pinValue);
  void update(const PinState&);

  /// Return if a pin is defined in this state
  bool hasPin(const int& i_number) const;

  // get the set of a specific pin
  int getPinState(const int&) const;

  void resetPinState();

  void displayPinState() const;

  // Opperators
  bool operator==(const PinState& rhs) const;

  // constructors
  PinState();

  // vector of pin numbers which will be set to default value
  explicit PinState(const PinVector&);

  explicit PinState(const PinVector& i_vector, const bool& i_setting);
};

#endif  // MOTOR_PINSTATE_H_
