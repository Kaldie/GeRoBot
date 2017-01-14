#include <macroHeader.h>
#include <Robot.h>
#include <RotationTraceCalculator.h>
#include <LineTraceCalculator.h>
#include <RotationTrace.h>
#include <JointController.h>
#include <BaseJoint.h>

RotationTraceCalculator::RotationTraceCalculator()
    : LineTraceCalculator()
{}


RotationTraceCalculator::RotationTraceCalculator
( Robot* i_robotPointer)
    : LineTraceCalculator(i_robotPointer)
{}


RotationTraceCalculator::RotationTraceCalculator
(Robot* i_robotPointer,
 const traceType& i_tolerance)
    : LineTraceCalculator(i_robotPointer, i_tolerance)
{}


void RotationTraceCalculator::calculateTrace(const Trace& i_trace) {
  if (i_trace.getTraceType() != Trace::Curve) {
    LOG_ERROR("Only supply rotational traces to this calculator!");
  }
  const RotationTrace& rotationTrace = static_cast<const RotationTrace&>(i_trace);
  std::vector<RotationTrace> traces = rotationTrace.getNecessaryTraces();
  LOG_DEBUG("Number of traces: " << traces.size());
  Point2D startPointOfThisTrace;
  for (std::vector<RotationTrace>::const_iterator itr = traces.begin();
       itr != traces.end();
       itr++) {
    LOG_DEBUG("Arc endPoint x, y: " << itr->getArc().getSecondPoint().x <<
              ", " << itr->getArc().getSecondPoint().y);
    // This start position will be used to determine the current translation direction
    startPointOfThisTrace = m_robot->getVirtualPosition();
    // calculate the trace
    LineTraceCalculator::calculateTrace(*itr);
    // if there will be a next one
    if (std::next(itr) != traces.end()) {
      // Additional steps will be set to ensure that the robot will follow the right path
      ensureProperMovement(traces, itr, startPointOfThisTrace);
    }
  }
}


void RotationTraceCalculator::ensureProperMovement(const std::vector<RotationTrace>& i_traces,
                                                   const std::vector<RotationTrace>::const_iterator& i_traceIterator,
                                                   const Point2D& i_startPosition) {
  auto nextTrace = std::next(i_traceIterator);
  if (nextTrace->isOriginWithinCircle()) {
    return;
  }
  // what is the current translation direction
  std::string translationDirection =
     nextTrace->getTranslationDirectionToEndPoint(i_startPosition);
  LOG_DEBUG("Current Translation direction " << translationDirection);
  // Set a rotation step if appropate and let us know if we need to continue
  ensureRotationMovement(i_traces, nextTrace);
  // Now ensure translation steps are set to put it at an appropate position
  if (translationDirection != "STABLE") {
    ensureTranslationMovement(nextTrace, translationDirection);
  }
}


bool RotationTraceCalculator::ensureRotationMovement(const std::vector<RotationTrace>& i_traces,
                                                     const std::vector<RotationTrace>::const_iterator& i_trace) {
   /// Determine if it is necessary to set a rotation step at all
  if (!shouldRotateAtAll(i_traces, i_trace, m_robot->getVirtualPosition())) {
    // if not, it is sure that we do not have to adjust the position here
    return false;
  }

  /// Determine if it is necessary to set a rotation step
  if (shouldRotate(*i_trace, m_robot->getVirtualPosition())) {
    // what will be the current rotation direction
    std::string rotationDirection =
      i_trace->getRotationDirectionToEndPoint(m_robot->getVirtualPosition());
    LOG_DEBUG("Current Rotation direction " << rotationDirection);
    /// Set one rotation step
    if (getWriteLog())
      writeToStepLog(rotationDirection, 1, m_robot->getVirtualPosition());
    m_robot->prepareSteps(rotationDirection, 1);
  }
  return true;
}


void RotationTraceCalculator::
ensureTranslationMovement(const std::vector<RotationTrace>::const_iterator& i_trace,
                          const std::string& i_requiredDirection) {
  /*
    do translation steps untill the correction will be
    the same as the direcion we are stepping now
  */
  std::string currentCorrectionTranslationDirection;
  Point2D intersectingPoint;
  do {
    LOG_DEBUG("Setting another translation step: " << i_requiredDirection << "!");
    // Prepare a step
    m_robot->prepareSteps(i_requiredDirection ,1);
    if (getWriteLog())
      writeToStepLog(i_requiredDirection, 1, m_robot->getVirtualPosition());
    try {
      // get current intersecting point
      intersectingPoint = i_trace->intersectingPoint(m_robot->getVirtualPosition());
    } catch (std::runtime_error) {
      // if no intersecting point can be determined, we're done
      break;
    }
    // determine the translation direction to that intersecting point
    currentCorrectionTranslationDirection = i_trace->getTranslationDirection
      (m_robot->getVirtualPosition(), intersectingPoint);
    LOG_DEBUG("Current correction direction :" << currentCorrectionTranslationDirection);
    // if that is the same, stop
  } while (currentCorrectionTranslationDirection != i_requiredDirection);
}


bool RotationTraceCalculator::
shouldRotateAtAll(const std::vector<RotationTrace>& i_traces,
                  const std::vector<RotationTrace>::const_iterator& i_currentTrace,
                  const Point2D& i_currentPosition) {
  bool willRotate = false;
  std::vector<RotationTrace>::const_iterator iterator = i_currentTrace;
  while (iterator != i_traces.end() && !willRotate) {
    willRotate |= shouldRotate(*iterator, m_robot->getVirtualPosition());
    ++iterator;
  }
  return willRotate;
}
