// Copyright [2014] Ruud Cools

#ifndef MOTOR_BASEMOTOR_H_
#define MOTOR_BASEMOTOR_H_

#include <PinState.h>

// Forward declared, only used as reference
class SequenceVector;
class StateSequence;

class BaseMotor {
 public:
  typedef std::shared_ptr<BaseMotor> MotorPointer;
  
  enum MotorType {StepperDriver, StepperMotor, None};
  // Motor type
  GETSET(MotorType, m_motorType, Type);
  
  /// State in which the pins currently resides
  GETSETPROTECTED(PinState, m_currentPinState, CurrentPinState);

  /// Direction which the motor goes when all pins are in default mode
  GETSET(std::string, m_defaultDirection, DefaultDirection);

  /// After setting the step, hold the motor or release the current
  GETSET(bool, m_holdMotor, HoldMotor);

  /// Current speed of the motor
  GETSETPROTECTED(int, m_speed, Speed);

 public:
  PinState* getPinStatePointer() {return &m_currentPinState;}

  virtual void setPins(const PinVector&);

  virtual PinVector getPins() const {return m_currentPinState.getPinVector();}

  // Move steps
  virtual void moveStep(const std::string&,
                        StateSequence*) =0;

  virtual void moveSteps(const std::string& i_motorDirection,
                         const int& i_numberOfSteps,
                         SequenceVector* i_vector) =0;

  /// from a pin state, determine the status of the motor at that point
  virtual void getMotorStatus(const PinState& i_pinState,
			      bool* i_isEnabled,
			      std::string* i_rotationDirection) const = 0;


  /// return the number of states needed for 1 step
  virtual int numberOfStatesPerStep() const = 0;

  /// return the maximum speed the motor could achive right now
  /// including acceleration etc.
  virtual int getMaximumSpeed() const = 0;

  /// return the minimum speed the motor could achive right now
  /// including breaking etc.
  virtual int getMinimumSpeed() const = 0;

  // Print pin states
  virtual void displayPinState() const;
  virtual void displayPinState(const PinState&) const;

  /// Print debug messages to std out
  void displayPinStateSequence(const StateSequence&) const;

  // Base Constructor
  BaseMotor();

  // Defining the type
  BaseMotor(const MotorType&);

  // Vector with pin IDs
  explicit BaseMotor(const MotorType&,
		     const PinVector&);

  // Pin IDs and default direction
  BaseMotor(const MotorType&,
	    const PinVector&,
            const std::string&);

    // Pin IDs and default direction
  BaseMotor(const MotorType&,
	    const PinVector& i_pinVector,
            const std::string& i_defaultDirection,
            const bool i_holdMotor);

  virtual ~BaseMotor() {}
};
#endif  // MOTOR_BASEMOTOR_H_
