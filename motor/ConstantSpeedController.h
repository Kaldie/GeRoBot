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

  /// Default constructor
  ConstantSpeedController();

  /// Constructor with a speed defined
   ConstantSpeedController(const float& speed);

   ConstantSpeedController(const float& i_robotSpeed, const int& i_vectorPosition);

  virtual ~ConstantSpeedController(){};
private:
  int determineJointSpeed() const;

  int determineJointSpeed(const std::shared_ptr<BaseJoint>& i_joint) const;
  
  int determineJointSpeed(const std::shared_ptr<BaseJoint>& i_joint,
			   const traceType& i_movementPerStep) const;
};

#endif  // MOTOR_CONSTANTSPEEDCONTROLLER_H_
