// Copyright [2015] Ruud Cools
#ifndef MATH_SEQUENTIALTRACECALCULATOR_H_
#define MATH_SEQUENTIALTRACECALCULATOR_H_

#include <Robot.h>
#include "./TSA.h"

class SequentialTraceCalculator {
  /// Robot which will be operated on
  GETSET(Robot::RobotPointer, m_robot, Robot);
  /// Trace section which will be calculated
  GETSET(tsa::TraceSection, m_traceSection, TraceSection);
 public:
  /// Easy Constructor
  SequentialTraceCalculator();
  /// Construction with robot
  SequentialTraceCalculator(const Robot::RobotPointer& i_robot);
  /// Full Fledged Constructor
  SequentialTraceCalculator(const Robot::RobotPointer& i_robot,
                            const tsa::TraceSection&);
  /// Calculated Traces
  void calculatedTraces();
  /// return the ordered traces
  tsa::TraceSection getOrdedSections();
 private:
  void AddMovementTrace(const Point2D& i_startPoint,
                        const Point2D& i_endPoint);
};


#endif  // MATH_SEQUENTIALTRACECALCULATOR_H_
