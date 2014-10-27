#include <iostream>
#include <algorithm>
#include <PinState.h>
#include <map>

PinState::PinState():
    m_pinVector{},
    m_numericValue(0)
{
    resetPinState();
}


PinState::PinState(const PinVector& i_pins):
    m_pinVector(i_pins),
    m_numericValue(0)
{
    setPins(i_pins);
}


void PinState::resetPinState()
{
    std::vector<int> zero{};
    m_pinVector.swap(zero);
    m_numericValue=0;
}


void PinState::setPins(const PinVector& i_pins)
{
    resetPinState();

    for(PinVector::const_iterator itr=i_pins.begin();
	itr!=i_pins.end();
	itr++)
	{
	    m_pinVector.push_back(*itr);
	    LOG_INFO("Setting: "<<(*itr)<<" to: "<<DEFAULT_STATE);
	    if(DEFAULT_STATE)
		m_numericValue+=getNumericPinValue(*itr);
	}
}

void PinState::setPinsToDefault()
{
    const PinVector pinVector(m_pinVector);
    setPins(pinVector);
}


void PinState::update(const int& i_pinNumber,
		      const int& i_pinValue)
{
    if(std::find(m_pinVector.begin(), m_pinVector.end(), i_pinNumber)!=m_pinVector.end())
	{
	    if(getPinState(i_pinNumber)!=i_pinValue)
		{
		    if(i_pinValue)
			m_numericValue+=getNumericPinValue(i_pinNumber);
		    else
			m_numericValue-=getNumericPinValue(i_pinNumber);
		}
	}
    else
	{	
	    m_pinVector.push_back(i_pinNumber);
	    if(i_pinValue)
		m_numericValue+=getNumericPinValue(i_pinNumber);
	}
}


void PinState::update(const PinState& i_pinState)
{
    for(PinVector::const_iterator itr=i_pinState.m_pinVector.begin(); 
	itr!=i_pinState.m_pinVector.end();
	itr++)
	{
	    update(*itr,i_pinState.getPinState(*itr));
	}
}


int PinState::getNumericPinValue(const int& i_pinNumber)const
{
    return 1<<i_pinNumber;
}


int PinState::getPinState(const int& i_pinNumber)const
{
    PinVector::const_iterator itr=std::find(m_pinVector.begin(), m_pinVector.end(), i_pinNumber);
    
    if(itr==m_pinVector.end())
	{
	    std::cerr<<"Pin not available!";
	    throw 100;
	}

    return (m_numericValue & getNumericPinValue(i_pinNumber))>>i_pinNumber;
}


void PinState::displayPinState()const
{
    std::cout<<"Display Pin State: "<<std::endl;
       for(PinVector::const_iterator itr=m_pinVector.begin();
	itr!=m_pinVector.end();
	itr++)
	   {
	       std::cout<<"Pin: "<<*itr<<" is in state: "<<getPinState(*itr)<<std::endl;
	   }   
}
