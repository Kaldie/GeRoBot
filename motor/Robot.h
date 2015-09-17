// Copyright [2014] Ruud Cools
#ifndef ROBOT_ROBOT_H_
#define ROBOT_ROBOT_H_

#include <JointController.h>
#include <Point2D.h>
class Trace;

/**
 * Robot is the basis of the project.
 *  Robot is the root of a object which looks like: Robot->Joint->Actuator
 *  Robot will also be calculating the traces
 *  To calculating traces we need info on the joints
 *  These traces will be handled by the actuators of the joints
 */
class Robot {
  // Handles the joints and can update the position after a step
  GETSET(JointController::JointControllerPointer, m_jointController, JointController);
  // Speed of the robot during the movement
  GETSET(float, m_speed, Speed);
  // Position of the head of the robot
  GETSET(Point2D, m_currentPosition, Position);

 public:
  typedef std::shared_ptr<Robot> RobotPointer;
  bool hasValidConnection();

  Point2D* getPositionPointer() {return &m_currentPosition;}
    // actuations
  void goToPosition(const Point2D&);
  void goTrace(const Trace&);
  void definePosition(Point2D&);
  void callibrate(Trace&);

  /// Get the amount of movement per step given a direction
  traceType getMovementPerStep(const MovementType&) const ;

  /// Given a point, predict the final position when steps have been set
  Point2D predictSteps(const std::string& i_direction,
                       const int& i_numberOfSteps,
                       Point2D* i_position) const;

  /**
   * prepares to take steps with the given movement type, direction and number
   * This function pepares the joint controller to take steps when actuate is called
   * @param[in] i_movementType Type of movement, Rotational or Translational
   * @param[in] i_direction direction which the step has to be taken, currently IN/OUT or CCW/CW
   * @param[in] i_numberOfSteps Number of steps that has to be taken
   */
  void prepareStep(const MovementType& i_movementType,
                   const std::string& i_direction,
                   const int& i_numberOfSteps);

  /// easy to do
  Robot();
  /// defines controller and speed
  Robot(const JointController::JointControllerPointer& i_pointer,
        const int& i_speed);
  /// fully fledged constructor
  Robot(const JointController::JointControllerPointer& i_pointer,
        const int& i_speed,
        const Point2D& i_point);
};

#endif  // ROBOT_ROBOT_H_

