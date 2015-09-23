#include <macroHeader.h>
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

void RotationTraceCalculator::calculateTrace
(const RotationTrace& i_rotationTrace) {
  if (i_rotationTrace.getTraceType() != Trace::Curve)
    LOG_ERROR("Rotational trace calculator only works on rotational traces!"<<
              "\nvalue: " << i_rotationTrace.getTraceType() <<
              " curve value: " << Trace::Curve);
  std::vector<RotationTrace> traces = i_rotationTrace.getNecessaryTraces();
  LOG_DEBUG("Number of traces: " << traces.size());

  Point2D startPointOfThisTrace;
  for (std::vector<RotationTrace>::const_iterator itr = traces.begin();
       itr != traces.end();
       itr++) {
    startPointOfThisTrace = m_robot->getVirtualPosition();

    LOG_DEBUG("Arc endPoint x, y: " << itr->getArc().getSecondPoint().x <<
              ", " << itr->getArc().getSecondPoint().y);

    LineTraceCalculator::calculateTrace(*itr);
    // if there will be a next one
    if (std::next(itr) != traces.end()) {
      auto nextTrace = std::next(itr);
      // what is the current translation direction
      std::string translationDirection =
        itr->getTranslationDirectionToEndPoint(startPointOfThisTrace);
      LOG_DEBUG("Current Translation direction " << translationDirection);

      // what will be the current rotation direction
      std::string rotationDirection =
        nextTrace->getRotationDirectionToEndPoint(m_robot->getVirtualPosition());
      LOG_DEBUG("Current Rotation direction " << rotationDirection);
      /// Set one rotation step
      if (getWriteLog())
        writeToStepLog(rotationDirection, 1, m_robot->getVirtualPosition());
      m_robot->prepareSteps(rotationDirection, 1);

      // do set translation step untill the correction will be the same as the direcion we are stepping now
      std::string currentCorrectionTranslationDirection;
      Point2D intersectingPoint;
      do {
        LOG_DEBUG("Setting another translation step!");
        // Prepare a set
        m_robot->prepareSteps(translationDirection ,1);
        if (getWriteLog())
          writeToStepLog(translationDirection, 1, m_robot->getVirtualPosition());
        LOG_INFO("Translating: " << translationDirection);
        // get current intersecting point
        intersectingPoint = nextTrace->intersectingPoint(m_robot->getVirtualPosition());
        // determine the translation direction to that intersecting point
        currentCorrectionTranslationDirection = nextTrace->getTranslationDirection
          (m_robot->getVirtualPosition(), intersectingPoint);
        LOG_DEBUG("Current correction direction :" << currentCorrectionTranslationDirection);
        // if that is the same, stop
      } while (currentCorrectionTranslationDirection != translationDirection);
    }
  }
}
