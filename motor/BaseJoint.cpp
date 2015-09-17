// Code file for Joint class

#include <macroHeader.h>
#include "BaseJoint.h"
#include "./BaseMotor.h"
#include "./SequenceVector.h"

void BaseJoint::setRange(const std::vector<double>& i_range) {
  if (i_range.size() != 2)
    LOG_ERROR("Size of range vector should be 2, not : "
              << i_range.size());
  // first range entry should smaller then the first!
  if (i_range[0] > i_range[1])
    LOG_ERROR("first entry of the range should be smaller then the second!");
  m_range = i_range;
}


void BaseJoint::isInRange(double i_attemptedPosition) const{
  if (i_attemptedPosition > m_range[1] or
     i_attemptedPosition < m_range[0]) {
    LOG_ERROR("Position is not in the range of the joint: " <<
              m_movementType <<
              "Intended position is: " << i_attemptedPosition<<
              "Range: " << m_range[0] << ", " << m_range[1]);
  }
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


void BaseJoint::moveSteps(const std::string& i_directionString,
                          const int& i_numberOfSteps,
                          SequenceVector* i_vector) {
  int positionModifier = getPositionModifier(i_directionString);
  LOG_DEBUG("Position modifier is: "<< positionModifier);
  LOG_DEBUG("Movement per step is: " << getMovementPerStep());
  setPosition(m_currentPosition +
              (getMovementPerStep() * positionModifier * i_numberOfSteps));
  // tell the actuator to add some steps to sequenceVector
  getMotor()->moveSteps(convertDirection(i_directionString),
                       i_numberOfSteps,
                       i_vector);
}


BaseJoint::BaseJoint()
    : BaseJoint(0,
                0.01,
                {-90.0, 90.0},
                None,
                {{"CCW","CCW"},
                 {"CW","CW"}})
{}


BaseJoint::BaseJoint(const double& i_currentPosition,
                     const double& i_movementPerStep)
    :BaseJoint(i_currentPosition,
               i_movementPerStep,
               {-90.0, 90.0},
               None,
               {{"CCW","CCW"}, {"CW","CW"}}
               )
{}


BaseJoint::BaseJoint(const double& i_currentPosition,
                     const double& i_movementPerStep,
                     const DirectionConversionMap& i_directionConversionMap)
    :BaseJoint(i_currentPosition,
               i_movementPerStep,
               {-90.0, 90.0},
               None,
               i_directionConversionMap)
{}


BaseJoint::BaseJoint(const double& i_currentPosition,
                     const double& i_movementPerStep,
                     const std::vector<double> i_range,
                     const MovementType& i_movementType,
                     const DirectionConversionMap& i_directionConversion)
    :m_currentPosition(i_currentPosition),
     m_movementPerStep(i_movementPerStep),
     m_range(i_range),
     m_movementType(None),
     m_directionConversion(i_directionConversion)
{}
