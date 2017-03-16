// Copyright Ruud Cools [2017]
#ifndef MOTOR_CALIBRATION_ACCELERATIONCALIBRATION_H_
#define MOTOR_CALIBRATION_ACCELERATIONCALIBRATION_H_

#include <MotorSpeedCalibration.h>

class BaseJoint;
class Robot;

class AccelerationCalibration : public MotorSpeedCalibration {
  GETSET(int, m_acceleration, Acceleration);

 public:
  AccelerationCalibration(const std::shared_ptr<BaseJoint>& i_joint,
			  const std::shared_ptr<Robot>& i_robot);

  virtual void execute() override;

  virtual void apply() override;

  virtual std::string name() const override {return "Acceleration Callibration";};

  virtual int getCalibratedParameter() override {return m_acceleration;};

  virtual void createOutput() override;
};

#endif  // MOTOR_CALIBRATION_ACCELERATIONCALIBRATION_H_
