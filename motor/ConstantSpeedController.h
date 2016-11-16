// Copyright [2016] Ruud Cools
#ifndef MOTOR_CONSTANTSPEEDCONTROLLER_H_
#define MOTOR_CONSTANTSPEEDCONTROLLER_H_

#include <SpeedController.h>

class Trace;

// Controller which choses 1 speed for a whole trace
class ConstantSpeedController : public SpeedController {
public:
  /// This method will be called at the moment the robot needs to decied on some speed
  virtual bool adviseSpeed(int* o_speed);

  /**
   * This is call just before a algorithm determins movement of the robot
   */
  virtual void prepareSpeedController(const Trace& i_trace,
                                      const JointController& i_controller);

  /// Default constructor
  ConstantSpeedController();

  /// Constructor with a speed defined
   ConstantSpeedController(float speed);

   ConstantSpeedController(float i_robotSpeed, float i_motorSpeed);

  virtual ~ConstantSpeedController(){};
private:
  float m_jointSpeed;
};

#endif  // MOTOR_CONSTANTSPEEDCONTROLLER_H_
