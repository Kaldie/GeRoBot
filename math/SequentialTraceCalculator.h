// Copyright [2015] Ruud Cools

#ifndef MATH_SEQUENTRIALTRACECALCULATOR_H_
#define MATH_SEQUENTRIALTRACECALCULATOR_H_

#include <Robot.h>
#include <Trace.h>
#include "./Polygon2D.h"
#include "./TraceSections.h"

/**
 * Class which is responsible for dispatching calculation calls
 * to create a proper sequence trace vector for the robot
 */
class SequentialTraceCalculator {
  GETSET(Trace::TracePointerVector, m_traceVector, TracePointerVector);

 public:
  /// easy constructor
  SequentialTraceCalculator();
  /// fully fledged constructor
  SequentialTraceCalculator(const Trace::TracePointerVector& i_vector);

  /**
   * Returns a vector in which the traces are orderd
   * Based on the employed algorithm the traces will be reordered
   */
  void orderVector(const Point2D& i_currentPosition,
              const bool& i_useHeuristics) const;
 private:
  /**
   * Given TraceSections create an order for which:
   * Each sections which is inside another is ordered earlier then the including section
   * Sections which are mutual include are ordered such that the distance between them is maximised
   * @param[in] i_sections TraceSections for are ordered such that:
   * - the first includes at least all others
   * - all sections except the last are closed sections
   */
  void orderSections
    (TraceSections::TraceSections* i_sections, Trace::TracePointerVector* i_vector) const;
};

#endif  // MATH_SEQUENTRIALTRACECALCULATOR_H_
