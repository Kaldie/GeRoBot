// Copyright [2015] Ruud Cools

#ifndef MATH_TSA_H_
#define MATH_TSA_H_

#include <Robot.h>
#include "./Polygon2D.h"
#include "./Trace.h"

/**
 * In this name space all algorithms used on a section or sections is defined!
 */
namespace tsa {
  /**
   * A group of TracePointers which have a common property
   * This can be that they reside in the same BIGGER section etc.
   */
  typedef Trace::TracePointerVector TraceSection;

  /**
   * A group of TraceSections which describe a project
   */
  typedef std::vector<TraceSection> TraceSections;

  /// determine if this trace is part of an section
  bool isPartOfSection(const Trace::TracePointer& i_trace,
		       const TraceSection& i_section);

  /// determines if this trace is a part of a section section
  bool isPartOfSections(const Trace::TracePointer& i_trace,
			const TraceSections& i_sections);

  /**
   * Returns a vector which has all the traces which start/ stop at the trace
   */
  TraceSection getAbuttingTraces(const Trace::TracePointer& i_trace,
				 const Trace::TracePointerVector& i_vector);

  /**
   * Given the traces in the vector, determine the sections,
   * @param[in] i_section Groups which are closed
   */
  TraceSections getSections(TraceSection i_section);

  /**
   * Get all connected Trace starting from i_trace
   */
  void getAllConnected(const Trace::TracePointer& i_trace,
                       const Trace::TracePointerVector& i_vector,
                       TraceSection* o_connectedTraces);

  /**
   * Given TraceSections get the nearest/farthest one near the given point
   * @param[in] i_point Given point
   * @param[in] i_sections Sections to chose from
   * @param[in] o_fromStart Returns if the nearest point of the trace is its start point
   * @param[in] i_findNearest If true find the nearest, if false find the furthest
   */
  TraceSection getExtremeSection(const Point2D& i_point,
                                 const bool& i_findNearest,
                                 const TraceSections& i_sections);

    /**
     * Get the nearest trace from i_traceVector
     * @param[in] i_point Point to which the neaerst trace will be returned
     * @param[in] i_vector All traces from this vector will be searched
     * @param[out] o_distance Holds the distance to the nearest trace after execution
     * @param[in] i_findNearest If true find the nearest, if false find the furthest
     */
    Trace::TracePointer getExtremeTrace(const Point2D& i_point,
                                        const bool& i_findNearest,
                                        const Trace::TracePointerVector& i_vector,
                                        traceType* o_distance);

  /**
   * Given TraceSections, return the remaining TracePointers
   * @param[in] i_section A groupe of sections which need to be checked if they are sorted in one of the sections
   * @param[in] i_sections A group of sections which have all ready be sorted.
   */
  Trace::TracePointerVector getRemainingTraces(const TraceSection& i_section,
					       const TraceSections& i_sections);

  /// Determine of the section described by the vector is closed
  bool isClosedSection(const TraceSection& i_section);

  /**
   * given a TracePointerVector create an qt polygon from all the start and stop positions
   * The order of the vector is considered during the creation of the vector
   * The best way to create such a vector is to call getAllconnected()
   * @param[in] i_vector ordered vector with trace pointers from which a polygon need to be created
   */
  Polygon2D createPolygon(const TraceSection& i_vector);

  /// check if a trace is inside a number of traces which forms a closed thingy
  bool isInside(const Trace::TracePointerVector& i_larger,
                const Trace::TracePointer& i_trace);

  /// check if a closed trace thingy is inside another one
  bool isInside(const Trace::TracePointerVector& i_bigger,
                const Trace::TracePointerVector& i_smaller);

  TraceSection getInsideTraces(const TraceSection& i_hayStack,
                               const TraceSection& i_biggerSection);

  TraceSections getInsideSections(const TraceSection& i_biggerSection,
                                  const TraceSections& i_sections);
  /**
   * Returns the end of an independent range
   * This is an range for which no sections lay inside each other
   */
  TraceSections::iterator getIndependendSections(const TraceSections::iterator& i_begin,
                                                 const TraceSections::iterator& i_end);


  /**
   * Returns if one trace section is bigger then another
   * Could be used to sort TraceSections based on it size
   * i.e. std::sort(TraceSections::begin(), TraceSections::end(), isBigger);
   */
  bool sortSmallToBig(const TraceSection& i_lhs,
                      const TraceSection& i_rhs);

  bool sortBigToSmall(const TraceSection& i_lhs,
                      const TraceSection& i_rhs);

}
#endif  // MATH_TSA_H_
