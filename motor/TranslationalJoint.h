#ifndef TranslationalJoint_H
#define TranslationalJoint_H

#include <BaseJoint.h>

template <typename ActuatorType>
class TranslationalJoint: public BaseJoint
{
 private:
    ActuatorType m_actuator;
    virtual const int getPositionModifier(const std::string&) const;

 protected:
    virtual TranslationalJoint<ActuatorType>* cloneImpl() const;

 public:
    //Actual methods!
    virtual void predictSteps(Point2D&,
			    const std::string&,
			    const int&);
    
    virtual ActuatorType* getMotor(){return &m_actuator;};
    
    //virtual void setMotor(const ActuatorType* i_motor){m_actuator=(*i_motor);};





    std::shared_ptr<TranslationalJoint<ActuatorType>> clone() 
	const { return std::shared_ptr<TranslationalJoint<ActuatorType>>( this->cloneImpl() ); };

    //    TranslationalJoint& operator=(TranslationalJoint&) const;
    
    //Constructors
    TranslationalJoint();
    
    TranslationalJoint(float&,
		       float&,
		       ActuatorType&);

    TranslationalJoint(float&,
		       float&,
		       DirectionConversionMap&,
		       ActuatorType&);

  virtual ~TranslationalJoint(){};
};


//Constructors
template <class ActuatorType>
TranslationalJoint<ActuatorType>::TranslationalJoint()
:BaseJoint()
{
    setMovementType(Translational);
}


template <class ActuatorType>
TranslationalJoint<ActuatorType>::TranslationalJoint(float& i_currentPosition,
						     float& i_movementPerStep,
						     ActuatorType& i_actuator)
:
BaseJoint(i_currentPosition,
	  i_movementPerStep),
    m_actuator(i_actuator)    
{
    setMovementType(Translational);
}

template <class ActuatorType>
TranslationalJoint<ActuatorType>::TranslationalJoint(float& i_currentPosition,
						     float& i_movementPerStep,
						     DirectionConversionMap& i_directionConversionMap,
						     ActuatorType& i_actuator)
:
BaseJoint(i_currentPosition,
	  i_movementPerStep,
	  i_directionConversionMap),
    m_actuator(i_actuator)    
{
    setMovementType(Translational);
}

//Actual methods
template <class ActuatorType>
const int TranslationalJoint<ActuatorType>::getPositionModifier(const std::string& i_direction) const
{
    if(i_direction=="OUT")
	return 1;
    else if(i_direction=="IN")
	return -1;
    else
	{
	    std::cerr<<"Unknown direction!\n";
	    throw 101;
	}
}


template <class ActuatorType>
void TranslationalJoint<ActuatorType>::predictSteps(Point2D& o_position,
						    const std::string& i_directionString,
						    const int& i_numberOfSteps)
{
  double currentAngle=o_position.getAlpha();
  double newLength=Magnitude(o_position)+getMovementPerStep()*
    getPositionModifier(i_directionString)*i_numberOfSteps;  
  
  o_position.x=cos(currentAngle)*newLength;
  o_position.y=sin(currentAngle)*newLength;
  LOG_INFO("Position after translation is: "<<o_position.x<<", "<<o_position.y);
}


template <class ActuatorType>
TranslationalJoint<ActuatorType>* TranslationalJoint<ActuatorType>::cloneImpl() const 
{

    TranslationalJoint<ActuatorType>* joint= new TranslationalJoint<ActuatorType>;

    joint->setPosition(this->getPosition());
    joint->setMovementPerStep(this->getMovementPerStep());
    joint->setDirectionConversionMap(this->getDirectionConversionMap());
    //    joint->m_range=this->m_range;
    joint->setRange(this->getRange());
    joint->setMovementType(this->getMovementType());
    joint->m_actuator=this->m_actuator;

    LOG_INFO("movement per step 1: "<<joint->getMovementPerStep());
    LOG_INFO("movement per step 1: "<<this->getMovementPerStep());

    return joint;
}

#endif
