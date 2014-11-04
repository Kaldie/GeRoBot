/*
  Header file for the StepperMotor
  Will also define the direction enum
*/

#ifndef BaseMotor_h
#define BaseMotor_h

#include <PinState.h>

//Pin definitions
/*
  An integer which represents the pins.
  Each pin represents a bit with the position as pin number
  So we get a sort of binary expression of our pins
  if DefaultState then its low
  else its high
*/

class BaseMotor
{
	//State in which the pins currently resides
	GETSET(PinState, m_currentPinState,CurrentPinState);
	
	//Direction which the motor goes when all pins are in default mode
	GETSET(std::string,m_defaultDirection,DefaultDirection);
	
	//After setting the step, hold the motor or release the current
	GETSET(bool,m_holdMotor,HoldMotor);
  
 public:

	PinState* getPinStatePointer(){return &m_currentPinState;};
	virtual void setPins(const PinVector&);
	virtual PinVector getPins() const {return m_currentPinState.getPinVector();};
		
	//Move steps
	virtual void moveStep(const std::string&,
												PinStateSequence&) =0;

	virtual void moveSteps(const std::vector<std::string>*,
												 PinStateSequence&) =0;

	virtual int numberOfStatesPerStep() const =0;

	//Print pin states
	virtual void displayPinState()const;
	virtual void displayPinState(const PinState&)const;

	void displayPinStateSequence(const PinStateSequence&)const;

	//Base Constructor
	BaseMotor();

	//Vector with pin IDs
	BaseMotor(const PinVector&);
    
	//Pin IDs and default direction
	BaseMotor(const PinVector&,
						const std::string& );

};
 
#endif 
