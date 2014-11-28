#include <iostream>
#include <cmath>
#include <Vector2D.h>
#include <Point2D.h>
#include "Robot.h"
#include "DelayList.h"
#include "TraceCalculator.h"


TraceCalculator::TraceCalculator(Robot &i_robot):
    BaseTraceCalculator(i_robot,0.1)
{}

  
TraceCalculator::TraceCalculator(Robot& i_robot,
				 const float& i_tolerance):
    BaseTraceCalculator(i_robot,i_tolerance)
{}
  

//assign operator
TraceCalculator::TraceCalculator(const TraceCalculator& obj)
{
  LOG_INFO"Copy constructor"<<std::endl;
  setRobot(obj.m_robot);
  setTolerance(obj.m_tolerance);
}


void TraceCalculator::determineDelayLists(Trace* i_trace)
{

  Point2D requestedPoint2D;
  Vector2D requestedVector2D;

  LOG_INFO"getting delay lists!"<<std::endl;
  DelayList* rotationJointDelayList=m_robot->getJoint(Rotational)->getDelayList();
  DelayList* translationJointDelayList=m_robot->getJoint(Translational)->getDelayList();

  int rotationDelayListLength,translationDelayListLength;
  setRotationalDelayVectorLength(i_trace,
				 rotationDelayListLength,
				 translationDelayListLength);

  rotationJointDelayList->updateDelayListLength(rotationDelayListLength);
  translationJointDelayList->updateDelayListLength(translationDelayListLength);
  
  
  Line2D traceLine;
  if (i_trace->getType() == Line)
    { //Need a single straight trace line for line traces, 
      //A dynamic trace line will be calculated for arcs.
      traceLine=i_trace->getTraceLine();
    }

  else
    {
      LOG_INFO"None translational traces are currently not supported!"<<std::endl;
      throw 100;
    }

  int i=0;
  do
    {
      //resetting the hasrotated and has translated
      prepareNextStep();

      LOG_INFO"Current position is: "<<m_robot->getPosition().x<<", "<< m_robot->getPosition().y<<std::endl;
      calculateStep(rotationJointDelayList,translationJointDelayList);
      i++;
    }while(!i_trace->isWithinEndRange(m_robot->getPosition()) and hasStepped());
  

  //setting the smallest value to the predifined value...lala...something
  DelayList::updateListsToMinimumDelay(translationJointDelayList,
				       rotationJointDelayList,
				       DELAY_BETWEEN_STEP_MICROSECONDS);
  LOG_INFO"YOLO2"<<rotationJointDelayList->getDelayVector()->size()<<std::endl;
}


void TraceCalculator::prepareNextStep()
{

  m_currentTranslationDelay+=m_defaultDelay;
  m_currentRotationDelay+=m_defaultDelay;

  if(m_hasRotated)
    m_currentRotationDelay=0;
  if(m_hasTranslated)
    m_currentTranslationDelay=0;

  m_hasRotated=false;
  m_hasTranslated=false;

}

void TraceCalculator::calculateStep(DelayList* i_rotationDelayList,
				     DelayList* i_translationDelayList)
{
  //keeping track if the robot is orded to translate or rotated
  
  //should rotate?
  if(m_robot->getTrace().shouldRotate(m_robot->getPosition()))
    {
      //add rotation to the delay list and correct translations
      prepareRotation(i_rotationDelayList,
		      i_translationDelayList);
    }
  else if(m_robot->getTrace().shouldTranslate(m_robot->getPosition()))
    {
      prepareTranslation(i_translationDelayList,
			 i_rotationDelayList);
    
    }
}
  

void TraceCalculator::prepareRotation(DelayList *i_rotationJointDelayList,
				      DelayList* i_translationJointDelayList)
{
  //Rotating
  std::string direction=m_robot->getTrace().getRotationDirectionToEndPoint(m_robot->getPosition());
  m_robot->getJoint(Rotational)->predictStep(m_robot->getPosition(),direction);
  m_hasRotated=true;
  LOG_INFO"Rotating: "<<direction<<std::endl;

  //Correct the rotation
  int numberOfCorrectionSteps=correctRotation(i_translationJointDelayList);

  /*
    Figure out how manny heartbeats/ minimum delays you need to wait before doing this step
    Meaning that this step has to be taking after all the correction steps has been taken
    This assumes that given all the interatia...stuff smooth out!
  */
  int atHeartBeat;
  if(numberOfCorrectionSteps!=0)
    {
      atHeartBeat=numberOfCorrectionSteps;
      m_hasTranslated=true;
    }
  else
    {
      atHeartBeat=1;
      m_hasTranslated=false;
    }

  i_rotationJointDelayList->add(m_currentRotationDelay+m_defaultDelay*atHeartBeat,direction);
  LOG_INFO"Size: "<<i_rotationJointDelayList->getDelayVector()->size()<<std::endl;
}


void TraceCalculator::prepareTranslation(DelayList *i_translationDelayList,
					 DelayList *i_rotationDelayList)
{
  //identify the direction to move to
  std::string direction=m_robot->getTrace().getTranslationDirectionToEndPoint(m_robot->getPosition());
  
  //Predict the step
  m_robot->getJoint(Translational)->predictStep(m_robot->getPosition(),direction);
  LOG_INFO"Translating: "<<direction<<std::endl;
  m_hasTranslated=true;
  //Correct the rotation
  int numberOfCorrectionSteps=correctTranslation(i_rotationDelayList);

  /*
    Figure out how manny heartbeats/ minimum delays you need to wait before doing this step
    Meaning that this step has to be taking after all the correction steps has been taken
    This assumes that given all the interatia...stuff smooth out!
  */
  int numberOfHeartBeats;
  if(numberOfCorrectionSteps!=0)
    {
      numberOfHeartBeats=numberOfCorrectionSteps;
      m_hasRotated=true;
    }
  else
    {
      numberOfHeartBeats=1;
      m_hasRotated=false;
    }

  i_translationDelayList->add(m_currentTranslationDelay+m_defaultDelay*numberOfHeartBeats,direction);

}


int TraceCalculator::correctRotation(DelayList *i_translationDelayList)
{
  float movementPerStep=m_robot->getJoint(Translational)->getMovementPerStep();

  Point2D destinationPoint=m_robot->getTrace().getTraceLine().getIntersectingPoint(m_robot->getPosition());
  float jointPositionDifference=Magnitude(destinationPoint)-Magnitude(m_robot->getPosition());
  int numberOfSteps=std::floor(std::abs(jointPositionDifference)/movementPerStep);
  
  //going to figure out if doing 1 more step creates a beter agreement
  float translationDistance=numberOfSteps*movementPerStep;

  if (std::abs(jointPositionDifference-translationDistance)>(movementPerStep/2.))
      numberOfSteps+=1;

  if (numberOfSteps>0)
    {
      std::string translationDirection=m_robot->getTrace().getTranslationDirection(m_robot->getPosition(),
									      destinationPoint);
      for(int i=0;i<numberOfSteps;i++)
	{
	  i_translationDelayList->add(m_defaultDelay+m_currentTranslationDelay,translationDirection);
	  m_robot->getJoint(Translational)->predictStep(m_robot->getPosition(),translationDirection);
      
	  jointPositionDifference=Magnitude(destinationPoint)-Magnitude(m_robot->getPosition());
	  LOG_INFO"After translation:\tJoint position difference: "<<jointPositionDifference<<std::endl;
	  m_currentTranslationDelay=0;
	}
      return numberOfSteps;
    }
  
  else
    {
      return 0;
    }
}


int TraceCalculator::correctTranslation(DelayList* i_rotationDelayList)
{
  float currentRotation=m_robot->getPosition().getAlpha();

  Point2D destinationPoint=m_robot->getTrace().getTraceLine().getIntersectingPoint(m_robot->getPosition());
  if (destinationPoint==Point2D(0,0))
	  return 0;

  LOG_INFO"Distination point: "<<destinationPoint.x<<", "<<destinationPoint.y<<std::endl; 
  LOG_INFO"Current robot rotation: "<<currentRotation*(180.0/PI)<<std::endl;
  LOG_INFO"DistinationPoint rotation: "<<destinationPoint.getAlpha()*(180.0/PI)<<std::endl;
  float distinationPointRotation=destinationPoint.getAlpha();

  float jointAngleDifference=currentRotation-distinationPointRotation;

  LOG_INFO"Joint angle difference: "<<jointAngleDifference*(PI/180)<<std::endl;

  int numberOfSteps=std::floor(std::abs(jointAngleDifference)/m_robot->getJoint(Rotational)->getMovementPerStep()*(PI/180.0));

  if (numberOfSteps>0)
    {
      std::string rotationDirection=m_robot->getTrace().getRotationDirection(m_robot->getPosition(),destinationPoint);
      LOG_INFO"Rotation direction: "<<rotationDirection<<std::endl;
      for(int i=0;i<numberOfSteps;i++)
	{
	  numberOfSteps=m_robot->getJoint(Translational)->getMovementPerStep();
	  i_rotationDelayList->add(m_defaultDelay+m_currentRotationDelay,rotationDirection);
	  m_robot->getJoint(Translational)->predictStep(m_robot->getPosition(),rotationDirection);
	  m_currentRotationDelay=0;
	}
      return numberOfSteps;
    }
  else
    {
      return 0;
    }
}

bool TraceCalculator::hasStepped()
{
  return (m_hasTranslated or m_hasRotated);
}

