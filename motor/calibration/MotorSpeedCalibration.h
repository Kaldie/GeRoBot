// copyright Ruud Cools [2017]
#ifndef MOTOR_CALIBRATION_MOTORSPEEDCALIBRATION_H_
#define MOTOR_CALIBRATION_MOTORSPEEDCALIBRATION_H_
#include <BaseCalibration.h>

class SpeedController;
class StepperDriver;

class MotorSpeedCalibration : public BaseCalibration {
  /// the area in the calibration will be search based on the currently known values
  /// meaning that if search range is 0.1 a range of 90% to 110% of the current value will be investigated
  /// in contrast to other calibrators, this is necessary due to the fact that we really dont know beforehand how it will go
  GETSETPROTECTED(traceType, m_searchRange, SearchRange);

  /// Size of the steps taken during the calibrations
  GETSETPROTECTED(traceType, m_stepSize, StepSize);

  /// Delay between calibration steps in msec
  GETSET(int, m_delay, Delay);

  /// Previous speed controller
  GET(std::shared_ptr<SpeedController>, m_prevController, PrevController);

  /// hasSucceded
  GETSETPROTECTED(bool, m_hasSucceded, HasSucceded);

  /// Vector with the numbers that will be tested
  GETSETPROTECTED(std::vector<int>, m_testVector, TestVector);
 public:
  /// constructor
  MotorSpeedCalibration(const std::shared_ptr<BaseJoint>& i_joint,
			const std::shared_ptr<Robot>& i_robot,
			const BaseCalibration::Type);

  /// Deconstructor
  virtual ~MotorSpeedCalibration();
  
  /// indicate if execution is possible -> all requirements are met
  virtual bool isReady() const override;

  /// return the value from the calibration
  virtual int getCalibratedParameter() = 0 ;

  virtual void createOutput();

 private:
  /// return the direction for which the joint should move
  std::string getJointDirection() const;

  /// no default constructor
  MotorSpeedCalibration();

 protected:
  /**
   * function will make the robot preform a serie of steps
   * For each element in the speed vector the steps per speed will be set
   * it returns the m_isFinished member which indicate if the user saw shit move
   */
   bool testSpeed(const std::vector<int>& i_speedVector,	
	  const int& i_stepsPerSpeed);

   void prepareParameterVector(const int&(StepperDriver::*)()const);
};

#endif  // MOTOR_CALIBRATION_MOTORSPEEDCALIBRATION_H_
