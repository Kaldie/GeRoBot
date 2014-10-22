#ifndef StepperDriver_h
#define StepperDriver_h

#include <vector>
#include <map>
#include "BaseMotor.h"
#include <macroHeader.h>
//#include "StepperMotor.h"

class StepperDriver: public BaseMotor
{
 private:
    //Pointers to

    const int getPin(const int i_number) const {return getCurrentPinState().getPinVector()[i_number];};
    
    const int enablePin() const{return getPin(0);};
    const int directionPin() const{return getPin(1);};
    const int stepPin() const{return getPin(2);};

    const bool setDirection(const std::string&);

    const int getPinValueForDirection(const std::string&);

 public:
    //Constructors
    StepperDriver();

    //Construct Driver from given pin numbers: enable pin, direction pin, en step pin
    StepperDriver(const PinVector&);

    //Constructor given pin number and default direction
    StepperDriver(const PinVector&,
		  const std::string&);

    //Enable functions of the driver!
    virtual const bool setEnable(const bool&);

    //Move Steps
    virtual void moveStep(const std::string&,
			  PinStateSequence&);

    virtual void moveSteps(const std::vector<std::string>*,
			   PinStateSequence& );

    virtual const int numberOfStatesPerStep() const{return 2;};

    //Display pin state vector
    virtual void displayPinState(const PinState&) const;
    virtual void displayPinState()const;
};
#endif
