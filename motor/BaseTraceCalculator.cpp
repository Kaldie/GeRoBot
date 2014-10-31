#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <Vector2D.h>
#include <Point2D.h>
#include "BaseTraceCalculator.h"


BaseTraceCalculator::BaseTraceCalculator():
    m_tolerance(0.01),
    m_jointController(nullptr),
    m_translationTolerance(0.01),
    m_rotationTolerance(0.01),
    m_writeLog(false),
    m_logFileName("./stepLog.log")
{}

BaseTraceCalculator::BaseTraceCalculator(JointController* i_jointController):
    m_tolerance(0.01),
    m_jointController(i_jointController),
    m_translationTolerance(0.01),
    m_rotationTolerance(0.01),
    m_writeLog(false),
    m_logFileName("./stepLog.log")
{
    setTolerances();
}

  
BaseTraceCalculator::BaseTraceCalculator(JointController* i_jointController,
					 const float& i_tolerance):
    m_tolerance(i_tolerance),
    m_jointController(i_jointController),
    m_translationTolerance(0.01),
    m_rotationTolerance(0.01),
    m_writeLog(false),
    m_logFileName("./stepLog.log")
{
    setTolerances();
}
  

//assign operator
BaseTraceCalculator::BaseTraceCalculator(const BaseTraceCalculator& obj)
{
    LOG_INFO("Copy constructor");
    m_jointController=obj.m_jointController;
    m_tolerance=obj.m_tolerance;
    m_writeLog=obj.m_writeLog;
    m_logFileName=obj.m_logFileName;
    m_rotationTolerance=obj.m_rotationTolerance;
    m_translationTolerance=obj.m_translationTolerance;
}


const bool BaseTraceCalculator::hasJointController()
{
    if(m_jointController==nullptr)
	return false;
    else
	return true;
}

void BaseTraceCalculator::setTolerances()
{
    if(hasJointController())
	{
	    try
		{m_rotationTolerance=m_jointController->getJoint(Rotational)->getMovementPerStep();}
	    catch(int e)
		{LOG_INFO("Could not find a rotaional joint, tolerance is left at default!");}

	    try
		{m_translationTolerance=m_jointController->getJoint(Translational)->getMovementPerStep();}
	    catch(int e)
		{LOG_INFO("Could not find a translational joint, tolerance is left at default!");}
	}
}


const std::vector<int> BaseTraceCalculator::getNumberOfSteps(const Trace& i_trace,
							     const Point2D& i_position) const
{
    Point2D endPoint=i_trace.getEndPosition();

    LOG_INFO("Translational movement per step: "<<
	      (float)m_jointController->getJoint(Translational)->getMovementPerStep());

    LOG_INFO("Rotationalal movement per step: "<<
	      m_jointController->getJoint(Rotational)->getMovementPerStep());
    
    //Magnitude difference / movement per step of Translational joint
    int numberOfTranslationSteps=std::abs(Magnitude(i_position)-Magnitude(endPoint))/
	(m_jointController->getJoint(Translational)->getMovementPerStep());

    numberOfTranslationSteps*=m_jointController->getJoint(Translational)->getMotor()->numberOfStatesPerStep();
    
    //Rotational difference / movement per step of Rotational joint
    int numberOfRotationSteps=(std::abs(i_position.getAlpha()-endPoint.getAlpha())*(180/PI))/
	(m_jointController->getJoint(Rotational)->getMovementPerStep());

    numberOfRotationSteps*=m_jointController->getJoint(Translational)->getMotor()->numberOfStatesPerStep();
    
    return std::vector<int>{numberOfRotationSteps,numberOfTranslationSteps};
}


void BaseTraceCalculator::writeToStepLog(const std::string i_direction,
					 const int i_numberOfSteps) const{
  std::ofstream stepLogFile(m_logFileName,std::ios::app);
  if (stepLogFile.good())
    stepLogFile<<i_direction<<", "<<i_numberOfSteps<<std::endl;
}

const bool BaseTraceCalculator::shouldTranslate(const Trace& i_trace,
					                                    	const Point2D &i_point2D) const{
  float pointMagnitude=Magnitude(i_point2D);
  float endPositionMagnitude=Magnitude(i_trace.getEndPosition());
  float difference=std::abs(pointMagnitude-endPositionMagnitude);
  
  bool shouldTranslate;
  
  if(difference>m_translationTolerance)
    shouldTranslate = true;
  
  else if(difference>(getJointController()->getJoint(Translational)->getMovementPerStep() / 2.0))
    shouldTranslate=true;
  
  else
    shouldTranslate = false;

  if(!shouldTranslate){
	LOG_INFO("Not translating!! ");
	LOG_INFO("Current magnitude: "<<pointMagnitude<<". ");
	LOG_INFO("Wanted magnitude: "<<endPositionMagnitude);
	LOG_INFO("diff: "<<std::abs(pointMagnitude-endPositionMagnitude));
  }

  return shouldTranslate;
}

const bool BaseTraceCalculator::shouldRotate(const Trace& i_trace,
					                                   const Point2D &i_point2D) const{
  float pointAngle=i_point2D.getAlpha()*180/PI;
  float endPositionAngle=i_trace.getEndPosition().getAlpha()*180/PI;
  float difference=std::abs(pointAngle-endPositionAngle);

  bool shouldRotate;
  
  if(difference>m_rotationTolerance)
      shouldRotate=true;
  
  else if(difference>(getJointController()->getJoint(Rotational)->getMovementPerStep()/2.0))
    shouldRotate=true;
  
  else
    shouldRotate=false;
    
  if(!shouldRotate){
    LOG_INFO("Not rotating!!");
    LOG_INFO("current angle: "<<pointAngle<<" ");
    LOG_INFO("wanted angle: "<<endPositionAngle);
    LOG_INFO("diff: "<<difference);
  }
    
    return shouldRotate;
}
	    

void BaseTraceCalculator::calculateTrace(const Trace* i_trace,
					 Point2D& i_startPoint)
{
    LOG_INFO("Current robot position: "<<i_startPoint.x<<", "<<i_startPoint.y);
    LOG_INFO("Going to position: "<<i_trace->getEndPosition().x<<", "<<i_trace->getEndPosition().y);

    std::string rotationDirection=i_trace->getRotationDirectionToEndPoint(i_startPoint);
    std::string translationDirection=i_trace->getTranslationDirectionToEndPoint(i_startPoint);

    std::vector<int> numberOfSteps=getNumberOfSteps(*i_trace,i_startPoint);
    LOG_INFO("\nNumber of translation steps: "<<numberOfSteps[0]<<"\n"
	     "Number of rotation steps: "<<numberOfSteps[1]);

    std::vector<int>::iterator maximumNumberOfSteps=std::max_element(numberOfSteps.begin(),
								     numberOfSteps.end());

    PinStateSequence pinStateSequence=getJointController()->getPinStateSequence();
    pinStateSequence.reserve(pinStateSequence.size()+(*maximumNumberOfSteps)+5);

    getJointController()->moveSteps(getJointController()->getJoint(Rotational),
				    rotationDirection,		
				    numberOfSteps[0]);
		

    getJointController()->moveSteps(getJointController()->getJoint(Translational),
				    translationDirection,
				    numberOfSteps[1]);

    /*
      int numberOfTranslationalSteps, numberOfRotationalSteps;
      getRotationalDelayVectorLength(i_trace,
      numberOfRotationalSteps,
      numberOfTranslationalSteps);

      //create new rotational delay vector
      std::vector<int> rotationalDelayVector(numberOfRotationalSteps,m_defaultDelay);
      std::vector<std::string> rotationalDirectionVector(numberOfRotationalSteps,rotationDirection);

      DelayList newRotationalDelayList(&rotationalDelayVector,
      &rotationalDirectionVector);

      //create new translational delay vector
      std::vector<int> translationalDelayVector(numberOfTranslationalSteps,m_defaultDelay);
      std::vector<std::string> translationalDirectionVector(numberOfTranslationalSteps,translationDirection);
      DelayList newTranslationalDelayList(&translationalDelayVector,
      &translationalDirectionVector);


      //override the delaylist to the new ones and be done with it!
      m_jointController->getJoint(Rotational)->setDelayList(newRotationalDelayList);
      m_jointController->getJoint(Translational)->setDelayList(newTranslationalDelayList);
    */
}

