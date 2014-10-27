/*Stepper driver class
  Create the control the driver

  Dep WiringPi
*/
#include <iostream>
#include <assert.h>
#include <vector>
#include <StepperDriver.h>


//Constructor
StepperDriver::StepperDriver()
  : BaseMotor({2,3,4})
{}


StepperDriver::StepperDriver(const PinVector& i_pinVector)
    : BaseMotor(i_pinVector)
{}

StepperDriver::StepperDriver(const PinVector& i_pinVector,
			     const std::string& i_defaultDirection)
    : BaseMotor(i_pinVector,i_defaultDirection)
{}


bool StepperDriver::setEnable(const bool& i_setEnable)
{
    PinState pinState= getCurrentPinState();
    int newState;
    if(i_setEnable)
			newState=!DEFAULT_STATE;
    else
			newState=DEFAULT_STATE;
    
    if(newState!=pinState.getPinState(enablePin())){
			pinState.update(enablePin(),newState);
			setCurrentPinState(pinState);
			return true;
    }
    else
			return false;
}


const int StepperDriver::getPinValueForDirection(const std::string &i_directionString)
{
    /*
      Function will give the direction pin value given a directionEnum
      First instance this function will be used at the StepperDriver
    */

    if (i_directionString==getDefaultDirection())
	return DEFAULT_STATE;
    else
	//reverse the default state
	return static_cast<int>(!static_cast<bool>(DEFAULT_STATE));
}


const bool StepperDriver::setDirection(const std::string& i_directionString)
{
    PinState pinState=getCurrentPinState();
    int newPinValue=getPinValueForDirection(i_directionString);
    
    if(newPinValue!=pinState.getPinState(directionPin()))
	{
	    LOG_INFO("Current Pin Value: "<<pinState.getPinState(directionPin())<<"\t"<<"New pin Value: "<<newPinValue);
	    if(newPinValue!=pinState.getPinState(directionPin()))
		pinState.update(directionPin(),newPinValue);
	 
	    setCurrentPinState(pinState);
	    return true;
	}
    else
	return false;

}


void StepperDriver::moveStep(const std::string& i_direction,
			     PinStateSequence &i_pinStateSequence)
{


    //Enable the driver!
    bool changeDirectionOrEnable=false;
    if(setEnable(true))
	changeDirectionOrEnable=true;
    
    if(setDirection(i_direction))
	changeDirectionOrEnable=true;
    
    if(changeDirectionOrEnable)
	i_pinStateSequence.push_back(getCurrentPinState());

    PinState pinState=getCurrentPinState();
    pinState.update(stepPin(),!DEFAULT_STATE);
    i_pinStateSequence.push_back(pinState);

    pinState.update(stepPin(),DEFAULT_STATE);
    i_pinStateSequence.push_back(pinState);

    if(!getHoldMotor())
	{	//Disable the driver!!
	    if(setEnable(false))
		i_pinStateSequence.push_back(getCurrentPinState());
	}
}


void StepperDriver::moveSteps(const std::vector<std::string>* i_directionVector,
			      PinStateSequence& o_pinStateSequence)
{
    /*
      We are always holding the moter between steps of a sequence!
      So set it to true and remember what it was...
    */
    bool holdMotorAfterSteps=getHoldMotor();
    setHoldMotor(true);

    //Enable the driver
    if(setEnable(true))
	o_pinStateSequence.push_back(getCurrentPinState());

    for(std::vector<std::string>::const_iterator direction=i_directionVector->begin();
	direction!=i_directionVector->end();
	direction++)
	{
	    moveStep(*direction,o_pinStateSequence);
	}

    setHoldMotor(holdMotorAfterSteps);
    if(!getHoldMotor())
	{
	    if(setEnable(false))
		o_pinStateSequence.push_back(getCurrentPinState());
	}

}


void StepperDriver::displayPinState(const PinState& i_pinState)const
{
    std::string pinName;
    for(PinVector::const_iterator itr=i_pinState.getPinVector().begin();
	itr!=i_pinState.getPinVector().end();
	itr++)
	{
	    if((*itr)==enablePin())
		pinName="Enable Pin";
	    else if((*itr)==directionPin())
		pinName="Direction Pin";
	    else if((*itr)==stepPin())
		pinName="Step Pin";
	    else
		pinName="un identified pin: " + std::to_string(*itr);

	    std::cout<<"State of: "<<pinName <<", "<<i_pinState.getPinState(*itr)<<std::endl;
	}

}

void StepperDriver::displayPinState()const
{
    displayPinState(getCurrentPinState());
}
