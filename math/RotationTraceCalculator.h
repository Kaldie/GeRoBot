// Copyright [2015] Ruud Cools

#ifndef MATH_ROTATIONTRACECALCULATOR_H_
#define MATH_ROTATIONTRACECALCULATOR_H_

#include <LineTraceCalculator.h>

class RotationTrace;
class Point2D;
class JointController;

class RotationTraceCalculator: public LineTraceCalculator {
 public:
  /**
   * Calculate the trace
   * @param[in] i_trace i_trace trace that needs to be evaluated
   * @param[in] i_currentPosition Position of the robot
   */
  virtual void calculateTrace(const RotationTrace& i_trace);
  /// Default constructor
  RotationTraceCalculator();

  /// Constructor with defined joint controller pointer
  explicit RotationTraceCalculator
    (Robot* i_robot);

  /// Constructor with tolerance and joint controller
  RotationTraceCalculator
    (Robot* i_robot,
     const traceType& i_tolerance);
};

#endif  // MATH_ROTATIONTRACECALCULATOR_H_
