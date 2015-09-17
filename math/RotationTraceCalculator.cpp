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

    if (std::next(itr) != traces.end()) {
      LOG_DEBUG("Setting another translation step!");
      std::string direction =
          itr->getTranslationDirectionToEndPoint(startPointOfThisTrace);

      // Predict the step
      m_robot->prepareSteps(direction ,1);
      if (getWriteLog())
        writeToStepLog(direction, 1, m_robot->getVirtualPosition());
      LOG_INFO("Translating: " << direction);
      // Correct the rotation, if nessesary!
      bool hasCorrectionSteps = correctTranslation(*std::next(itr));
      LOG_DEBUG("Has correction steps: " << std::boolalpha <<
                hasCorrectionSteps);
    }
  }
}
