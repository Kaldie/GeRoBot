// Copyright [2015] Ruud Cools
#ifndef MATH_SEQUENTIALTRACECALCULATOR_H_
#define MATH_SEQUENTIALTRACECALCULATOR_H_

class Robot;
class Trace;

class SequentialTraceCalculator {
  /// Robot which will be operated on
  GETSET(std::shared_ptr<Robot>, m_robot, Robot);

  /// Trace section which will be calculated
  GETSET(std::vector<std::shared_ptr<Trace>>, m_traceSection, TraceSection);

  /// Let the robot come to an halt after each section
  GETSET(bool, m_stopAfterSection, StopAfterSection);
 public:
  /// Easy Constructor
  SequentialTraceCalculator();
  /// Construction with robot
  SequentialTraceCalculator(const Robot::RobotPointer& i_robot);
  /// Full Fledged Constructor
  SequentialTraceCalculator(const Robot::RobotPointer& i_robot,
                            const tsa::TraceSection&);
  /// Full Fledged Constructor
  SequentialTraceCalculator(const Robot::RobotPointer& i_robot,
                            const tsa::TraceSection&,
                            const bool& i_stopAfterSegment);
  /// Calculated Traces
  void calculatedTraces();
  /// return the ordered traces
  std::vector<std::shared_ptr<Trace>> getOrdedSections();
 private:
  void AddMovementTrace(const Point2D& i_startPoint,
                        const Point2D& i_endPoint);
};


#endif  // MATH_SEQUENTIALTRACECALCULATOR_H_
