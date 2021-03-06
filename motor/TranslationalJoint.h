#ifndef TranslationalJoint_H
#define TranslationalJoint_H

#include <macroHeader.h>
#include <BaseJoint.h>
#include <Point2D.h>

template <typename ActuatorType>
class TranslationalJoint: public BaseJoint {
 public:
    // Actual methods!
    virtual void predictSteps(Point2D*,
                              const std::string&,
                              const int&) const;

    virtual traceType distancePerStep() const;

    std::shared_ptr<TranslationalJoint<ActuatorType>> clone() const {
      return std::shared_ptr
          <TranslationalJoint<ActuatorType> >(this->cloneImpl());
    };

    // Constructors
    TranslationalJoint();

  virtual ~TranslationalJoint(){};

 protected:
  virtual TranslationalJoint<ActuatorType>* cloneImpl() const;

  /// return length of the Joint
  virtual traceType getLength() const;

  /// return the angle of the Joint
  virtual traceType getAngle() const;

 private:
  ActuatorType m_actuator;
  virtual int getPositionModifier(const std::string&) const;
};


// Constructors
template <class ActuatorType>TranslationalJoint<ActuatorType>::
TranslationalJoint()
 : BaseJoint(std::shared_ptr<BaseMotor>(new ActuatorType())) {
   setDirectionConversionMap({{"IN", "CCW"}, {"OUT", "CW"}});
   setMovementType(Translational);
}


template <class ActuatorType>
traceType TranslationalJoint<ActuatorType>::getLength() const {
  return m_currentPosition;
}


template <class ActuatorType>
traceType TranslationalJoint<ActuatorType>::getAngle() const {
  return 0;
}



template <class ActuatorType>
int TranslationalJoint<ActuatorType>::
  getPositionModifier(const std::string& i_direction) const {
  if (i_direction == "OUT")
    return 1;
  else if (i_direction == "IN")
    return -1;
  else
    LOG_ERROR("Unknown direction: " << i_direction);
}


template <class ActuatorType>
void TranslationalJoint<ActuatorType>::
predictSteps(Point2D* o_robotPosition,
             const std::string& i_directionString,
             const int& i_numberOfSteps) const {
  //initialise variable
  Point2D relativeRobotPosition(0,0);
  traceType angle(0.0);
  // calculate the relative robot position from this joint
  parentPosition(&relativeRobotPosition, &angle);
  // how much will this joint shift
  traceType addedExtension = getMovementPerStep() *
    getPositionModifier(i_directionString) * i_numberOfSteps;
  // Determine the new relative length of the robot
  double newLength = magnitude(relativeRobotPosition) + addedExtension;
  // Update the current end position of the robot
  o_robotPosition->x += cos(angle) * newLength;
  o_robotPosition->y += sin(angle) * newLength;
  LOG_INFO("Position after translation is: " <<*o_robotPosition);
}


template <class ActuatorType>
traceType TranslationalJoint<ActuatorType>::distancePerStep() const {
  return m_movementPerStep;
}


template <class ActuatorType>
TranslationalJoint<ActuatorType>* TranslationalJoint<ActuatorType>::
  cloneImpl() const {
  TranslationalJoint<ActuatorType>* joint= new TranslationalJoint<ActuatorType>;
  joint->setPosition(this->getPosition());
  joint->setMovementPerStep(this->getMovementPerStep());
  joint->setDirectionConversionMap(this->getDirectionConversionMap());
  //    joint->m_range=this->m_range;
  joint->setRange(this->getRange());
  joint->m_motor = this->m_motor;
  joint->setMovementType(this->getMovementType());
  joint->m_child = this->m_child;
  joint->m_parent = this->m_parent;
  return joint;
}

#endif
