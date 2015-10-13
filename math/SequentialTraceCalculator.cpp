// Copyright Ruud Cools [2015]
#include <macroHeader.h>
#include <Robot.h>
#include <Trace.h>
#include <Point2D.h>
#include "./RotationTrace.h"
#include "./Polygon2D.h"
#include "./SequentialTraceCalculator.h"


/// easy constructor
SequentialTraceCalculator::SequentialTraceCalculator()
  : SequentialTraceCalculator(tsa::TraceSection()){
}


/// fully fledged constructor
SequentialTraceCalculator::
SequentialTraceCalculator(const tsa::TraceSection& i_vector)
  : m_traceVector(i_vector){
}


void
SequentialTraceCalculator::orderVector(const bool& i_useHeuristics) {
  if (!i_useHeuristics) {
    // hope someone was paying attention
    return;
  }
  // the final ordered trace pointer vector
  tsa::TraceSection orderedTracePointerVector;
  // Start position of the calculations
  Point2D virtualPosition = m_robot->getPosition();
  // get all closed sections
  tsa::TraceSections sections = tsa::getSections(m_traceVector);
  // Sort them based on the surface
  std::sort(sections.begin(), sections.end(), tsa::sortBigToSmall);
  // get remaining/ isolated traces
  tsa::TraceSection remainingTraces =
    tsa::getRemainingTraces(m_traceVector, sections);
  // while we have closed sections
  while (sections.size() > 0) {
    // Handle one large section and its inner sections
    tsa::TraceSection largeSection =
      handleDependendSections(&sections, &virtualPosition, &orderedTracePointerVector);
    // Find the isolated traces that are within the largeSection
    tsa::TraceSection isolatedTraces =
      tsa::getInsideTraces(remainingTraces, largeSection);
    // add the isolated sections
    appendSection(isolatedTraces, &virtualPosition, &orderedTracePointerVector);
    for (const auto& aTrace : isolatedTraces) {
      remainingTraces.erase(std::remove(remainingTraces.begin(),
                                        remainingTraces.end(),
                                        aTrace));
    }
  }
  LOG_ERROR("Not implemented yet");
}


tsa::TraceSection SequentialTraceCalculator::
handleDependendSections(tsa::TraceSections* o_sections,
                        Point2D* o_position,
                        tsa::TraceSection* o_ordedVector) {
  tsa::TraceSection largeSection = o_sections->front();
  tsa::TraceSections insideSections =
    tsa::getInsideSections(largeSection, *o_sections);
  /// the smallest sections shall be done first
  std::reverse(insideSections.begin(), insideSections.end());
  tsa::TraceSections::iterator startIndependedSection = insideSections.begin();
  tsa::TraceSections::iterator endIndependedSection = startIndependedSection;
  while (endIndependedSection != insideSections.end()) {
    endIndependedSection =
      tsa::getIndependendSections(startIndependedSection, insideSections.end());

    handleIndependendSections(tsa::TraceSections(startIndependedSection,
                                                 endIndependedSection),
                              o_ordedVector,
                              o_position);
    startIndependedSection = endIndependedSection;
  }
  // remove the sections from the overall sections
  for (const auto& section : insideSections) {
    o_sections->erase(std::remove(sections->begin(), sections->end(), section));
  }
  return largeSection;
}


void SequentialTraceCalculator::
handleIsolatedTraces(tsa::TraceSection* o_section,
                     Point2D* o_position,
                     tsa::TraceSection* o_isolatedTraces,
                     tsa::TraceSection* o_ordedVector) {
    tsa::TraceSection isolatedTraces =
      tsa::getInsideTraces(*o_isolatedTraces, *o_section);
    // add the isolated sections
    appendSection(isolatedTraces, o_position, o_ordedVector);
    for (const auto& aTrace : isolatedTraces) {
      o_section->erase(std::remove(o_section->begin(),
                                   o_section->end(),
                                   aTrace));
    }
}


void SequentialTraceCalculator::
handleIndependendSections(tsa::TraceSections i_sections,
                          Trace::TracePointerVector* o_orderedVector,
                          Point2D* i_position) const {
  tsa::TraceSection currentSection;
  while (i_sections.size() > 0) {
    // get the section which is closed
    currentSection =  tsa::getExtremeSection(*i_position,
                                             true,
                                             i_sections);
    // append it the the ordered vector
    appendSection(currentSection, i_position, o_orderedVector);
    // Then throw it out
    i_sections.erase
      (std::remove(i_sections.begin(), i_sections.end(), currentSection));
  }
}


void SequentialTraceCalculator::appendSection(Trace::TracePointerVector i_section,
                                              Point2D* i_virtualPosition,
                                              Trace::TracePointerVector* i_vector) const {
  bool isClosed = tsa::isClosedSection(i_section);
  traceType currentDistance;
  while (i_section.size() > 0) {
    // get the trace which is at the most suitable position
    Trace::TracePointer currentTrace = tsa::getExtremeTrace(*i_virtualPosition,
                                                            isClosed,
                                                            i_section,
                                                            &currentDistance);
    // reverse it if necessary
    if (currentDistance <
        Magnitude(*i_virtualPosition - currentTrace->getStartPoint())) {
      currentTrace->reverse();
    }
    // push it in the ordered vector
    i_vector->push_back(currentTrace);
    // update the position
    *i_virtualPosition = currentTrace->getEndPoint();
    // throw it out
    i_section.erase(std::remove(i_section.begin(), i_section.end(), currentTrace));
  }
}
