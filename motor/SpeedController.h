// Copyright [2015] Ruud Cools

#ifndef MOTOR_SPEEDCONTROLLER_H_
#define MOTOR_SPEEDCONTROLLER_H_

#include "./BaseJoint.h"
class JointController;
class StateSequence;

class SpeedController {
  /// Current speed of the joints
  typedef std::map<BaseJoint::WeakJointPointer, int,
    std::owner_less<BaseJoint::WeakJointPointer>> JointMap;

  /// shared pointer of the class
  typedef std::shared_ptr<SpeedController> SpeedControllerPointer;

  /// Mapping the number of consecutive steps with the joint
  GETSET(JointMap, m_stepMap, StepMap);

  /// Required speed with which the robot should travel
  GETSET(int, m_motorSpeed, MotorSpeed);

  /// Current speed
  GETSET(traceType , m_robotSpeed, RobotSpeed);

  /**
   * Notify the controller that steps have been taken
   * @param [in] i_joint the joint that has taken the step
   * @param [in] i_direction the direction it has been taken in
   * @param [in] i_numberOfSteps the number of steps
   */
  void notifyStep(const BaseJoint::JointPointer& i_joint,
                  const int& i_numberOfSteps);

  /// Request speed
  bool adviseSpeed(int* o_speed) const;

  /**
   * Let the controller know a speed is selected
   * The controller will hanlde setting the speeds to the joint actuators
   */
  void acknowledgeSpeed(const int& i_speed);

  /**
   * Given the steps in the stepmap and there corrisponding motor speeds
   * give an approximation of the robot speed
   */
  traceType approximateRobotSpeed() const {return getRobotSpeed(m_motorSpeed);}

  /// easy constructor
  SpeedController();

  /// Fully fledged
  SpeedController(const int& i_speed);

 private:
  typedef BaseJoint::WeakJointPointer WeakJoint;
  typedef std::vector<WeakJoint> WeakJointVector;
  void updateStepMap(const WeakJoint& i_joint,
                            const int& i_numberOfSteps);

  void updateSpeedMap();

  int getMaximumConsecutiveSteps() const;
  /**
   * Get the limiting speed based on the active joints
   * Warning c++ magic. Too lazy to write 2 of the same functions
   * @param[in] getLimitingSpeed function pointer to the method of
   * BaseMotor that will give the speed limit of the motor.
   * Use &BaseMotor::getMinimumspeed() for minimum and &BaseMotor::getMaximumspeed()
   */
  int getLimitingMotorSpeed(int (BaseMotor::*getLimitingSpeed)() const,
                            const bool& i_findMaxAcceleration) const;

  /**
   * return if i_speed is possible for all the joints,
   * if it is not possible returns false and speedyJoint will point to the bad boy
   */
  bool isAchievable(const int& i_speed, WeakJoint* speedyJoint) const;

  /**
   * returns the achievable speed in o_speed
   * If the speed needed to be adjusted for other joints it will return true
   * @param[out] o_speed The found achievable speed for all the joints
   * @param[in] i_findMax indicate if the maximum or
   * minimum speed needs to be found
   */
  bool getAchievable(int* o_speed, const bool& i_findMax) const;

  /// return the speed of the robot give the motor speed
  traceType getRobotSpeed(const int i_speed) const;

  /// Validates all the weak pointers in the m_stepMap
  void validatedJoints() const;
};

#endif  // MOTOR_SPEEDCONTROLLER_H_
