// Copyright Ruud Cools [2015]
#include <macroHeader.h>
#include <Point2D.h>
#include "./OTSA.h"
namespace tsa {
  namespace otsa {
    // order an group of traces
    void orderVector(TraceSection* io_traces,
                     const Point2D& i_startPosition) {
      TraceSection inputTraceSection;
      inputTraceSection.swap(*io_traces);
      // Start position of the calculations
      Point2D virtualPosition(i_startPosition);
      // get all closed sections
      tsa::TraceSections sections = tsa::getSections(inputTraceSection);
      LOG_DEBUG("Number of sections: " << sections.size());
      // Sort them based on the surface
      std::sort(sections.begin(), sections.end(), sortBigToSmall);
      // get remaining/ isolated traces
      for (const auto& trace : getRemainingTraces(inputTraceSection, sections)) {
        sections.push_back(TraceSection({trace}));
      }
      // while we have closed sections
      while (sections.size() > 0) {
        // Handle one large section and its inner sections
        LOG_DEBUG("Handle Large section");
        TraceSection largeSection =
          handleLargestSection(&sections, &virtualPosition, io_traces);
        // Add the isolated traces within the large section
        //        LOG_DEBUG("Handle isolated traces.");
        //        handleIsolatedTraces
        //          (largeSection, &virtualPosition, &remainingTraces, io_traces);
      }
      // add the remaining isolated sections
      //      appendSection(remainingTraces, &virtualPosition, io_traces);

    }


    TraceSection handleLargestSection(TraceSections* o_sections,
                                      Point2D* o_position,
                                      TraceSection* o_traces) {
      LOG_DEBUG("handleLargestSection");
      TraceSection largeSection = o_sections->front();
      TraceSections insideSections =
        getInsideSections(largeSection, *o_sections);
      /// the smallest sections shall be done first
      std::reverse(insideSections.begin(), insideSections.end());
      insideSections.push_back(largeSection);
      TraceSections::iterator startIndependedSection = insideSections.begin();
      TraceSections::iterator endIndependedSection = startIndependedSection;
      while (endIndependedSection != insideSections.end()) {
        endIndependedSection =
          getIndependendSections(startIndependedSection, insideSections.end());

        handleIndependendSections(TraceSections(startIndependedSection,
                                                endIndependedSection),
                                  o_traces,
                                  o_position);
        startIndependedSection = endIndependedSection;
      }
      // remove the sections from the overall sections
      for (const auto& section : insideSections) {
        o_sections->erase(std::remove(o_sections->begin(), o_sections->end(), section));
      }
      return largeSection;
    }


    void handleIsolatedTraces(const TraceSection& i_section,
                              Point2D* o_position,
                              TraceSection* o_isolatedTraces,
                              TraceSection* o_traces) {
      TraceSection isolatedTraces =
        getInsideTraces(*o_isolatedTraces, i_section);
      // add the isolated sections
      appendSection(isolatedTraces, o_position, o_traces);
      for (const auto& aTrace : isolatedTraces) {
        o_isolatedTraces->erase(std::remove(o_isolatedTraces->begin(),
                                            o_isolatedTraces->end(),
                                            aTrace));
      }
    }


    void handleIndependendSections(TraceSections i_sections,
                                   Trace::TracePointerVector* o_orderedVector,
                                   Point2D* i_position) {
      TraceSection currentSection;
      while (i_sections.size() > 0) {
        // get the section which is closed
        currentSection =  getExtremeSection(*i_position,
                                            true,
                                            i_sections);
        // append it the the ordered vector
        appendSection(currentSection, i_position, o_orderedVector);
        // Then throw it out
        i_sections.erase
          (std::remove(i_sections.begin(), i_sections.end(), currentSection));
      }
    }


    void appendSection(Trace::TracePointerVector i_section,
                       Point2D* i_virtualPosition,
                       Trace::TracePointerVector* i_vector) {
      bool isClosed = isClosedSection(i_section);
      traceType currentDistance;
      while (i_section.size() > 0) {
        // get the trace which is at the most suitable position
        Trace::TracePointer currentTrace = getExtremeTrace(*i_virtualPosition,
                                                           isClosed,
                                                           i_section,
                                                           &currentDistance);
        // reverse it if necessary
        traceType startPointDistance =
          Magnitude(*i_virtualPosition - currentTrace->getStartPoint());
        currentDistance = std::abs(currentDistance);
        if (!(currentDistance - TOLERANCE < startPointDistance &&
              currentDistance + TOLERANCE > startPointDistance)) {
          LOG_DEBUG("Reverse trace!");
          currentTrace->reverse();
        }
        LOG_DEBUG("Current Trace Start point: " << currentTrace->getStartPoint().x <<
                  " , " << currentTrace->getStartPoint().y);
        LOG_DEBUG("Current Trace End point: " << currentTrace->getEndPoint().x <<
                  " , " << currentTrace->getEndPoint().y);

        // push it in the ordered vector
        i_vector->push_back(currentTrace);
        // update the position
        *i_virtualPosition = currentTrace->getEndPoint();
        // throw it out
        i_section.erase(std::remove(i_section.begin(), i_section.end(), currentTrace));
      }
    }

  }
}
