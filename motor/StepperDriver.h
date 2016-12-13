#ifndef StepperDriver_h
#define StepperDriver_h

#include <BaseMotor.h>

// Forward declared classes
class StateSequence;
class SequenceVector;

class StepperDriver: public BaseMotor {
 public:
  typedef std::shared_ptr<StepperDriver> DriverPointer;

  /// Maximum frequency (in deca hertz) which the motor starts from
  GETSET(int, m_pullIn, PullIn);

  /// Maximum frequency (in deca hertz) which the motor can stop from
  GETSET(int, m_pullOut, PullOut);

  /// Maximum frequency (in deca hertz)
  GETSET(int, m_maxSpeed, MaxSpeed);

  /// How manny step between pullIn and maxSpeed
  GETSET(int, m_incremental, Incremental);
 private:
  // Private methods to have easy acces to certain pins, such as enable pin
  const int getPin(const int i_number) const
  {return getCurrentPinState().getPinVector()[i_number];}

  int enablePin() const {return getPin(0);}
  int directionPin() const {return getPin(1);}
  int stepPin() const {return getPin(2);}

  bool setDirection(const std::string&);
  int getPinValueForDirection(const std::string&);

 public:
  // Constructors
  StepperDriver();

  // Construct Driver from given pin numbers:
  // enable pin, direction pin, en step pin
  explicit StepperDriver(const PinVector&);

  // Constructor given pin number and default direction
  StepperDriver(const PinVector& i_pinVector,
                const std::string& i_defaultDirection,
                const int& i_pullIn,
                const int& i_pullOut,
                const int& i_max);

  // Enable functions of the driver!
  virtual bool setEnable(const bool&);

  /// Add to the Statesequence states which will ensure the motor sets a step
  virtual void moveStep(const std::string&,
                        StateSequence*);


  virtual void moveSteps(const std::string& i_motorDirection,
                         const int& i_numberOfSteps,
                         SequenceVector* i_vector);

  /// return the maximum speed the motor could achive right now
  /// including acceleration etc.
  virtual int getMaximumSpeed() const;

  /// return the minimum speed the motor could achive right now
  /// including breaking etc.
  virtual int getMinimumSpeed() const;


  /// Return the number of states needed per step
  virtual int numberOfStatesPerStep() const {return 2;}

  /// from a pin state, determine the status of the motor at that point
  virtual void getMotorStatus(const PinState& i_pinState,
			      bool* i_isEnabled,
			      std::string* i_rotationDirection) const;

  // Display pin state vector
  virtual void displayPinState(const PinState&) const;
  virtual void displayPinState()const;
};
#endif  // MOTOR_STEPPERDRIVER_H_
