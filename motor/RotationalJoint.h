
#ifndef RotationalJoint_H
#define RotationalJoint_H

#include <iostream>
#include <Point2D.h>
#include <BaseJoint.h>
#include <memory>

template <class ActuatorType>
class RotationalJoint:
public BaseJoint
{
 private:
    ActuatorType m_actuator;
    virtual const int getPositionModifier(const std::string&) const;
    
 protected:
    virtual RotationalJoint<ActuatorType>* cloneImpl() const;

 public:
    //Can also be used as a "setter (*joint.getMotor())=exampleMotor;
    virtual ActuatorType* getMotor(){return &m_actuator;};
    
    // Actual methods!
    void predictSteps(Point2D&,
		      const std::string&,
		      const int&);
  
    std::shared_ptr<RotationalJoint<ActuatorType>> clone() 
	const { return std::shared_ptr<RotationalJoint<ActuatorType>>( this->cloneImpl());};

    // RotationalJoint& operator=(RotationalJoint&) const;
    // Constructors
    RotationalJoint();

    RotationalJoint(float&,
		    float&,
		    ActuatorType&);

    RotationalJoint(float&,
		    float&,
		    DirectionConversionMap&,
		    ActuatorType&);

    ~RotationalJoint(){};
};


//Constructors
template <class ActuatorType>
RotationalJoint<ActuatorType>::RotationalJoint()
:BaseJoint()
{
    setMovementType(Rotational);
}


template <class ActuatorType>
RotationalJoint<ActuatorType>::RotationalJoint(float& i_currentPosition,
					       float& i_movementPerStep,
					       ActuatorType& i_actuator)
:
BaseJoint(i_currentPosition,
	  i_movementPerStep),
    m_actuator(i_actuator)    
{
    setMovementType(Rotational);
}


template <class ActuatorType>
RotationalJoint<ActuatorType>::RotationalJoint(float& i_currentPosition,
					       float& i_movementPerStep,
					       DirectionConversionMap& i_directionConversionMap,
					       ActuatorType& i_actuator)
:
BaseJoint(i_currentPosition,
	  i_movementPerStep,
	  i_directionConversionMap),
    m_actuator(i_actuator)    
{
    setMovementType(Rotational);
}
  

//Actual methods
template <class ActuatorType>
const int RotationalJoint<ActuatorType>::getPositionModifier(const std::string& i_direction) const
{
    if(i_direction=="CCW")
	return 1;
    else if(i_direction=="CW")
	return -1;
    else
	{
	    std::cerr<<"Unknown direction!\n";
	    throw 101;
	}
}


template <class ActuatorType>
void RotationalJoint<ActuatorType>::predictSteps(Point2D& o_position,
						 const std::string& i_directionString,
						 const int& i_numberOfSteps)
{
    int positionModifier=getPositionModifier(i_directionString);
    double newPosition=getPosition()+(getMovementPerStep()*positionModifier*i_numberOfSteps);
    LOG_INFO("Current Position: "<<getPosition()<<", going to position: "<<newPosition);
    isInRange(newPosition);
    o_position.rotate(getMovementPerStep()*(PI/180.0)*double(positionModifier)*double(i_numberOfSteps));
    setPosition(newPosition);
    LOG_INFO("Position after rotation is: "<<o_position.x<<", "<<o_position.y);
}


template <class ActuatorType>
RotationalJoint<ActuatorType>* RotationalJoint<ActuatorType>::cloneImpl() const 
{
    RotationalJoint<ActuatorType>* joint= new RotationalJoint<ActuatorType>;

    joint->setPosition(this->getPosition());
    joint->setMovementPerStep(this->getMovementPerStep());
    joint->setDirectionConversionMap(this->getDirectionConversionMap());
    //    joint->m_range=this->m_range;
    joint->setRange(this->getRange());
    joint->setMovementType(this->getMovementType());
    joint->m_actuator=this->m_actuator;
    return joint;
}

/*
template <class ActuatorType>
RotationalJoint<ActuatorType>& RotationalJoint<ActuatorType>::operator=(RotationalJoint<ActuatorType>& rhs)const
{
    BaseJoint::operator=(rhs);
    this->m_actuator=rhs.m_actuator;
    return  *this;
}
*/    
#endif



