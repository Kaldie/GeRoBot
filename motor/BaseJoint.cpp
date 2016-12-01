// Code file for Joint class

#include <macroHeader.h>
#include "BaseJoint.h"
#include "./BaseMotor.h"
#include "./SequenceVector.h"
#include <Point2D.h>

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
  // Find the right conversion
  DirectionConversionMap::const_iterator
      itr = m_directionConversion.find(i_direction);
  // if it is not found
  if (itr == m_directionConversion.end()) {
    LOG_ERROR("Could not find a conversion for the direction: "
              <<i_direction);
  }
  return itr->second;
}


const Point2D BaseJoint::childPosition() const {
  Point2D position(0,0);
  traceType angle = 0.0;
  childPosition(&position, &angle);
  return position;
}


// calculate the child position
void BaseJoint::childPosition(Point2D* o_position, traceType* o_angle) const {
  *o_angle += getAngle();
  if (getLength() != 0) {
    o_position->x += getLength() * cos(*o_angle);
    o_position->y += getLength() * sin(*o_angle);
    LOG_DEBUG("Current angle: " << *o_angle *180/PI << " degree." );
    LOG_DEBUG("Current length: " << getLength());
    LOG_DEBUG("Current child position: " << *o_position);
  }
  if (auto child = m_child.lock()) {
    child->childPosition(o_position, o_angle);
  }
}


void BaseJoint::parentPosition(Point2D* o_position, traceType* o_angle) const {
  if (auto parent = m_parent.lock()) {
    *o_angle += parent->getAngle();
    o_position->x += parent->getLength() * cos(*o_angle);
    o_position->y += parent->getLength() * sin(*o_angle);
    LOG_DEBUG("Current angle: " << *o_angle);
    LOG_DEBUG("Current length: " << getLength());
    LOG_DEBUG("Current child position: " << *o_position);
    parent->parentPosition(o_position, o_angle);
  }
}


void BaseJoint::moveSteps(const std::string& i_directionString,
                          const int& i_numberOfSteps,
                          SequenceVector* i_vector) {
  getMotor()->moveSteps(convertDirection(i_directionString),
                       i_numberOfSteps,
                       i_vector);
  m_currentPosition +=
    getMovementPerStep() * getPositionModifier(i_directionString) * i_numberOfSteps;
}


BaseJoint::BaseJoint()
  :BaseJoint(0, 0, {0, 1}, MovementType::None, {}, WeakJointPointer(), WeakJointPointer()) {
}

BaseJoint::BaseJoint(const traceType& i_currentPosition,
		     const traceType& i_movementPerStep,
		     const MovementType i_type,
		     const DirectionConversionMap& i_conversionMap)
  : BaseJoint(i_currentPosition,
	      i_movementPerStep,
	      {0, 1},
	      i_type,
	      i_conversionMap,
	      WeakJointPointer(),
	      WeakJointPointer()) {
}

BaseJoint::BaseJoint(const traceType& i_currentPosition,
		     const traceType& i_movementPerStep,
		     const std::vector<traceType>& i_rangeVector,
		     const MovementType i_type,
		     const DirectionConversionMap& i_conversionMap,
		     const WeakJointPointer& i_child,
		     const WeakJointPointer& i_parent)
  :m_currentPosition(i_currentPosition),
   m_movementPerStep(i_movementPerStep),
   m_range(i_rangeVector),
   m_movementType(i_type),
   m_directionConversion(i_conversionMap),
   m_child(i_child),
   m_parent(i_parent) {
}
