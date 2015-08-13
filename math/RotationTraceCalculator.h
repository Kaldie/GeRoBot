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
   * @param[in] RotationTrace* trace that needs to be evaluated
   * @param[in] i_currentPosition Position of the robot
   */
  virtual void calculateTrace(const RotationTrace* i_trace,
                              Point2D& i_currentPosition);
  /// Default constructor
  RotationTraceCalculator();

  /// Constructor with defined joint controller pointer
  explicit RotationTraceCalculator(JointController* i_jointControllerPointer);

  /// Constructor with tolerance and joint controller
  RotationTraceCalculator(JointController* i_jointControllerPointer,
                          const traceType& i_tolerance);
};

#endif  // MATH_ROTATIONTRACECALCULATOR_H_
