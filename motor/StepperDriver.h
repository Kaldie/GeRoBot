#ifndef StepperDriver_h
#define StepperDriver_h

#include <BaseMotor.h>

class StepperDriver: public BaseMotor {
 private:
  // Private methods to have easy acces to certain pins, such as enable pin
  const int getPin(const int i_number) const
  {return getCurrentPinState().getPinVector()[i_number];}

  const int enablePin() const {return getPin(0);}
  const int directionPin() const {return getPin(1);}
  const int stepPin() const {return getPin(2);}

  const bool setDirection(const std::string&);
  const int getPinValueForDirection(const std::string&);

 public:
  // Constructors
  StepperDriver();

  // Construct Driver from given pin numbers:
  // enable pin, direction pin, en step pin
  explicit StepperDriver(const PinVector&);

  // Constructor given pin number and default direction
  StepperDriver(const PinVector& i_pinVector,
                const std::string& i_defaultDirection);

  // Enable functions of the driver!
  virtual bool setEnable(const bool&);

  // Move Steps
  virtual void moveStep(const std::string&,
                        StateSequence&);

  virtual void moveSteps(const std::string&,
                         const int& i_numberOfSteps,
                         PinStateSequenceVector&);

  virtual int numberOfStatesPerStep() const {return 2;}
	
  // Display pin state vector
  virtual void displayPinState(const PinState&) const;
  virtual void displayPinState()const;
};
#endif  // MOTOR_STEPPERDRIVER_H_
