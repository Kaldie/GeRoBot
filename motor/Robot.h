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

  // Positions which has been seen during stepping
  GETSET(std::vector<Point2D>, m_traveledPoints, TraveledPoints);

  /// Controller which is in charge of the speed of the robot
  GETSET(std::shared_ptr<SpeedController>, m_speedController, SpeedController);

 public:
  typedef std::shared_ptr<Robot> RobotPointer;

  /// Returns if there is a valid arduino connenction
  bool hasValidConnection();

  /// Based on the joints in the controller, calculated the current position
  const Point2D getVirtualPosition() const;

  /// sets the robot to a position updates its joints to go there 2!
  void setPosition(const Point2D& i_position);

  /// Creates a state which will switch the tool to a active or inactive state
  void switchTool(const bool& i_shouldBeActive) {}; // no yet implemented!

  /// move to a position, independend on maner
  void goToPosition(const Point2D&);

  /// Given the pre-calculated steps, prefrom them
  void actuate();

  /// set a step and set the hold motor function to the boolean value
  void setIdle(const bool& i_setIdle);

  /// Get the amount of movement per step given a direction
  traceType getMovementPerStep(const BaseJoint::MovementType&) const ;


  void traceCalculation(const std::shared_ptr<Trace>& i_trace);

  /**
   * prepares to take steps with the given movement type, direction and number
   * This function pepares the joint controller to take steps when actuate is called
   * It will handle to Speedcontroller so the correct speed is used
   * @param[in] i_direction direction which the step has to be taken, currently IN/OUT or CCW/CW
   * @param[in] i_numberOfSteps Number of steps that has to be taken
   */
  void prepareSteps(const std::string& i_direction,
                    const int& i_numberOfSteps);

  /// Add an StateSequence to the vector. Use this for instance when a wait is required
  void addToSequence(const StateSequence& i_sequence);

  /// easy to do
  Robot();

  /// defines controlle
  Robot(const std::shared_ptr<JointController>& i_pointer);

  /// fully fledged constructor
  Robot(const std::shared_ptr<JointController>& i_pointer,
        const Point2D& i_point);

 private:
  int getNumberOfSequences(const std::shared_ptr<JointController>&);
  int getNumberOfSequences(const bool&);
};

#endif  // ROBOT_ROBOT_H_

