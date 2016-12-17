// Copyright [2016] Ruud Cools
#ifndef MOTOR_CONSTANTSPEEDCONTROLLER_H_
#define MOTOR_CONSTANTSPEEDCONTROLLER_H_

#include <SpeedController.h>

class Trace;
class BaseJoint;
// Controller which choses 1 speed for a whole trace
class ConstantSpeedController : public SpeedController {
public:
  /// This method will be called at the moment the robot needs to decied on some speed
  virtual bool adviseSpeed(int* o_speed) const;

  /**
   * This is call just before a algorithm determins movement of the robot
   */
  virtual void prepareSpeedController(const Trace& i_trace,
                                      const JointController& i_controller);

  /**
   * Prepare the controller that steps with this joint will be set
   * it is unknown how many and in which direction
   * if a "Constant" type controller is used, use the current position as a base to calculate the speed
   */
  virtual void prepareSpeedController(const std::shared_ptr<BaseJoint>& i_pointer);

  /// Default constructor
  ConstantSpeedController();

  /// Constructor with a speed defined
   ConstantSpeedController(const float& speed);

   ConstantSpeedController(const float& i_robotSpeed, const int& i_vectorPosition);

  virtual ~ConstantSpeedController(){};
private:

  void updateJointSpeed(const std::shared_ptr<BaseJoint>& i_joint,
			const traceType& i_movementPerStep);

  float m_frequency;
};

#endif  // MOTOR_CONSTANTSPEEDCONTROLLER_H_
