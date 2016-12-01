// Copyright [2015] Ruud Cools

#ifndef MOTOR_SPEEDCONTROLLER_H_
#define MOTOR_SPEEDCONTROLLER_H_

#include <BaseJoint.h>
#include <SequenceVector.h>
class Trace;
class JointController;
class SequenceVector;

// Interface of speed controllers, there will be some
class SpeedController {
public:
  enum Type {None, Constant, Prescribed, Relative};
  typedef  std::shared_ptr<SpeedController> SpeedControllerPointer;

  GETSET(Type, m_type, Type);
  GETSETPROTECTED(float, m_robotSpeed, RobotSpeed);
  GETSET(int, m_currentSequenceVectorPosition, CurrentSequenceVectorPosition);

  virtual void notifyStep(const BaseJoint::JointPointer& i_joint,
                          const unsigned int& i_numberOfSteps);

  /// This method will be called at the moment the robot needs to decied on some speed
  virtual bool adviseSpeed(int* o_speed) = 0;

  /**
   * This method will be called at the moment the robot deciedes to use the certain speed for the previously notified steps
   */
  virtual void acknowledgeSpeed(const unsigned int& i_speed,
                                SequenceVector* i_sequenceVector);

  /**
   * This is call just before a algorithm determins movement of the robot
   *
   */
  virtual void prepareSpeedController(const Trace& i_trace, const JointController&);

  SpeedController();

  SpeedController(const Type& i_type);
  
  SpeedController(const Type& i_type,
		  float i_robotSpeed);
  
  SpeedController(const Type& i_type,
		  float i_robotSpeed,
		  int i_currentSequenceVectorPosition);
 protected:  
  typedef std::vector<std::tuple<BaseJoint::JointPointer,int>> JointStepVector;
  /**
   * Brief: estimate the number of steps for each joint has to set during to complete the trace
   */
  JointStepVector estimateSteps(const Trace& i_trace, const JointController& i_controller);
};

#endif  // MOTOR_SPEEDCONTROLLER_H_
