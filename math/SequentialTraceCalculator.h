// Copyright [2015] Ruud Cools

#ifndef MATH_SEQUENTRIALTRACECALCULATOR_H_
#define MATH_SEQUENTRIALTRACECALCULATOR_H_

#include <Robot.h>
#include <Trace.h>

class Polygon2D;
class TraceSections;  // forward declared because it is a private type def.
/**
 * Class which is responsible for dispatching calculation calls
 * to create a proper sequence trace vector for the robot
 */
class SequentialTraceCalculator {
  GETSET(Trace::TracePointerVector, m_traceVector, TracePointerVector);

 public:
  /**
   * TraceSections describes groups of traces
   * These groups have  something in common.
   * I.e. they describe a closed section
   */
  typedef std::vector<Trace::TracePointerVector> TraceSections;
  /// easy constructor
  SequentialTraceCalculator();
  /// fully fledged constructor
  SequentialTraceCalculator(const Trace::TracePointerVector& i_vector);

  /**
   * Returns a vector in which the traces are orderd
   * Based on the employed algorithm the traces will be reordered
   */
  Trace::TracePointerVector getOrderVector();

  /// determines if this trace is a part of a closed section
  bool isPartOfClosedSection(const Trace::TracePointer& i_trace) const;

  /**
   * Returns the number of closed sections and add these to the i_closedSections TraceSections
   * @param[in] i_closedSections Groups which are closed
   */
  int getClosedSections(TraceSections* i_closedSections) const;
 private:
  /**
   * Returns a vector which has all the traces which start/ stop at the trace
   */
  Trace::TracePointerVector getAbuttingTraces(const Trace::TracePointer& i_trace) const;
  /**
   * Get all connected Trace starting from i_trace
   */
  void getAllConnected(const Trace::TracePointer& i_trace,
                       Trace::TracePointerVector* i_connectedTraces) const ;

  /// determines if 2 traces are abutting
  bool isAbutting(const Trace::TracePointer& i_first,
		  const Trace::TracePointer& i_second) const;

  /**
   * Get the nearest trace from i_traceVector
   * @param[in] i_trace Distances will be calculated from this trace
   * @param[in] i_vector All traces from this vector will be searched
   */
  Trace::TracePointer getNearestTrace(const Trace::TracePointer& i_trace,
				      const Trace::TracePointerVector&);

  /// Determine of the section described by the vector is closed
  bool isClosedSection(const Trace::TracePointerVector& i_vector) const ;


  traceType getMinimalDistance(const Trace::TracePointer& i_first,
                               const Trace::TracePointer& i_second) const;
  /**
   * given a TracePointerVector create an qt polygon from all the start and stop positions
   * The order of the vector is considered during the creation of the vector
   * The best way to create such a vector is to call getAllconnected()
   * @param[in] i_vector ordered vector with trace pointers from which a polygon need to be created
   * @param[in] i_polygon The Polygon that will be made
   */
  void createPolygon(const Trace::TracePointerVector& i_vector,
                     Polygon2D* i_polygon) const;

  bool isInside(const Trace::TracePointerVector& i_larger,
                const Trace::TracePointerVector& i_smaller) const;

  bool isInside(const Trace::TracePointerVector& i_larger,
                const Trace::TracePointer& i_trace) const;
};

#endif  // MATH_SEQUENTRIALTRACECALCULATOR_H_
