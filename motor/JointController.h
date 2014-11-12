#ifndef JointController_H
#define JointController_H

#include <ArduinoMotorDriver.h>
#include <PinState.h>

class JointController
{
 private:
    //vector with the joints
    GETSET(JointPointerVector,m_jointPointerVector,JointPointerVector);

    //points to the actuator on the heap
    GETSET(ArduinoMotorDriver,m_actuator,Actuator);

    //Vector which all the pin settins are given!
    GETSET(PinStateSequence,m_pinStateSequence,PinStateSequence);

		int getNumberOfJoints(){return m_jointPointerVector.size();};

    bool validateJoint(const JointPointer&) const;

    bool validateJointVector(const JointPointerVector&) const;

    bool hasJoint(const JointPointer&) const;

    void addEmptyPinStatesToSequence(const int&);

    void addStepToPrevious(const JointPointer&, const std::string&);

    void appendPinStateSequence(const PinStateSequence&,
																const bool&);

 public:
    JointController();
    ~JointController();

    void resetPinStateSequence();

    void addJoint(const JointPointer&);

    void moveStep(JointPointer& jointPointer,
									const std::string& direction, 
									const bool& appendToPrevious);
		
		
    void moveSteps(JointPointer& jointPointer,  
									 const std::string& direction,
									 const int& numberOfSteps);   
    
		
    //Methods to retrieve the joint(s) of a specific type
    JointPointerVector getJoints(const MovementType&);
    JointPointer& getJoint(const MovementType&);

		//Method to the Arduino actuator pointer
		ArduinoMotorDriver* getActuatorPointer(){return &m_actuator;};
    
    //no function yet
    void actuate();



};

#endif // JointController
