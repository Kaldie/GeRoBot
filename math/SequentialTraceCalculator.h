// Copyright [2015] Ruud Cools

#ifndef MATH_SEQUENTRIALTRACECALCULATOR_H_
#define MATH_SEQUENTRIALTRACECALCULATOR_H_

#include <Robot.h>
#include <Trace.h>
#include "./Polygon2D.h"
#include "./TSA.h"  // the tsa

/**
 * Class which is responsible for dispatching calculation calls
 * to create a proper sequence trace vector for the robot
 */
class SequentialTraceCalculator {
  GETSET(Trace::TracePointerVector, m_traceVector, TracePointerVector);
  GETSET(Robot::RobotPointer, m_robot, Robot);

 public:
  /// easy constructor
  SequentialTraceCalculator();
  /// fully fledged constructor
  SequentialTraceCalculator(const Trace::TracePointerVector& i_vector);

  /**
   * Returns a vector in which the traces are orderd
   * Based on the employed algorithm the traces will be reordered
   */
  void orderVector(const bool& i_useHeuristics) const;
 private:
  /**
   * Given TraceSections create an order for which:
   * Each sections which is inside another is ordered earlier then the including section
   * Sections which are mutual include are ordered such that the distance between them is maximised
   * @param[in] i_sections TraceSections for are ordered such that:
   * - the first includes at least all others
   * - all sections except the last are closed sections
   */
  void orderSections (tsa::TraceSections* i_sections,
                      Trace::TracePointerVector* i_vector) const;

  /// Order the given section, based on
  void appendSection(Trace::TracePointerVector i_section,
                     Point2D* i_virtualPosition,
                     Trace::TracePointerVector* i_orderedTracePointerVector) const;

  /**
   * Given TraceSections add the largest section and its included sections
   * @param[in/out] o_sections Sections from which the largest is
   * found and other sections which are inside this section
   * @param[in/out] o_position This position is used to order the sections and indicates the new position
   * @param[in/out] o_ordedVector The resulting vector in which all traces are nicely orded
   */
  tsa::TraceSection handleDependendSections(tsa::TraceSections* o_sections,
                                            Point2D* o_position,
                                            tsa::TraceSection* o_ordedVector);
  /**
   * Append the appropiate traces to the orded vector.
   * @param[in/out] o_section tsa::TraceSection all isolated traces inside this section will be added
   * @param[in/out] o_position Position which determines the order of the isolated traces
   * @param[in/out] o_isolatedTraces A group of individual traces from which appropiate traces will be added
   * @param[in/out] o_orderedvector The resulting vector in which all traces are nicely orded
   */
  void handleIsolatedTraces(tsa::TraceSection* o_section,
                            Point2D* o_position,
                            tsa::TraceSection* o_isolatedTraces,
                            tsa::TraceSection* o_ordedVector);

  void handleIndependendSections(tsa::TraceSections i_independedSections,
                                 Trace::TracePointerVector* i_orderedTracePointerVector,
                                 Point2D* i_virtualPosition) const;
};

#endif  // MATH_SEQUENTRIALTRACECALCULATOR_H_
