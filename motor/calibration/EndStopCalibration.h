#ifndef MOTOR_CALIBRATION_ENDSTOPCALIBRATION_H_
#define MOTOR_CALIBRATION_ENDSTOPCALIBRATION_H_

#include <BaseCalibration.h>

class BaseJoint;
class Robot;
class MovementRegistrator;
class EndStop;

class EndStopCalibration : public BaseCalibration {
  /// Registrator which looks at the movement of the robot between the end stops
  GETSET(std::shared_ptr<MovementRegistrator>, m_registrator, Registrator);

  /// For this calibration module it is not necessary that a registrator is present
  /// I.e. there only 1 endstop is available on the joint
  GETSET(bool, m_hasRegistrator, HasRegistrator);

  /// indicate if a movement per step calibration needs to be done
  GET(bool, m_doStepMovementCalibration, DoStepMovementCalibration);

  /// movent per step
  GET(traceType, m_movementPerStep, MovementPerStep);

  /// Position
  GET(traceType, m_jointPosition, JointPosition);

  /// amount of steps that will be taken to reach the end stop relative to the estimate amount
  /// The goal is to decrease the distance slowly, so we hit the end stop when doing 1 step
  GETSET(traceType, m_finalStepRate, FinalStepRate);

 public:
  EndStopCalibration(const std::shared_ptr<BaseJoint>& i_joint,
		     const std::shared_ptr<Robot>& i_robot);

  ~EndStopCalibration();

  /**
   * Try to update the do step movement calibration
   * returns the succes, if for instance the joint has only 1 endstop,
   * this does not work and thus returns false
   */
  bool setDoStepMovementCalibration(const bool& i_value);

  /// Preform the calibration -> produce the end results
  virtual void execute() override;

  /// indicate if execution is possible -> all requirements are met
  virtual bool isReady() const override;
  
  /// apply the values retrieved during the calibration to the joint
  virtual void apply() override;

  
 private:
  EndStopCalibration();

  bool canDoMovementPerStepCalibration() const;

  bool executePositionUpdateOnEndStop(const std::shared_ptr<EndStop>& i_endStop);
  void executeMovementPerStep();
  std::string getJointMovementToEndStop(const std::shared_ptr<EndStop> i_endStop) const;
  int estimateSteps(const std::shared_ptr<EndStop>& i_endStop) const;
  void createOutputForMovementPerStep(const std::vector<traceType>& i_movementPerStep);
};


#endif  // MOTOR_CALIBRATION_ENDSTOPCALIBRATION_H_
