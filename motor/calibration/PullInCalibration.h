//Copyright Ruud Cools [2017]
#ifndef MOTOR_CALIBRATION_PULLINCALLIBRATION_H_
#define MOTOR_CALIBRATION_PULLINCALLIBRATION_H_

#include <MotorSpeedCalibration.h>

class BaseJoint;
class Robot;

class PullInCalibration : public MotorSpeedCalibration {
  GETSET(bool, m_calibratePullIn, CalibratePullIn);
  GETSET(int, m_pullIn, PullIn);
  GETSET(int, m_pullInLength, PullInLength);

 public:
  PullInCalibration(const std::shared_ptr<BaseJoint>& i_joint,
		    const std::shared_ptr<Robot>& i_robot);

  virtual void execute() override;

  virtual void apply() override;

  virtual std::string name() const override {return "PullIn Callibration";};

};


#endif  // MOTOR_CALIBRATION_PULLINCALLIBRATION_H_
