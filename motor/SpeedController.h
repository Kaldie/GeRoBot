// Copyright [2015] Ruud Cools

#ifndef MOTOR_SPEEDCONTROLLER_H_
#define MOTOR_SPEEDCONTROLLER_H_

#include <SequenceVector.h>
class Trace;
class JointController;
class SequenceVector;
class BaseJoint;
// Interface of speed controllers, there will be some
class SpeedController {
public:
  enum Type {None, Constant, Prescribed, Relative};

  typedef  std::shared_ptr<SpeedController> SpeedControllerPointer;
  /// Defines the type of speed controller
  GETSET(Type, m_type, Type);
  /// Defines the requested speed of the robot in mm/sec
  GETSETPROTECTED(float, m_robotSpeed, RobotSpeed);
  /// Defines the current position the speedcontroller will update the sequence vector to achieve the selected speed
  GETSET(int, m_currentSequenceVectorPosition, CurrentSequenceVectorPosition);

  /// The controller is notified that steps will be taken
  virtual void notifyStep(const std::shared_ptr<BaseJoint>& i_joint,
                          const unsigned int& i_numberOfSteps);

  /// This method will be called at the moment the robot needs to decied on some speed
  virtual bool adviseSpeed(int* o_speed) const = 0;

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

  /**
   * Prepare the controller that steps with this joint will be set
   * it is unknown how many and in which direction
   * if a "Constant" type controller is used, use the current position as a base to calculate the speed
   */
  virtual void prepareSpeedController(const std::shared_ptr<BaseJoint>& i_pointer);

  SpeedController();

  SpeedController(const Type& i_type);
  
  SpeedController(const Type& i_type,
		  const float& i_robotSpeed);
  
  SpeedController(const Type& i_type,
		  const float& i_robotSpeed,
		  const int& i_currentSequenceVectorPosition);
 protected:  
  typedef std::vector<std::tuple<std::shared_ptr<BaseJoint>,int>> JointStepVector;
  /**
   * Brief: estimate the number of steps for each joint has to set during to complete the trace
   */
  JointStepVector estimateSteps(const Trace& i_trace, const JointController& i_controller);
};

#endif  // MOTOR_SPEEDCONTROLLER_H_
