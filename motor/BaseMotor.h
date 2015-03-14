// Copyright [2014] Ruud Cools

#ifndef MOTOR_BASEMOTOR_H_
#define MOTOR_BASEMOTOR_H_

#include <PinState.h>
#include <PinStateSequence.h>

class BaseMotor {
  // State in which the pins currently resides
  GETSET(PinState, m_currentPinState, CurrentPinState);

  // Direction which the motor goes when all pins are in default mode
  GETSET(std::string, m_defaultDirection, DefaultDirection);

  // After setting the step, hold the motor or release the current
  GETSET(bool, m_holdMotor, HoldMotor);

 public:
  PinState* getPinStatePointer() {return &m_currentPinState;}
  virtual void setPins(const PinVector&);
  virtual PinVector getPins() const {return m_currentPinState.getPinVector();}

  // Move steps
  virtual void moveStep(const std::string&,
                        PinStateSequence&) =0;

  virtual void moveSteps(const std::string& i_motorDirection,
                         const int& i_numberOfSteps,
                         PinStateSequenceVector& i_vector) =0;

  virtual int numberOfStatesPerStep() const =0;

  // Print pin states
  virtual void displayPinState()const;
  virtual void displayPinState(const PinState&)const;

  void displayPinStateSequence(const PinStateSequence&)const;

  // Base Constructor
  BaseMotor();

  // Vector with pin IDs
  explicit BaseMotor(const PinVector&);

  // Pin IDs and default direction
  BaseMotor(const PinVector&,
            const std::string&);

    // Pin IDs and default direction
  BaseMotor(const PinVector& i_pinVector,
            const std::string& i_defaultDirection,
            const bool i_holdMotor);

  virtual ~BaseMotor() {}
};
#endif  // MOTOR_BASEMOTOR_H_
