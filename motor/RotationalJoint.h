// Copyright [2015] Ruud Cools
#ifndef MOTOR_ROTATIONALJOINT_H_
#define MOTOR_ROTATIONALJOINT_H_

#include <macroHeader.h>
#include <Point2D.h>
#include <BaseJoint.h>

template <class ActuatorType>
class RotationalJoint: public BaseJoint {
 private:
    ActuatorType m_actuator;
    virtual int getPositionModifier(const std::string&) const;

 protected:
    virtual RotationalJoint<ActuatorType>* cloneImpl() const;

 public:
    // Can also be used as a "setter (*joint.getMotor())=exampleMotor;
    virtual ActuatorType* getMotor() {return &m_actuator;}

    // Actual methods!
    void predictSteps(Point2D&,
                      const std::string&,
                      const int&);

    std::shared_ptr<RotationalJoint<ActuatorType>> clone() const {
      return std::shared_ptr
          <RotationalJoint<ActuatorType> >(this->cloneImpl());
    };

    // Constructors
    RotationalJoint();

    RotationalJoint(const double& ,
                    const double&);
    
    RotationalJoint(const double& i_currentPosition,
                    const double& i_movementPerStep,
                    ActuatorType& i_actuator);

    RotationalJoint(const double&,
                    const double&,
                    const DirectionConversionMap&,
                    ActuatorType&);

    ~RotationalJoint(){};
};

 
// Constructors
template <class ActuatorType>
RotationalJoint<ActuatorType>::RotationalJoint()
:BaseJoint() {
  setMovementType(Rotational);
}


template <class ActuatorType>
RotationalJoint<ActuatorType>::RotationalJoint(const double& i_currentPosition,
                                               const double& i_movementPerStep)
: BaseJoint(i_currentPosition,
            i_movementPerStep) {
  setMovementType(Rotational);
}


template <class ActuatorType>
RotationalJoint<ActuatorType>::RotationalJoint(const double& i_currentPosition,
                                               const double& i_movementPerStep,
                                               ActuatorType& i_actuator)
: BaseJoint(i_currentPosition,
            i_movementPerStep),
  m_actuator(i_actuator) {
  setMovementType(Rotational);
}


template <class ActuatorType>
RotationalJoint<ActuatorType>::RotationalJoint(const double& i_currentPosition,
                                               const double& i_movementPerStep,
                                               const DirectionConversionMap& i_directionConversionMap,
                                               ActuatorType& i_actuator)
: BaseJoint(i_currentPosition,
            i_movementPerStep,
            i_directionConversionMap),
  m_actuator(i_actuator) {
  setMovementType(Rotational);
}


// Actual methods
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
void RotationalJoint<ActuatorType>::predictSteps(Point2D& o_position,
                                                 const std::string& i_directionString,
                                                 const int& i_numberOfSteps) {
  int positionModifier = getPositionModifier(i_directionString);
  LOG_DEBUG("Position modifier is: "<< positionModifier);
  LOG_DEBUG("Movement per step is: " << getMovementPerStep());
  double newPosition = getPosition() +
      (getMovementPerStep() * positionModifier * i_numberOfSteps);
  LOG_INFO("Current Position: " << getPosition()
           << ", going to position: " << newPosition);
  isInRange(newPosition);
  o_position.rotate(
      getMovementPerStep()*
      (PI/180.0) * static_cast<double>(positionModifier) * i_numberOfSteps);

  setPosition(newPosition);
  LOG_INFO("Position after rotation is: "
           << o_position.x << ", " << o_position.y);
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
  joint->m_actuator = this->m_actuator;
  return joint;
}
#endif  // MOTOR_ROTATIONALJOINT_H_



