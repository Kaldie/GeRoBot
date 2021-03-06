// Copyright [2015] Ruud Cools
#ifndef MOTOR_ROTATIONALJOINT_H_
#define MOTOR_ROTATIONALJOINT_H_

#include <macroHeader.h>
#include <Point2D.h>
#include <BaseJoint.h>

template <class ActuatorType>
class RotationalJoint: public BaseJoint {
  GETSET(traceType, m_length, FixedLength);
 public:
  // Actual methods!
  void predictSteps(Point2D*, const std::string&, const int&) const;

  std::shared_ptr<RotationalJoint<ActuatorType>> clone() const {
    return std::shared_ptr
      <RotationalJoint<ActuatorType> >(this->cloneImpl());
  }

  /// Return the distance set per step
  virtual traceType distancePerStep() const;

  // Constructors
  RotationalJoint();

  virtual ~RotationalJoint(){};

 protected:
  virtual RotationalJoint<ActuatorType>* cloneImpl() const;

  /// return length of the Joint
  virtual traceType getLength() const;

  /// return the angle of the Joint
  virtual traceType getAngle() const;
  
 private:
  virtual int getPositionModifier(const std::string&) const;
};


// Constructors
template <class ActuatorType>
RotationalJoint<ActuatorType>::RotationalJoint()
:BaseJoint(std::shared_ptr<BaseMotor>(new ActuatorType())), m_length(0) {
  setMovementType(Rotational);
}


template <class ActuatorType>
traceType RotationalJoint<ActuatorType>::getLength() const {
  return m_length;
}


template <class ActuatorType>
traceType RotationalJoint<ActuatorType>::getAngle() const {
  return m_currentPosition;
}


template <class ActuatorType>
int RotationalJoint<ActuatorType>::
getPositionModifier(const std::string& i_direction) const {
  if (i_direction == "CCW") {
    return 1;
  } else if (i_direction == "CW") {
    return -1;
  } else  {
    LOG_ERROR("Unknown direction!");
  }
}


template <class ActuatorType>
void RotationalJoint<ActuatorType>::predictSteps(Point2D* o_robotPosition,
                                                 const std::string& i_directionString,
                                                 const int& i_numberOfSteps) const {
  Point2D relativePosition(childPosition());
  Point2D rotationPoint(*o_robotPosition - relativePosition);
  traceType addedRotation = m_movementPerStep *
    getPositionModifier(i_directionString) * i_numberOfSteps;
  // Debug info
  LOG_DEBUG("Position modifier is: "<< getPositionModifier(i_directionString));
  LOG_DEBUG("Movement per step is: " << m_movementPerStep * 180/PI << " degree.");
  LOG_DEBUG("Robot position: " << *o_robotPosition);
  LOG_DEBUG("Child position: " << relativePosition);
  LOG_DEBUG("Rotation position: " << rotationPoint);
  LOG_DEBUG("Added rotation: " << addedRotation);

  // Rotate the robot around the rotation point
  relativePosition.rotate(addedRotation);
  // Update the robot position
  *o_robotPosition = rotationPoint + relativePosition;
  LOG_DEBUG("New position: " << *o_robotPosition);
}


template <class ActuatorType>
traceType RotationalJoint<ActuatorType>::distancePerStep() const {
  return magnitude(childPosition())*(m_movementPerStep);
}


template <class ActuatorType>
RotationalJoint<ActuatorType>*
RotationalJoint<ActuatorType>::cloneImpl() const {
  RotationalJoint<ActuatorType>* joint= new RotationalJoint<ActuatorType>;
  joint->setPosition(this->getPosition());
  joint->setMovementPerStep(this->getMovementPerStep());
  joint->setDirectionConversionMap(this->getDirectionConversionMap());
  //    joint->m_range=this->m_range;
  joint->setRange(this->getRange());
  joint->setMovementType(this->getMovementType());
  joint->m_motor = this->m_motor;
  joint->m_child = this->m_child;
  joint->m_parent = this->m_parent;
  return joint;
}
#endif  // MOTOR_ROTATIONALJOINT_H_
