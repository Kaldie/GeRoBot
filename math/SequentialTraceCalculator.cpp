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
  : SequentialTraceCalculator(Trace::TracePointerVector()){
}


/// fully fledged constructor
SequentialTraceCalculator::
SequentialTraceCalculator(const Trace::TracePointerVector& i_vector)
  : m_traceVector(i_vector){
}


void
SequentialTraceCalculator::orderVector(const Point2D& i_currentPosition,
                                       const bool& i_useHeuristics) const {
  using namespace TraceSections;
  if (!i_useHeuristics) {
    // hope someone was paying attention
    return;
  }
  // the final ordered trace pointer vector
  Trace::TracePointerVector orderedTracePointerVector;
  // Start position of the calculations
  Point2D virtualPosition = i_currentPosition;
      // get all closed sections
    TraceSections sections = getSections(m_traceVector);
    // Sort them based on the surface
    std::sort(sections.begin(), sections.end(), isBigger);
    // get remaining/ isolated traces
    TraceSection remainingTraces = getRemainingTraces(m_traceVector,
                                                      sections);
    // while we have closed sections
    while (sections.size() > 0) {
      // get the current biggest one
      TraceSection largeSection = sections.back();
      sections.pop_back();
      // find all sections and traces inside that larget section
      TraceSections insideSections = getInsideSections(sections,
                                                       largeSection);
      // Add the biggest section of them all
      insideSections.insert(insideSections.begin(), largeSection);
      // Find the isolated traces that are within the largeSection
      TraceSection isolatedTraces = getInsideSection(remainingTraces,
                                                      largeSection);
      /// the smallest sections shall be done first
      std::reverse(insideSections.begin(), insideSections.end());
      TraceSections::iterator startIndependedSection = insideSections.begin();
      TraceSections::iterator endIndependedSection;
      do {
        endIndependedSection = getIndependendSections(startIndependedSection,
                                                      insideSections.end());
        // add the isolated sections
        appendSection(isolatedTraces
                                 &orderedTracePointerVector,
                                 &virtualPosition);

        handleIndependendSections(&independedSections,
                                  &orderedTracePointerVector,
                                  &virtualPosition);
        startIndependedSection = endIndependedSection;
      } while (endIndependedSection != insideSections.end());
      // remove the isolated traces from the
      insideSections.pop_front();
      for (const auto& section : insideSections) {
        sections.erase(std::remove(sections.begin(), sections.end(), section));
      }

    }
    LOG_ERROR("Not implemented yet");
  }
}


void SequentialTraceCalculator::handleIndependendSections
(TraceSections* i_independedSections,
 Trace::TracePointerVector* i_orderedTracePointerVector,
 Point2D* i_virtualPosition) const {
  appendSection(i_independedSections,
                i_virtualPosition,
                i_orderedTracePointerVector,
                true);
  while (i_independedSections->size() > 0) {
    appendSection(i_independedSections,
                  i_virtualPosition,
                  i_orderedTracePointerVector,
                  false);
  }
}


void SequentialTraceCalculator::appendSection
(const Trace::TracePointerVector i_section,
 Point2D* i_virtualPosition,
 Trace::TracePointerVector* i_orderedTracePointerVector,
 const bool& i_isNearest) const {
  Trace::TracePointerVector sequence;
  Trace::TracePointer trace;
  bool fromStartPoint(true);
  if (i_isNearest) {
  // get nearest sequence
    sequence = getNearestSequence(*i_virtualPosition,
                                  *i_independedSections,
                                  &fromStartPoint);
    trace = getNearestTrace(*i_virtualPosition, sequence, nullptr);
  } else {
    // or furthest
    sequence = getFurthestSequence(*i_virtualPosition,
                                   *i_independedSections,
                                   &fromStartPoint);
    trace = getFurthestTrace(*i_virtualPosition, sequence, nullptr);
  }
  // if necessary reverse the trace
  if (!fromStartPoint) {
    for (auto& tracePointer : sequence) {
      tracePointer->reverse();
    }
  }
  // And finaly append the trace to the orded trace pointer vector!
  i_orderedTracePointerVector->insert(i_orderedTracePointerVector->end(),
                                      sequence.begin(),
                                      sequence.end());
  *i_virtualPosition = sequence.front()->getEndPoint();
  // remove it frmo the independedsections
  i_independedSections->erase(std::remove(i_independedSections->begin(),
                                          i_independedSections->end(),
                                          sequence));
}
