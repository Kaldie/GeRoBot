// Copyright Ruud Cools [2017]

#ifndef MOTOR_CALIBRATION_MAXIMUMSPEEDCALIBRATION_H_
#define MOTOR_CALIBRATION_MAXIMUMSPEEDCALIBRATION_H_

class BaseJoint;
class Robot;

#include <MotorSpeedCalibration.h>

class MaximumSpeedCalibration : public MotorSpeedCalibration {
  GETSET(int, m_maximumSpeed, MaximumSpeed);

 public:
  MaximumSpeedCalibration(const std::shared_ptr<BaseJoint>& i_joint,
			  const std::shared_ptr<Robot>& i_robot);

  virtual void execute() override;

  virtual void apply() override;

  virtual std::string name() const override {return "Maximum Speed Callibration";};
  virtual int getCalibratedParameter() override {return m_maximumSpeed;}; 
  virtual void createOutput() override;
};


#endif  //MOTOR_CALIBRATION_MAXIMUMSPEEDCALIBRATION__H
