#ifndef Joint_h
#define Joint_h

#include <macroHeader.h>
#include <memory>
#include <vector>
#include <Point2D.h>
#include <BaseMotor.h>

//Type independend code!
//Joint definitions
enum MovementType {None,Rotational,Translational}; 

class BaseJoint; //forward declare the class so we can create a type def JointPointer
typedef std::shared_ptr<BaseJoint> JointPointer;


typedef std::map<std::string,std::string> DirectionConversionMap;

class BaseJoint
{
 private:
    //Pure virtual call!
    virtual int getPositionModifier(const std::string&) const =0;

 protected:
    GETSET(float,m_currentPosition,Position)
    GETSET(double,m_movementPerStep,MovementPerStep)
    GET(std::vector<float>, m_range,Range)
    GETSET(MovementType,m_movementType,MovementType)
    GETSET(DirectionConversionMap,m_directionConversion,DirectionConversionMap)

    virtual BaseJoint* cloneImpl() const = 0;
    void isInRange(float);

 public:

    void setRange(const std::vector<float>&);
    
    const std::string convertDirection(const std::string)const;

    virtual BaseMotor* getMotor() = 0;
    //    virtual void setMotor(const BaseMotor*) = 0;


    //Actual methods!
    virtual void predictStep(Point2D&,
			     const std::string&); 

    virtual void predictSteps(Point2D&,
			      const std::string&,
			      const int&)=0;
  
    std::shared_ptr<BaseJoint> clone() const { return std::shared_ptr<BaseJoint>( this->cloneImpl() ); }
    
    //Constructors
    BaseJoint();
  
    BaseJoint(float&,
	      float&);

    BaseJoint(float&,
	      float&,
	      DirectionConversionMap&);


    BaseJoint(float&,
	      float&,
	      MovementType&,
	      DirectionConversionMap&);
    
    virtual ~BaseJoint(){};
};
#endif
