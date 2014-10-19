#include <iostream>
#include <string>
#include <wiringPi.h>
#include <vector>
#include "StepperMotor.h"



/*
  To move a step a moter need a put the signal !DEFAULT to DEFAULT
  So we need to delays
  The sequence goes like:
  Primary Delay
  !DEFAULT
  Secundary Delay
  DEFAULT
*/


StepperMotor::StepperMotor() : m_motorPins {0,1,2,3}, m_calculateSecundaryDelay(true){};


StepperMotor::StepperMotor(int i_pin1, int i_pin2, int i_pin3, int i_pin4): m_calculateSecundaryDelay(true)
{
  m_motorPins[0]=i_pin1;
  m_motorPins[1]=i_pin2;
  m_motorPins[2]=i_pin3;
  m_motorPins[3]=i_pin4;
}

StepperMotor::StepperMotor(const StepperMotor &i_stepperMotor)
{
    if (this!=&i_stepperMotor)
	{
	    std::vector<int> motorPins=i_stepperMotor.getPins();
	    int i=0;
	    for(std::vector<int>::const_iterator itr=motorPins.begin();
		itr!=motorPins.end();
		i++,itr++)
		{
		    m_motorPins[i]=(*itr);
		}
	}
  
    m_currentStep=*i_stepperMotor.getCurrentStep();
    m_defaultDirection=i_stepperMotor.getDefaultDirection();
}

const std::vector<int> StepperMotor::getPins() const
{
    std::vector<int> pinVector;
    for(int i=0;i<m_numberOfPins;i++)
	pinVector.push_back(m_motorPins[i]);

    return pinVector;
}

void StepperMotor::setDefaultDirection(const std::string &i_directionString)
{
  if (i_directionString=="CCW")
      m_defaultDirection="CCW";
  else if (i_directionString=="CW")
    m_defaultDirection="CW";
  else
    {
      std::cerr<<"Should be CW or CCW"<<std::endl;
      throw 100;
    }
  
}


void StepperMotor::calculateSecundaryDelay(int &i_primaryDelay,
					   int &i_secundaryDelay)
{
  // throw 100 erro if delay is not bigger then 0
  if(i_primaryDelay<=0)
    {
      LOG_INFO"Primary delay cannot be lower or equal to "<<std::endl;
      throw 100;
    }

  //calculate the secundary delay and change the primary so that the total delay time is equal
  if (i_primaryDelay>(DELAY_BETWEEN_STEP_MICROSECONDS*2))
    i_secundaryDelay=i_primaryDelay/2;
  else
    {
      if(i_primaryDelay<=DELAY_BETWEEN_STEP_MICROSECONDS)
	i_secundaryDelay=0;
      else
	i_secundaryDelay=DELAY_BETWEEN_STEP_MICROSECONDS;
    }
  //now make sure that the original delay is not distored!
  i_primaryDelay-=i_secundaryDelay;
  
  m_calculateSecundaryDelay=true;
}
 

void StepperMotor::calculateSecundaryDelay(std::vector<int> *i_delayVector) 
{
  if(m_calculateSecundaryDelay)
    {
      m_PrimaryDelayVector=(*i_delayVector);
            
      LOG_INFOm_PrimaryDelayVector.size()<<std::endl;
      
      m_SecundaryDelayVector.resize(m_PrimaryDelayVector.size(),0);
      
      for(unsigned int iii=0;iii<m_PrimaryDelayVector.capacity();iii++)
	calculateSecundaryDelay(m_PrimaryDelayVector[iii],m_SecundaryDelayVector[iii]);
      
      m_calculateSecundaryDelay=false;
      LOG_INFO"Done with delay calculation"<<std::endl;
    }
  m_calculateSecundaryDelay=false;
}

const int StepperMotor::getDirectionMultiplier(const std::string &i_direction)
{

  if ((i_direction!="CCW" and i_direction!="CW"))
    {
      LOG_INFO"Unknown direction: "<<i_direction<<std::endl;
      throw 101;
    }
  
  int directionMultiplier;

  if(i_direction==m_defaultDirection)
    directionMultiplier=1;
  else
    directionMultiplier=-1;

  return directionMultiplier;
}

void StepperMotor::updateCurrentStep(const std::string &i_direction)
{

  int directionMultiplier=getDirectionMultiplier(i_direction);
  m_currentStep+=directionMultiplier;

  if(m_currentStep>m_numberOfSteps-1)
    m_currentStep=0;
  
  else if (m_currentStep<0)
    m_currentStep=3;
}

void StepperMotor::moveSteps(int i_numberOfSteps, int i_delay,std::string i_direction) const
{
  int secundaryDelay;
  calculateSecundaryDelay(i_delay,secundaryDelay);
  
  for (int i=0; i < i_numberOfSteps; i++) 
    {
      delayMicroseconds(i_delay);
      //update the current step
      updateCurrentStep(i_direction);
      //set it to that step
      setState(m_stepArray[m_currentStep]);
      delayMicroseconds(secundaryDelay);
    }

}


void StepperMotor::moveSteps(std::vector<int>* i_delayVector,
			     std::vector<std::string>* i_directionVector) const
{
  if(m_calculateSecundaryDelay)
    calculateSecundaryDelay(i_delayVector);
 
  std::vector<std::string>::iterator directionIter=i_directionVector->begin();
  std::vector<int>::iterator secundaryDelayIter=m_SecundaryDelayVector.begin();
  std::vector<int>::iterator primaryDelayIter=m_PrimaryDelayVector.begin();
  
  for (;
	primaryDelayIter!=m_PrimaryDelayVector.end(); 
	primaryDelayIter++,secundaryDelayIter++,directionIter++)
    {
      // setting the current step to the next one time the direction enum, (-1 or 1)
      delayMicroseconds(*primaryDelayIter);
      
      //Given the direction update the current step.
      updateCurrentStep(*directionIter);
      //Set the pins to the current step.
      setState(m_stepArray[m_currentStep]);
      
      delayMicroseconds(*secundaryDelayIter);
    }
  m_calculateSecundaryDelay=true;
}


void StepperMotor::setState(const int i_stateArray[])
{
  for (int i=0; i < m_numberOfPins; i++) 
    setPin(m_motorPins[i],i_stateArray[i]);      
}

//Static const array in which the steps the motor needs to take are defined
const int StepperMotor::m_stepArray[4][4]=					\
  {									\
  {PIN_HIGH, PIN_LOW, PIN_LOW, PIN_LOW},				\
  {PIN_LOW, PIN_HIGH, PIN_LOW, PIN_LOW},				\
  {PIN_LOW, PIN_LOW, PIN_HIGH, PIN_LOW},				\
  {PIN_LOW ,PIN_LOW,PIN_LOW ,PIN_HIGH}					\
};

