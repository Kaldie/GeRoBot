// Copyright [2014] Ruud Cools
#ifndef MOTOR_JOINTCONTROLLER_H_
#define MOTOR_JOINTCONTROLLER_H_

#include <ArduinoMotorDriver.h>
#include <StateSequence.h>
#include <SequenceVector.h>

class JointController {
 private:
  // vector with the joints
  GETSET(JointPointerVector, m_jointPointerVector, JointPointerVector);
  GETSET(ArduinoMotorDriver, m_actuator, Actuator);

  // Vector where all the sequences will be stored in!
  GET(SequenceVector,
      m_sequenceVector,
      SequenceVector);

  int getNumberOfJoints() {return m_jointPointerVector.size();}
  bool validateJoint(const JointPointer&) const;
  bool validateJointVector(const JointPointerVector&) const;
  bool hasJoint(const JointPointer&) const;

 public:
  JointController();
  ~JointController();

  void resetPinStateSequence();

  bool addJoint(const JointPointer&);

  void moveStep(JointPointer& jointPointer,
                const std::string& direction,
                const bool& appendToPrevious);

  void moveSteps(JointPointer& jointPointer,  
                 const std::string& direction,
                 const int& numberOfSteps,
                 const bool i_append = false);

  // Methods to retrieve the joint(s) of a specific type
  JointPointerVector getJoints(const MovementType&);
  JointPointer& getJoint(const MovementType&);

  // Method to the Arduino actuator pointer
  ArduinoMotorDriver* getActuatorPointer() {return &m_actuator;}

  // function which will make the robot move!
  void actuate();
};

#endif  // MOTOR_JOINTCONTROLLER_H_
