#include <macroHeader.h>
#include <RotationTraceCalculator.h>
#include <LineTraceCalculator.h>
#include <RotationTrace.h>
#include <JointController.h>
#include <BaseJoint.h>

RotationTraceCalculator::RotationTraceCalculator()
    : LineTraceCalculator()
{}


RotationTraceCalculator::RotationTraceCalculator(
    JointController* i_jointControllerPointer)
    : LineTraceCalculator(i_jointControllerPointer)
{}

RotationTraceCalculator::RotationTraceCalculator(
    JointController* i_jointControllerPointer,
    const traceType& i_tolerance)
    : LineTraceCalculator(i_jointControllerPointer, i_tolerance)
{}

void RotationTraceCalculator::calculateTrace(
    const RotationTrace* i_rotationTrace,
    Point2D& i_currentPosition) {
  if (i_rotationTrace->getTraceType() != Curve)
    LOG_ERROR("Rotational trace calculator only works on rotational traces!"<<
              "\nvalue: " << i_rotationTrace->getTraceType() <<
              " curve value: " << Curve);
  std::vector<RotationTrace> traces = i_rotationTrace->getNecessaryTraces();
  LOG_DEBUG("Number of traces: " << traces.size());

  Point2D startPointOfThisTrace;
  for (std::vector<RotationTrace>::const_iterator itr = traces.begin();
      itr != traces.end();
      itr++) {
    startPointOfThisTrace = i_currentPosition;

    LOG_DEBUG("Arc endPoint x, y: " << itr->getArc().getEndPoint().x <<
              ", " << itr->getArc().getEndPoint().y);

    LineTraceCalculator::calculateTrace(&(*itr), i_currentPosition);

    if (std::next(itr) != traces.end()) {
      LOG_DEBUG("Setting another translation step!");
      std::string direction =
          itr->getTranslationDirectionToEndPoint(startPointOfThisTrace);

      // Predict the step
      getJointController()->getJoint(Translational)->
          predictStep(i_currentPosition, direction);

      if (getWriteLog())
        writeToStepLog(direction, 1, i_currentPosition);

      LOG_INFO("Translating: " << direction);
      // Correct the rotation, if nessesary!
      bool hasCorrectionSteps = correctTranslation(&(*std::next(itr)), i_currentPosition);
      LOG_DEBUG("Has correction steps: " << std::boolalpha <<
                hasCorrectionSteps);
      getJointController()->moveStep(
          getJointController()->getJoint(Translational),
          direction,
          hasCorrectionSteps);
    }
  }
}
