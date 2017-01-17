// Copyright Ruud Cools [2017]
#ifndef MOTOR_CALIBRATION_CALIBRATIONDIRECTOR_H_
#define MOTOR_CALIBRATION_CALIBRATIONDIRECTOR_H_

class BaseJoint;
class BaseCalibration;
class Robot;
class BaseJoint;
/// Class responsible for directing the calibration process
/// Involves deciding which calibrations need to be done
class CalibrationDirector {
 private:
  typedef std::vector<std::shared_ptr<BaseCalibration>> CalibrationVector;
  typedef std::map<const std::shared_ptr<BaseJoint>,bool> JointMap;
  ///vector where all the calibrations are stored
  GET(CalibrationVector, m_calibrations, CalibrationVector);
  /// Iterator that points to the current calibration
  GET(CalibrationVector::iterator, m_currentCalibration, CurrentCalibration);
  /// Pointer to the robot
  GET(std::shared_ptr<Robot>, m_robot, Robot);
  /// pointer to the current joint
  GETSET(std::shared_ptr<BaseJoint>, m_joint, Joint);
  GETSET(JointMap, m_jointMap, JointMap);

  /// bools indicating if types of calibrations has to be preformed
  GETSET(bool, m_pointCalibration, PointCalibration);
  GETSET(bool, m_motorCalibration, MotorCalibration);
  GETSET(bool, m_endstopPositionCalibration, EndStopPositionCalibration);
  GETSET(bool, m_endstopMovementCalibration, EndStopMovementCalibration)

 public:
  // constructor which can use all the fancy stuff
  CalibrationDirector(const std::shared_ptr<Robot>& i_robot, 
		      const std::shared_ptr<BaseJoint>& i_joint = nullptr);

  std::shared_ptr<BaseCalibration> next();

  /// Restart the calibrations, clean out the vector and reset the iterator
  void restart();

  /// get all the calibrations for all the joints of the joint controller
  void getCalibrations(CalibrationVector* o_vector) const;
  
  /// get the all the calibrations for this joint
  void getCalibrations(const std::shared_ptr<BaseJoint>&,
		       CalibrationVector* o_vector) const;

  void updateJointMap(const std::shared_ptr<BaseJoint>& i_joint,
		      const bool& i_include);


  int getNumberOfCalibrations() const;

  void getNumberOfCalibrations(const std::shared_ptr<BaseJoint> i_joint,
			       int* io_count) const;
  
 private:
  // no default constructor 
  CalibrationDirector();
  void initialiseJointMap();


};

#endif  // MOTOR_CALIBRATION_CALIBRATIONDIRECTOR_H_
