// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "./Trace.h"
#include "./TSA.h"

namespace tsa {
  bool isPartOfSection(const Trace::TracePointer& i_trace,
		       const TraceSection& i_section) {
    for (const auto& trace : i_section) {
      if (trace == i_trace) {
	return true;
      }
    }
    return false;
  }


  bool isPartOfSections(const Trace::TracePointer& i_trace,
			const TraceSections& i_sections) {
    for (const auto& section : i_sections) {
      if (isPartOfSection(i_trace, section)) {
	return true;
      }
    }
    return false;
  }


  bool isClosedSection(const TraceSection& i_section) {
    if (i_section.size() == 0) {
      return false;
    } else if (i_section.size() == 1) {
      if (i_section[0]->getStartPoint() ==
	  i_section[0]->getEndPoint()) {
	return true;
      } else {
	return false;
      }
    }
    return true;
  }


  TraceSection getAbuttingTraces(const Trace::TracePointer& i_trace,
				 const Trace::TracePointerVector& i_vector) {
    Trace::TracePointerVector abuttingVector;
    LOG_DEBUG("getAbuttingTraces()");
    for (auto& trace : i_vector) {
      if (i_trace->isAbutting(*trace)) {
	abuttingVector.push_back(trace);
      }
    }
    if (abuttingVector.size() == 0) {
      abuttingVector.push_back(i_trace);
    }
    return abuttingVector;
  }


  TraceSections getSections(TraceSection i_section) {
    LOG_DEBUG("getSections()");
    if (i_section.size() == 0) {
      return TraceSections();
    }
    // define the current closed section
    TraceSection closedSection;
    // define the sections which are closed
    TraceSections closedSections;
    do {
      getAllConnected(i_section.back(), i_section, &closedSection);
      for (const auto& connectedTrace : closedSection) {
	i_section.erase(std::remove(i_section.begin(), i_section.end(), connectedTrace));
      }
	// if there are more then 1 trace in the section
	if (closedSection.size() > 1) {
	  //Add the connectedTraces to the closedSections vector
	  closedSections.push_back(closedSection);
	  // else only add it if it is closed, otherwise it is an isolated trace
	} else if (isClosedSection(closedSection)) {
	  closedSections.push_back(closedSection);
      }
      // reset the closed section
      closedSection = TraceSection();
    } while (i_section.size() > 0);
    return closedSections;
  }


  void getAllConnected(const Trace::TracePointer& i_trace,
		       const Trace::TracePointerVector& i_vector,
		       TraceSection* o_connectedTraces) {
    if (!o_connectedTraces)
      LOG_ERROR("o_connectedTraces does not point to anything!");
    LOG_DEBUG("getAllConnected()");
    // get ALL the abutting traces
    LOG_DEBUG("Trace: " << i_trace->getStartPoint().x << " , " <<
	      i_trace->getStartPoint().y << " | " << i_trace->getEndPoint().x
	      << " , " << i_trace->getEndPoint().y);
    for (const auto& trace : getAbuttingTraces(i_trace,
					       i_vector)) {
      // if the trace is NOT in the i_connectedTraces
      if (std::find(o_connectedTraces->begin(),
		    o_connectedTraces->end(),
		    trace) == o_connectedTraces->end()) {
	LOG_DEBUG("Found a trace which we didnt look at!");
	// add the trace to the connectedTraces
	o_connectedTraces->push_back(trace);
	// preform recursion to get ALL the connected traces
	getAllConnected(trace, i_vector, o_connectedTraces);
      }
    }
  }


  TraceSection getExtremeSection(const Point2D& i_point,
                                 const bool& i_findNearest,
				 const TraceSections& i_sections) {
    TraceSection nearestSection;
    traceType currentDistance;
    traceType minimalDistance = std::numeric_limits<traceType>::max();
    for (const auto& section : i_sections) {
      getExtremeTrace(i_point, i_findNearest,
		      section,
		      &currentDistance);
      if (minimalDistance > currentDistance) {
	minimalDistance = (currentDistance);
	nearestSection = section;
      }
    }
    return nearestSection;
  }


  Trace::TracePointer getExtremeTrace(const Point2D& i_position,
				      const bool& i_findNearest,
				      const TraceSection& i_section,
				      traceType* o_distance) {
    int mutiplier(-1);
    if (i_findNearest) {
      mutiplier = 1;
    }
    *o_distance = std::numeric_limits<traceType>::max();
    Trace::TracePointer extreme(nullptr);
    traceType currentDistance;
    for (auto& trace : i_section) {
      currentDistance =
  magnitude(trace->getStartPoint() - i_position) * mutiplier;
      LOG_DEBUG("Current distance: " << currentDistance);
      LOG_DEBUG("Current Selected distance: " << *o_distance);
      LOG_DEBUG("Multiplier: " << mutiplier);
      if (*o_distance > currentDistance) {
	*o_distance = currentDistance;
	extreme = trace;
      }
      currentDistance =
  magnitude(trace->getEndPoint() - i_position) * mutiplier;
            LOG_DEBUG("Current distance: " << currentDistance);
      LOG_DEBUG("Current Selected distance: " << *o_distance);
      LOG_DEBUG("Multiplier: " << mutiplier);
      if (*o_distance > currentDistance) {
	*o_distance = currentDistance;
	extreme = trace;
      }
    }
    return extreme;
  }


  TraceSection getRemainingTraces(const TraceSection& i_needles,
				  const TraceSections& i_hayStacks) {
    bool hasFound;
    TraceSection remainingTraces;
    for (const auto& needle : i_needles) {
      hasFound = false;
      // Check if it is known in one of the sections
      for (const auto& hayStack : i_hayStacks) {
	// Check for each trace in the vector if it is the same
	for (const auto& hay : hayStack) {
	  if (needle == hay)
	    hasFound = true;
	}
	if (hasFound)
	  break;
      }
      if (!hasFound) {
	remainingTraces.push_back(needle);
      }
    }
    return remainingTraces;
  }


  Polygon2D createPolygon(const Trace::TracePointerVector& i_vector) {
    Polygon2D polygon;
    // for each trace
    for (const auto& trace : i_vector) {
      std::vector<Point2D> points = trace->estimateTrace(100);
      // add all points to the polygon
      for (const auto& point : points) {
	polygon.addPoint2D(point);
      }
    }
    return polygon;
  }


  bool isInside(const TraceSection& i_larger,
                const Trace::TracePointer& i_trace) {
    Polygon2D polygon = createPolygon(i_larger);
    std::vector<Point2D> points = i_trace->estimateTrace(100);
    for (const auto& point : points) {
      if (!polygon.isInside(point)) {
        return false;
      }
    }
    return true;
  }


  bool isInside(const Trace::TracePointerVector& i_bigger,
                const Trace::TracePointerVector& i_smaller) {
    /// create a Polygon2D
    Polygon2D polygon = createPolygon(i_bigger);
    /// for each Trace in the smaller vector
    for (const auto& trace : i_smaller) {
      /// estimate the trace
      std::vector<Point2D> points = trace->estimateTrace(100);
      /// then check if one of the points find outside of the polygon
      for (const auto& point : points) {
	if (!polygon.isInside(point)) {
	  LOG_DEBUG("This Point is not inside: " << point.x << " , " << point.y);
	  return false;
	}
      }
    }
    return true;
  }


  TraceSection getInsideTraces(const TraceSection& i_hayStack,
                               const TraceSection& i_biggerSection) {
    TraceSection needles;
    for (const auto& trace : i_hayStack) {
      if (isInside(i_biggerSection, trace)) {
	needles.push_back(trace);
      }
    }
    return needles;
  }


  TraceSections getInsideSections(const TraceSection& i_biggerSection,
				  const TraceSections& i_sections) {
    TraceSections areInside;
    for (const auto& section : i_sections) {
      if (isInside(i_biggerSection,
                   section)) {
	areInside.push_back(section);
      }
    }
    return areInside;
  }


  // could also be used with std::sort to sort TraceSections based on surface
  bool sortSmallToBig(const TraceSection& i_lhs,
		   const TraceSection& i_rhs) {
    Polygon2D lhsPolygon = createPolygon(i_lhs);
    Polygon2D rhsPolygon = createPolygon(i_rhs);
    return lhsPolygon.getSurface() < rhsPolygon.getSurface();
  }


  bool sortBigToSmall(const TraceSection& i_lhs,
		      const TraceSection& i_rhs) {
    return sortSmallToBig(i_rhs, i_lhs);
  }


  TraceSections::iterator
  getIndependendSections(const TraceSections::iterator& i_begin,
			 const TraceSections::iterator& i_end) {
    // determine if the current section is within another
    auto endSectionIterator = i_begin+1;
    bool isIndependent = true;
    while (isIndependent && endSectionIterator != i_end) {
      for (int i = 0;
	   i < std::distance(i_begin, endSectionIterator);
	   ++i) {
	if (isInside(*endSectionIterator, *(i_begin + i))) {
	  isIndependent = false;
	  break;
	}
      }
      if (isIndependent) {
	++endSectionIterator;
      }
    }
    return endSectionIterator;
  }

}  // namespace Section
