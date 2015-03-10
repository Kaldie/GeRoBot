// Code file for Joint class

#include <macroHeader.h>
#include "BaseJoint.h"


void BaseJoint::setRange(const std::vector<float>& i_range) {
  if (i_range.size() != 2)
    LOG_ERROR("Size of range vector should be 2, not : "
              << i_range.size());
  m_range = i_range;

  // first range entry should smaller then the first!
  if (m_range[0]>m_range[1])
    LOG_ERROR("first entry of the range should be smaller then the second!");
}


void BaseJoint::isInRange(float i_attemptedPosition) {
  const std::vector<float> thisRange = getRange();
  if (i_attemptedPosition > thisRange[1] or
     i_attemptedPosition < thisRange[0]) {
    LOG_ERROR("Position is not in the range of the joint: " <<
              m_movementType <<
              "Intended position is: " << i_attemptedPosition<<
              "Range: " << getRange()[0] << ", " << getRange()[1]);
  }
}


void BaseJoint::predictStep(Point2D& o_position,
                            const std::string& i_directionString) {
  predictSteps(o_position, i_directionString, 1);
}


const std::string BaseJoint::convertDirection(
    const std::string i_direction) const {

  DirectionConversionMap::const_iterator
      itr = m_directionConversion.find(i_direction);

  if (itr == m_directionConversion.end()) {
    std::cout << m_directionConversion.size();
    for (auto directionConversionIter = m_directionConversion.begin();
         directionConversionIter != m_directionConversion.end();
         directionConversionIter++) {
      std::cout << directionConversionIter->first <<
          ", " << directionConversionIter->second << std::endl;
    }
    LOG_ERROR("Could not find a conversion for the direction: "
              <<i_direction);
  } else {
    return itr->second;
  }
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
    : BaseJoint(0,
                0.01,
                {-90.0, 90.0},
                None,
                {
                  {"CCW","CCW"},
                  {"CW","CW"}
                })
{}


BaseJoint::BaseJoint(const float& i_currentPosition,
                     const float& i_movementPerStep)
    :BaseJoint(i_currentPosition,
               i_movementPerStep,
               {-90.0, 90.0},
               None,
               {{"CCW","CCW"}, {"CW","CW"}}
               )
{}


BaseJoint::BaseJoint(const float& i_currentPosition,
                     const float& i_movementPerStep,
                     const DirectionConversionMap& i_directionConversionMap)
    :BaseJoint(i_currentPosition,
               i_movementPerStep,
               {-90.0, 90.0},
               None,
               i_directionConversionMap)
{}


BaseJoint::BaseJoint(const float& i_currentPosition,
                     const float& i_movementPerStep,
                     const std::vector<float> i_range,
                     const MovementType& i_movementType,
                     const DirectionConversionMap& i_directionConversion)
    :m_currentPosition(i_currentPosition),
     m_movementPerStep(i_movementPerStep),
     m_range(i_range),
     m_movementType(None),
     m_directionConversion(i_directionConversion)
{}
