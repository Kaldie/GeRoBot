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


Trace::TracePointerVector SequentialTraceCalculator::getOrderVector() {
  LOG_ERROR("Not implemented yet");
}


Trace::TracePointerVector SequentialTraceCalculator::
getAbuttingTraces(const Trace::TracePointer& i_trace) const {
  Trace::TracePointerVector abuttingVector;
  LOG_DEBUG("getAbuttingTraces()");
  for (auto& trace : m_traceVector) {
    if (isAbutting(i_trace, trace)) {
      LOG_DEBUG("Found an abutting trace");
      abuttingVector.push_back(trace);
    }
  }
  return abuttingVector;
}


void SequentialTraceCalculator::
getAllConnected(const Trace::TracePointer& i_trace,
		Trace::TracePointerVector* i_connectedTraces) const {
  LOG_DEBUG("getAllConnected()");
  // get ALL the abutting traces
  for (const auto& trace : getAbuttingTraces(i_trace)) {
    LOG_DEBUG("Found a abutting trace");
    // if the trace is NOT in the i_connectedTraces
    if (std::find(i_connectedTraces->begin(),
		  i_connectedTraces->end(),
		  trace) == i_connectedTraces->end()) {
      LOG_DEBUG("Found a trace which we didnt look at!");
      // add the trace to the connectedTraces
      i_connectedTraces->push_back(trace);
      // preform recursion to get ALL the connected traces
      getAllConnected(trace, i_connectedTraces);
    }
  }
}


bool SequentialTraceCalculator::
isAbutting(const Trace::TracePointer& i_first,
	   const Trace::TracePointer& i_second) const {
  // if the trace is equal check if the start and end are at the same spot
  // then they are abutting otherwise nope
  if (i_first == i_second) {
    if (i_first->getStartPoint() == i_first->getEndPoint()) {
      return true;
    } else {
      return false;
    }
  }
  // if they are different, check if the minimal distance 0-ish
  if (getMinimalDistance(i_first, i_second) < 0.0001) {
    return true;
  } else {
    return false;
  }
}


Trace::TracePointer SequentialTraceCalculator::
getNearestTrace(const Trace::TracePointer& i_trace,
		const Trace::TracePointerVector&) {
  Trace::TracePointer nearest(nullptr);
  traceType currentDistance(std::numeric_limits<traceType>::infinity());
  for (auto& trace : m_traceVector) {
    if (currentDistance > getMinimalDistance(i_trace, trace)) {
      nearest = trace;
      currentDistance = getMinimalDistance(i_trace, trace);
    }
  }
  return nearest;
}


traceType SequentialTraceCalculator::
getMinimalDistance(const Trace::TracePointer& i_first,
		   const Trace::TracePointer& i_second) const {
  std::vector<traceType> distances;
  distances.push_back(Magnitude(i_first->getStartPoint() - i_second->getStartPoint()));
  distances.push_back(Magnitude(i_first->getEndPoint() - i_second->getEndPoint()));
  distances.push_back(Magnitude(i_first->getStartPoint() - i_second->getEndPoint()));
  distances.push_back(Magnitude(i_first->getEndPoint() - i_second->getStartPoint()));
  return *std::min_element(distances.begin(), distances.end());
}


bool SequentialTraceCalculator::
isClosedSection(const Trace::TracePointerVector& i_vector) const {
  Trace::TracePointerVector abuttingTraces;
  for ( const auto& trace : i_vector) {
    LOG_DEBUG("Testing if an trace is fully connected");
    abuttingTraces = getAbuttingTraces(trace);
    if (abuttingTraces.size() < 1) {
      LOG_DEBUG("A trace is connected by: " << abuttingTraces.size()
		<< " << connection broke");
      LOG_DEBUG("Start point of this trace: " << trace->getStartPoint().x
		<< " , " << trace->getStartPoint().y);
      LOG_DEBUG("End point of this trace: " << trace->getEndPoint().x
		<< " , " << trace->getEndPoint().y);
      return false;
    }
    if (abuttingTraces.size() == 1) {
      LOG_DEBUG("Trace has 1 partner, could be not connected!");
      if (abuttingTraces[0] != trace) {
	LOG_DEBUG("But it is not itself!");
	LOG_DEBUG("Start point of this trace: " << trace->getStartPoint().x
		  << " , " << trace->getStartPoint().y);
	LOG_DEBUG("End point of this trace: " << trace->getEndPoint().x
		  << " , " << trace->getEndPoint().y);
	return false;
      } else {
	LOG_DEBUG("It is itself, so its fine!");
      }
    }
  }
  return true;
}



bool SequentialTraceCalculator::
isPartOfClosedSection(const Trace::TracePointer& i_trace) const {
  Trace::TracePointerVector connectedTraces;
  getAllConnected(i_trace, &connectedTraces);
  return isClosedSection(connectedTraces);
}


void SequentialTraceCalculator::
createPolygon(const Trace::TracePointerVector& i_vector,
              Polygon2D* i_polygon) const {
  if (!i_polygon) {
    return;
  }
  if (!i_polygon->isEmpty()) {
    LOG_WARNING("Cleaning out the non-empty polygon!!");
    Polygon2D emptyPoly;
    *i_polygon = emptyPoly;
  }
  // for each trace
  for (const auto& trace : i_vector) {
    std::vector<Point2D> points = trace->estimateTrace(100);
    // add all points to the polygon
    for (const auto& point : points) {
      i_polygon->addPoint2D(point);
    }
  }
}



bool SequentialTraceCalculator::
isInside(const Trace::TracePointerVector& i_larger,
         const Trace::TracePointerVector& i_smaller) const {
  bool isThisInside = true;
  Trace::TracePointerVector::const_iterator currentTrace = i_smaller.begin();
  do {
    isThisInside &= isInside(i_larger, *currentTrace);
    ++currentTrace;
  } while(isThisInside && currentTrace != i_smaller.end());
  return isThisInside;
}


bool SequentialTraceCalculator::
isInside(const Trace::TracePointerVector& i_vector,
         const Trace::TracePointer& i_trace) const {
  Polygon2D polygon;
  createPolygon(i_vector, &polygon);
  if (!polygon.isInside(i_trace->getStartPoint()))
    return false;
  if (!polygon.isInside(i_trace->getEndPoint()))
    return false;

  if (RotationTrace::RotationTracePointer rotationTrace =
      std::dynamic_pointer_cast<RotationTrace>(i_trace)) {
    std::vector<Point2D> points = rotationTrace->estimateTrace(100);
    auto pointItr = points.begin();
    while (points.end() != pointItr) {
      if (!polygon.isInside(*pointItr))
        return false;
      ++pointItr;
    }
  }
  return true;
}

int SequentialTraceCalculator::
getClosedSections(SequentialTraceCalculator::TraceSections* i_closedSections) const {
  LOG_DEBUG("getClosedSections");
  Trace::TracePointerVector traces = m_traceVector;
  Trace::TracePointer currentTrace;
  int sections(0);
  do {
    Trace::TracePointerVector connected;
    currentTrace = traces.back();
    traces.pop_back();
    getAllConnected(currentTrace,
		    &connected);
    if (connected.size() > 0) {
      // indicate that we have found 1
      ++sections;
      // remove all connected traces from the traces vector
      for (const auto& connectedTrace : connected) {
        LOG_DEBUG("Size of traces before: " << traces.size());
        if (std::find(traces.begin(), traces.end(), connectedTrace) == traces.end()) {
          LOG_DEBUG("could not find trace!");
          continue;
        }
        traces.erase(std::remove(traces.begin(), traces.end(), connectedTrace));
        LOG_DEBUG("Size of traces after: " << traces.size());
      }
      // Add the connectedTraces to the closedSections vector
      i_closedSections->push_back(connected);
    }
  } while (traces.size() > 0);
  return sections;
}
