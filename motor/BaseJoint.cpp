// Code file for Joint class

#include <macroHeader.h>
#include "BaseJoint.h"


void BaseJoint::setRange(const std::vector<float>& i_range){
  if(i_range.size()!=2)
    LOG_ERROR("Size of range vector should be 2, not : "<<i_range.size());
  
  m_range=i_range;
  
  //first range entry should smaller then the first!
  if (m_range[0]>m_range[1])
    LOG_ERROR("first entry of the range should be smaller then the second!");
}


void BaseJoint::isInRange(float i_attemptedPosition)
{
  const std::vector<float> thisRange=getRange();
  if(i_attemptedPosition>thisRange[1] or i_attemptedPosition<thisRange[0])
    {
      LOG_ERROR("Position is not in the range of the joint: "<<m_movementType<<"\n"
		<<"Intended position is: "<<i_attemptedPosition<<"\n"
		<<"Range: "<<getRange()[0]<<", "<<getRange()[1]);
      throw 100;

    }
}


void BaseJoint::predictStep(Point2D& o_position,
			    const std::string& i_directionString)
{
    predictSteps(o_position,i_directionString,1);
}


const std::string BaseJoint::convertDirection(const std::string i_direction) const
{
    DirectionConversionMap::const_iterator itr = m_directionConversion.find(i_direction);
    if(itr==m_directionConversion.end())
      LOG_ERROR("Could not find a conversion for the direction: "<<i_direction);
    else
      return itr->second;
}


/*
BaseJoint& BaseJoint::operator=(const BaseJoint& rhs)
{
    if(&rhs==this)
	return *this;

    this->m_currentPosition=rhs.m_currentPosition;
    this->m_movementPerStep=rhs.m_movementPerStep;
    std::copy(std::begin(rhs.m_range),std::end(rhs.m_range),std::begin(this->m_range));
    this->m_movementType=rhs.m_movementType;
    this->m_directionConversion=rhs.m_directionConversion;
    return *this;
}
*/

BaseJoint::BaseJoint()
    :m_currentPosition(0),
     m_movementPerStep(0.01),
     m_range{-90.0,90.0},
     m_movementType(None),
     m_directionConversion{{"CCW","CCW"},{"CW","CW"}}
{}


BaseJoint::BaseJoint(float& i_currentPosition,
		     float& i_movementPerStep)
    :m_currentPosition(i_currentPosition),
     m_movementPerStep(i_movementPerStep),
     m_range{-90.0,90.0},
     m_movementType(None),						     
     m_directionConversion{{"CCW","CCW"},{"CW","CW"}}
{}


BaseJoint::BaseJoint(float& i_currentPosition,
		     float& i_movementPerStep,
		     DirectionConversionMap& i_directionConversionMap)
    :m_currentPosition(i_currentPosition),
     m_movementPerStep(i_movementPerStep),
     m_range{-90.0,90.0},
     m_movementType(None),
     m_directionConversion(i_directionConversionMap)
{}
  

BaseJoint::BaseJoint(float& i_currentPosition,
		     float& i_movementPerStep,
		     MovementType& i_movementType,
		     DirectionConversionMap& i_directionConversion)
    :m_currentPosition(i_currentPosition),
     m_movementPerStep(i_movementPerStep),
     m_range{-90.0,90.0},
     m_movementType(None),
     m_directionConversion(i_directionConversion)
{}
		     
     
