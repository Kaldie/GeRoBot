// Copyright [2015] Ruud Cools

#ifndef MOTOR_SPEEDCONTROLLER_H_
#define MOTOR_SPEEDCONTROLLER_H_

#include <SequenceVector.h>
class Trace;
class JointController;
class SequenceVector;
class BaseJoint;
class MovementRegistrator;
// Interface of speed controllers, there will be some
class SpeedController {
public:
  enum Type {None, Constant, Prescribed, Relative, Manual};

  typedef  std::shared_ptr<SpeedController> SpeedControllerPointer;
  /// Defines the type of speed controller
  GETSET(Type, m_type, Type);

  /// Defines the requested speed of the robot in mm/sec
  GETSETPROTECTED(float, m_robotSpeed, RobotSpeed);

  /// Defines the current position the speedcontroller will update the sequence vector to achieve the selected speed
  GETSET(int, m_currentSequenceVectorPosition, CurrentSequenceVectorPosition);

  /// The registrator that will be called when the robot will move
  GETSETPROTECTED(std::shared_ptr<MovementRegistrator>, m_movementRegistrator, MovementRegistrator);

  /// get the movement registrator
  virtual std::weak_ptr<MovementRegistrator> getRegistrator() const;

  /// This method will be called at the moment the robot needs to decied on some speed
  virtual bool adviseSpeed(int* o_speed) const = 0;

  /**
   * This method will be called at the moment the robot deciedes to use the certain speed for the previously notified steps
   */
  virtual void acknowledgeSpeed(const unsigned int& i_speed,
                                SequenceVector* i_sequenceVector);
  
  SpeedController(const Type& i_type);
  
  SpeedController(const Type& i_type,
		  const float& i_robotSpeed);
  
  SpeedController(const Type& i_type,
		  const float& i_robotSpeed,
		  const int& i_currentSequenceVectorPosition);

 private:
  SpeedController();
};

#endif  // MOTOR_SPEEDCONTROLLER_H_
