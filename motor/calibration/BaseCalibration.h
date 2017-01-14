// Copyright [2017] Ruud Cools
#ifndef MOTOR_CALIBRATION_BASECALIBRATION_H_
#define MOTOR_CALIBRATION_BASECALIBRATION_H_

class CalibrationOutput;
class BaseJoint;
class Robot;

// Class represents the base calibration unit, which defines the basic functionality
class BaseCalibration {
 public:
  enum Type {Base, PointPoisition,
	     EndStopPosition, MotorSpeed};
  
  GETSETPROTECTED(bool, m_finished, Finished);
  GETSETPROTECTED(std::shared_ptr<BaseJoint>, m_joint, Joint);
  GETSETPROTECTED(std::shared_ptr<Robot>, m_robot, Robot);
  GETSETPROTECTED(std::shared_ptr<CalibrationOutput>, m_output, Output);
  GETSET(BaseCalibration::Type, m_type, Type);

 public:
  /// Preform the calibration -> produce the end results
  virtual void execute() = 0;

  /// indicate if execution is possible -> all requirements are met
  virtual bool isReady() const = 0;

  /// apply the values retrieved during the calibration to the joint
  virtual void apply() = 0;

  /// Constructors
  BaseCalibration(const std::shared_ptr<BaseJoint>& i_joint,
		  const std::shared_ptr<Robot>& i_robot,
		  const BaseCalibration::Type& i_type);

  virtual ~BaseCalibration(){};
 protected:
  std::string getJointDirection(const bool& i_isInner) const;
 private:
  /// no default constructor
  BaseCalibration();
};


#endif  // MOTOR_CALIBRATION_BASECALIBRATION_H_
