// Copyright [2017] Ruud Cools
#ifndef MOTOR_CALIBRATION_POINTCALIBRATION_H_
#define MOTOR_CALIBRATION_POINTCALIBRATION_H_

#include <BaseCalibration.h>

class Point2D;
class MovementRegistrator;
class BaseJoint;
class Robot;

// Class represents the base calibration unit, which defines the basic functionality
class PointCalibration : public BaseCalibration {
  GETSET(std::vector<Point2D>, m_points, Points);
  GETSET(std::vector<int>, m_stepsBetweenPoints, StepsBetweenPoints);
  GETSET(std::vector<int>, m_priorEstimateSteps, PriorEstimates);
  GETSET(std::vector<double>, m_stepSizes, StepSizes);
  GETSET(std::shared_ptr<MovementRegistrator>, m_registrator, Registrator);
 public:
  // Constructor
  PointCalibration(const std::shared_ptr<BaseJoint>& i_joint,
		   const std::shared_ptr<Robot>& i_robot);
  // Deconstructor
  ~PointCalibration();
  
  virtual void execute() override;

  virtual bool isReady() const override;

  virtual void apply() override;
  
  void recordPoint(const Point2D& i_point);

  /// add a point that will be used in the calibration.
  void addPoint2D(const Point2D& i_point);

 private:
  typedef std::map<std::shared_ptr<BaseJoint>, traceType,
    std::owner_less<std::shared_ptr<BaseJoint>>> JointMap;

  PointCalibration();
  void recordPoint(const Point2D& i_point,
		   const int& i_stepsSet);
  void createOutput();
  
  void addOtherJointErrorToOutput(const JointMap& i_map,
				  const Point2D& i_point);
  
  int estimateStepsBetweenPoints(const Point2D& i_start,
				 const Point2D& i_end) const;

  JointMap estimateErrorOtherJoints(const Point2D& i_point);
};


#endif  // MOTOR_CALIBRATION_POINTCALIBRATION_H_
