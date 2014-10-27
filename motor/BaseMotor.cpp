#include <iostream>
#include "BaseMotor.h"

//Constructors
BaseMotor::BaseMotor()
    :
    m_defaultDirection("CW"),
    m_holdMotor(false)
{
  PinVector pinVector{1,2,3,4};
  setPins(pinVector);
}

BaseMotor::BaseMotor(const PinVector& i_pins)
    :
    m_defaultDirection("CW"),
    m_holdMotor(false)
{
    setPins(i_pins);
}

BaseMotor::BaseMotor(const PinVector& i_pins,
										 const std::string& i_defaultDirection)
    :
    m_defaultDirection(i_defaultDirection),
    m_holdMotor(false)
{
    setPins(i_pins);    
}


void BaseMotor::setPins(const PinVector& i_pins)
{
    m_currentPinState.setPins(i_pins);
}

void BaseMotor::displayPinState() const
{
    displayPinState(m_currentPinState);
}

void BaseMotor::displayPinState(const PinState& i_pinState) const
{
    i_pinState.displayPinState();
}


void BaseMotor::displayPinStateSequence(const PinStateSequence& i_pinStateSequence) const
{

   int index=0;
    for(PinStateSequence::const_iterator itr=i_pinStateSequence.begin();
	itr!=i_pinStateSequence.end();
	itr++,index++)
	{
	    std::cout<<"------- at index: "<<index<<"--------"<<std::endl;
	    displayPinState(*itr);
	}
}
