// Copyright [2014] Ruud Cools
#ifndef ROBOT_ROBOT_H_
#define ROBOT_ROBOT_H_

//#include <BaseJoint.h>
#include <Point2D.h>
#include <BaseJoint.h>

class Trace;
class JointController;
class SpeedController;
class StateSequence;
class MovementRegistrator;

/**
 * Robot is the basis of the project.
 *  Robot is the root of a object which looks like: Robot->Joint->Actuator
 *  Robot will also be calculating the traces
 *  To calculating traces we need info on the joints
 *  These traces will be handled by the actuators of the joints
 */
class Robot {
  // Handles the joints and can update the position after a step
  GETSET(std::shared_ptr<JointController>, m_jointController, JointController);

  // The current position of the head of the robot
  GET(Point2D, m_position, Position);

  /// Controller which is in charge of the speed of the robot
  GET(std::shared_ptr<SpeedController>, m_speedController, SpeedController);

 private:
  typedef std::set<std::weak_ptr<MovementRegistrator>, std::owner_less<std::weak_ptr<MovementRegistrator>>>
    MovementRegistorSet; // typedef otherwise it does not fit the macro
  /// Retrieve the set of movement registrators. Use registrer/deregister to update it
  GET(MovementRegistorSet, m_movementRegistrators, MovementRegistrators);

  // notify the movement registrators steps have been taken
  void notifySteps(std::shared_ptr<BaseJoint>,
		   const int& i_numberOfSteps) const;

  // notify the movement registrators actuation has been done
  void notifyActuation() const;

 public:
  typedef std::shared_ptr<Robot> RobotPointer;

  /// Set the speed controller, register its movement registrator en deregister the old 1.
  void setSpeedController(const std::shared_ptr<SpeedController>& i_speedController);

  /// Returns if there is a valid arduino connenction
  bool hasValidConnection();

  /// Based on the joints in the controller, calculated the current position
  const Point2D getVirtualPosition() const;

  ///Sets the position of the robot and joints without actuating.
  ///Can be used after calibration or something
  void setPosition(const Point2D& i_position);

  /// move to a position, independend on maner
  void goToPosition(const Point2D&);

  /// Given the pre-calculated steps, prefrom them
  void actuate();

  /// Reset trace calculation. Remove all previous calculations and start over!
  void restartTraceCalculation();

  /// set a step and set the hold motor function to the boolean value
  void setIdle(const bool& i_setIdle);

  /// Get the amount of movement per step given a direction
  traceType getMovementPerStep(const BaseJoint::MovementType&) const ;

  // Register a movement registrator
  bool registerMovementRegistrator(std::shared_ptr<MovementRegistrator> i_registrator);

  // Remove a movement registrator
  bool deregisterMovementRegistrator(std::shared_ptr<MovementRegistrator> i_registrator);

  /**
   * prepares to take steps with the given movement type, direction and number
   * This function pepares the joint controller to take steps when actuate is called
   * It will handle to Speedcontroller so the correct speed is used
   * @param[in] i_direction direction which the step has to be taken, currently IN/OUT or CCW/CW
   * @param[in] i_numberOfSteps Number of steps that has to be taken
   */
  void prepareSteps(const std::string& i_direction,
                    const int& i_numberOfSteps);

  /// easy to do
  Robot();

  /// defines controller
  Robot(const std::shared_ptr<JointController>& i_pointer);

  /// fully fledged constructor
  Robot(const std::shared_ptr<JointController>& i_pointer,
        const Point2D& i_point);
};

#endif  // ROBOT_ROBOT_H_

