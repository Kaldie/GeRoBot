// copyright Ruud Cools [2017]
#ifndef MOTOR_CALIBRATION_MOTORSPEEDCALIBRATION_H_
#define MOTOR_CALIBRATION_MOTORSPEEDCALIBRATION_H_
#include <BaseCalibration.h>

class SpeedController;

class MotorSpeedCalibration : public BaseCalibration {
  GETSET(bool, m_calibratePullIn, CalibratePullIn);
  GETSET(int, m_pullIn, PullIn);
  GETSET(int, m_pullInLength, PullInLength);
  GETSET(bool, m_pullInSucces, PullInSucces);
  
  GETSET(bool, m_calibrateAcceleration, CalibrateAcceleration);
  GETSET(traceType, m_acceleration, Acceleration);
  GETSET(bool, m_accelerationSucces, AccelerationSucces);
  
  GETSET(bool, m_calibrateMaximumSpeed, CalibrateMaximumSpeed);
  GETSET(int, m_maximumSpeed, MaximumSpeed);
  GETSET(bool, m_maximumSpeedSucces, MaximumSpeedSucces);

  /// the area in the calibration will be search based on the currently known values
  /// meaning that if search range is 0.1 a range of 90% to 110% of the current value will be investigated
  /// in contrast to other calibrators, this is necessary due to the fact that we really dont know beforehand how it will go
  GETSET(traceType, m_searchRange, SearchRange);

  /// Size of the steps taken during the calibrations
  GETSET(traceType, m_stepSize, StepSize);

  /// Delay between calibration steps in msec
  GETSET(int, m_delay, Delay);

  /// Previous speed controller
  GET(std::shared_ptr<SpeedController>, m_prevController, PrevController);
 public:
  /// constructor
  MotorSpeedCalibration(const std::shared_ptr<BaseJoint>& i_joint,
			const std::shared_ptr<Robot>& i_robot);

  /// Deconstructor
  ~MotorSpeedCalibration();
  
  /// Preform the calibration -> produce the end results
  virtual void execute() override;

  /// indicate if execution is possible -> all requirements are met
  virtual bool isReady() const override;

  /// apply the values retrieved during the calibration to the joint
  virtual void apply() override;

 private:
  /// return the direction for which the joint should move
  std::string getJointDirection() const;

  /// no default constructor
  MotorSpeedCalibration();

  /**
   * Calibrate the pull in speed by start and
   * stopping with increased speeds untill users says it fails
   */
  bool calibratePullIn();

  /**
   * Calibrate the acceleration by 
   * starting, accelerate to reasonable value
   * stop and over with increased acceleration
   */
  bool calibrateAcceleration();

  /**
   * Calibrate the maximum speed by 
   * starting, accelerating to a value, stop and again
   * while increasing the maximum speed
   */
  bool calibrateMaximumSpeed();


  /**
   * function will make the robot preform a serie of steps
   * For each element in the speed vector the steps per speed will be set
   * it returns the m_isFinished member which indicate if the user saw shit move
   */
   bool testSpeed(const std::vector<int>& i_speedVector,
		  const int& i_stepsPerSpeed);
};

#endif  // MOTOR_CALIBRATION_MOTORSPEEDCALIBRATION_H_
