// Copyright [2015] Ruud Cools
#ifndef MATH_SEQUENTIALTRACECALCULATOR_H_
#define MATH_SEQUENTIALTRACECALCULATOR_H_

class Robot;
#include <TSA.h>
class Trace;

class SequentialTraceCalculator {
  /// Robot which will be operated on
  GETSET(std::shared_ptr<Robot>, m_robot, Robot);

  /// Trace section which will be calculated
  GETSET(std::vector<std::shared_ptr<Trace>>, m_traceSection, TraceSection);

  /// Let the robot come to an halt after each section
  GETSET(bool, m_stopAfterSection, StopAfterSection);

  /// Determine if the traces should be orded before calculation
  GETSET(bool, m_orderTraces, OrderTraces);

 public:
  /// Easy Constructor
  SequentialTraceCalculator();
  /// Construction with robot
  SequentialTraceCalculator(const std::shared_ptr<Robot>& i_robot);
  /// Full Fledged Constructor
  SequentialTraceCalculator(const std::shared_ptr<Robot>& i_robot,
                            const tsa::TraceSection&);
  /// Full Fledged Constructor
  SequentialTraceCalculator(const std::shared_ptr<Robot>& i_robot,
                            const tsa::TraceSection&,
                            const bool& i_stopAfterSegment,
                            const bool& i_orderTraces);
  /// Calculated Traces
  void calculatedTraces();
  /// return the ordered traces
  std::vector<std::shared_ptr<Trace>> getOrdedSections();
 private:
  void addMovementTrace(const Point2D& i_startPoint,
                        const Point2D& i_endPoint);
};


#endif  // MATH_SEQUENTIALTRACECALCULATOR_H_
