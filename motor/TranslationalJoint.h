#ifndef TranslationalJoint_H
#define TranslationalJoint_H

#include <macroHeader.h>
#include <BaseJoint.h>
#include <Point2D.h>

template <typename ActuatorType>
class TranslationalJoint: public BaseJoint
{
 private:
    ActuatorType m_actuator;
    virtual int getPositionModifier(const std::string&) const;

 protected:
    virtual TranslationalJoint<ActuatorType>* cloneImpl() const;

 public:
    // Actual methods!
    virtual void predictSteps(Point2D&,
                              const std::string&,
                              const int&);

    virtual ActuatorType* getMotor() {return &m_actuator;};

    std::shared_ptr<TranslationalJoint<ActuatorType>> clone() const {
      return std::shared_ptr
          <TranslationalJoint<ActuatorType> >(this->cloneImpl());
    };

    // Constructors
    TranslationalJoint();

    TranslationalJoint(const float&,
                       const float&);

    TranslationalJoint(const float&,
                       const float&,
                       const ActuatorType&);

    TranslationalJoint(const float&,
                       const float&,
                       const DirectionConversionMap&,
                       const ActuatorType&);

  virtual ~TranslationalJoint() {};
};


// Constructors
template <class ActuatorType>
TranslationalJoint<ActuatorType>::TranslationalJoint()
:BaseJoint() {
  setDirectionConversionMap({{"IN", "CCW"}, {"OUT", "CW"}});
  setMovementType(Translational);
}


template <class ActuatorType>
TranslationalJoint<ActuatorType>::
  TranslationalJoint(const float& i_currentPosition,
                     const float& i_movementPerStep)
: BaseJoint(i_currentPosition,
            i_movementPerStep,
            {{"IN", "CCW"}, {"OUT", "CW"}}) {
  setMovementType(Translational);
}


template <class ActuatorType>
TranslationalJoint<ActuatorType>::
  TranslationalJoint(const float& i_currentPosition,
                     const float& i_movementPerStep,
                     const ActuatorType& i_actuator)
: BaseJoint(i_currentPosition,
            i_movementPerStep,
            {{"IN", "CCW"}, {"OUT", "CW"}}),
  m_actuator(i_actuator) {
  setMovementType(Translational);
}

template <class ActuatorType>
TranslationalJoint<ActuatorType>::
  TranslationalJoint(const float& i_currentPosition,
                     const float& i_movementPerStep,
                     const DirectionConversionMap& i_directionConversionMap,
                     const ActuatorType& i_actuator)
: BaseJoint(i_currentPosition,
            i_movementPerStep,
            i_directionConversionMap),
  m_actuator(i_actuator) {
  setMovementType(Translational);
}

// Actual methods
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
  predictSteps(Point2D& o_position,
               const std::string& i_directionString,
               const int& i_numberOfSteps) {
  double currentAngle = o_position.getAlpha();
  double newLength = Magnitude(o_position)+getMovementPerStep() *
      getPositionModifier(i_directionString) * i_numberOfSteps;
  o_position.x = cos(currentAngle)*newLength;
  o_position.y = sin(currentAngle)*newLength;
  LOG_INFO("Position after translation is: "
           <<o_position.x << ", " << o_position.y);
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
  joint->setMovementType(this->getMovementType());
  joint->m_actuator = this->m_actuator;

  LOG_INFO("movement per step 1: " << joint->getMovementPerStep());
  LOG_INFO("movement per step 1: " << this->getMovementPerStep());

  return joint;
}

#endif
