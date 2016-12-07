// Copyright [2016] Ruud Cools
#ifndef MOTOR_RELATIVESPEEDCONTROLLER_H_
#define MOTOR_RELATIVESPEEDCONTROLLER_H_

#include <SpeedController.h>

class Trace;

// Controller which choses 1 speed for a whole trace
class RelativeSpeedController : public SpeedController {
public:
  /// This method will be called at the moment the robot needs to decied on some speed
  virtual bool adviseSpeed(int* o_speed) const;

  virtual void acknowledgeSpeed(const unsigned int& i_speed,
                                SequenceVector* i_sequenceVector);

  virtual void notifyStep(const BaseJoint::JointPointer& i_joint,
                          const unsigned int& i_numberOfSteps);

  virtual void prepareSpeedController(const BaseJoint::JointPointer& i_joint);

  traceType determineRobotSpeed(const BaseJoint::JointPointer& i_joint) const;

  /// Default constructor
  RelativeSpeedController();

  /// Constructor with a speed defined
  RelativeSpeedController(const float& i_robotSpeed);

  /// Constructor with a speed defined
  RelativeSpeedController(const float& i_robotSpeed, const int& i_vectorPosition);

  virtual ~RelativeSpeedController(){};
 private:
  void calculateMinMaxSpeed(const JointController& i_controller,
			    const Trace& i_trace);

  traceType estimateRobotSpeed(const BaseJoint::JointPointer& i_joint,
			       const int& i_motorSpeed) const;

  bool adviseSpeed(int* i_speed, const int& i_minSpeed, const int& i_maxSpeed) const;
  
  traceType estimateRobotSpeed() const;
  
  void determineMotorSpeed(const BaseJoint::JointPointer&,
			   int* i_minSpeed,
			   int* i_maxSpeed) const;

  int m_minSpeed, m_maxSpeed;

  
};

#endif  // MOTOR_RELATIVESPEEDCONTROLLER_H_
