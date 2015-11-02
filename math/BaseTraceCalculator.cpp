// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <Point2D.h>
#include <BaseTraceCalculator.h>
#include <Trace.h>
#include <BaseMotor.h>
#include <BaseJoint.h>
#include <JointController.h>


BaseTraceCalculator::BaseTraceCalculator()
    : m_tolerance(0.01),
      m_robot(nullptr),
      m_translationTolerance(0.01),
      m_rotationTolerance(0.01),
      m_writeLog(false),
      m_logFileName("./stepLog.log")
{}

BaseTraceCalculator::BaseTraceCalculator
(Robot* i_robotPointer)
    : m_tolerance(0.01),
      m_robot(i_robotPointer),
      m_translationTolerance(0.01),
      m_rotationTolerance(0.01),
      m_writeLog(false),
      m_logFileName("./stepLog.log") {
  setTolerances();
}

BaseTraceCalculator::BaseTraceCalculator
(Robot* i_robotPointer,
 const traceType& i_tolerance)
    : m_tolerance(i_tolerance),
      m_robot(i_robotPointer),
      m_translationTolerance(0.01),
      m_rotationTolerance(0.01),
      m_writeLog(false),
      m_logFileName("./stepLog.log") {
  setTolerances();
}


bool BaseTraceCalculator::hasRobot() const {
  if (m_robot == nullptr)
    return false;
  else
    return true;
}

void BaseTraceCalculator::setTolerances() {
  if (hasRobot()) {
   m_rotationTolerance = m_robot->getMovementPerStep(Rotational) / 1.0;
   m_translationTolerance = m_robot->getMovementPerStep(Translational) / 2.0;
  }
  m_tolerance = std::min(m_rotationTolerance, m_translationTolerance);
}


std::vector<int> BaseTraceCalculator::getNumberOfSteps
(const Trace& i_trace,
 const Point2D& i_position) const {
  if (!hasRobot())
    LOG_ERROR("Does not have a joint controller!");

  Point2D endPoint = i_trace.getEndPoint();
  LOG_INFO
    ("Translational movement per step: "<<
     m_robot->getMovementPerStep(Translational));

  LOG_INFO
    ("Rotationalal movement per step: "<<
     m_robot->getMovementPerStep(Rotational));

  // Magnitude difference / movement per step of Translational joint
  int numberOfTranslationSteps =
      std::abs(Magnitude(i_position)-Magnitude(endPoint))/
      (m_robot->getMovementPerStep(Translational));

  // Rotational difference / movement per step of Rotational joint
  int numberOfRotationSteps =
    (std::abs(i_position.getAlpha()-endPoint.getAlpha())*(180/PI)) /
    m_robot->getMovementPerStep(Rotational);
  LOG_DEBUG("Number of translation steps: " << numberOfTranslationSteps);
  LOG_DEBUG("Number of rotation steps: " << numberOfRotationSteps);
  return std::vector<int> {numberOfRotationSteps, numberOfTranslationSteps} ;
}


void BaseTraceCalculator::writeToStepLog(const std::string& i_direction,
                                         const int& i_numberOfSteps,
                                         const Point2D& i_newPos) const {
  std::ofstream stepLogFile(m_logFileName, std::ios::app);
  if (stepLogFile.good())
    stepLogFile << i_direction << ", " << i_numberOfSteps <<  ", " <<
        i_newPos.x <<  ", " << i_newPos.y << std::endl;
}


bool BaseTraceCalculator::shouldTranslate(const Trace& i_trace,
                                          const Point2D &i_point2D) const {
  /*
    Translate if the magnitude differs between the current point and the endpoint.
  */
#ifdef DEBUG
  traceType pointMagnitude = Magnitude(i_point2D);
  traceType endPointMagnitude = Magnitude(i_trace.getEndPoint());
#endif
  traceType difference = std::abs(Magnitude(i_point2D) -
                                  Magnitude(i_trace.getEndPoint()));
  bool shouldTranslate;
  if (difference > m_translationTolerance)
    shouldTranslate = true;
  else
    shouldTranslate = false;
#ifdef DEBUG
    LOG_INFO("Not translating!! ");
    LOG_INFO("Current magnitude: " << pointMagnitude << ". ");
    LOG_INFO("Wanted magnitude: " << endPointMagnitude);
    LOG_INFO("diff: " << difference);
    LOG_INFO("translation tolerance: " << m_translationTolerance);
#endif
  return shouldTranslate;
}


bool BaseTraceCalculator::shouldRotate(const Trace& i_trace,
                                       const Point2D &i_point2D) const {
  traceType difference = std::abs(i_point2D.getAlpha()-
                                  i_trace.getEndPoint().getAlpha());
  difference *= 180 / PI;
  bool shouldRotate;

  if (difference > m_rotationTolerance)
    shouldRotate = true;
  else
    shouldRotate = false;

#ifdef DEBUG
  if (!shouldRotate) {
    LOG_INFO("Not rotating!!");
    LOG_INFO("current angle: " << i_point2D.getAlpha() * 180 / PI << " ");
    LOG_INFO("wanted angle: " << i_trace.getEndPoint().getAlpha() * 180 / PI);
    LOG_INFO("diff: " << difference);
  }
#endif
  return shouldRotate;
}


void BaseTraceCalculator::calculateTrace(const Trace& i_trace) {
  if (!hasRobot()) {
    LOG_ERROR("Does not have a joint controller!");
  }
  Point2D currentVirtualPosition = m_robot->getVirtualPosition();
  LOG_INFO("Current robot position: " << currentVirtualPosition.x <<
           ", " << currentVirtualPosition.y);
  LOG_INFO("Going to position: " << i_trace.getEndPoint().x <<
           ", " << i_trace.getEndPoint().y);
  // Get rotation direction
  std::string rotationDirection = i_trace.
      getRotationDirectionToEndPoint(currentVirtualPosition);
  // Get translation direction
  std::string translationDirection = i_trace.
      getTranslationDirectionToEndPoint(currentVirtualPosition);
  // get number of steps
  std::vector<int> numberOfSteps =
    getNumberOfSteps(i_trace, currentVirtualPosition);
  // set steps
  if (numberOfSteps[0] > 0) {
    m_robot->prepareSteps(rotationDirection, numberOfSteps[0]);
  }
  if (numberOfSteps[1] > 0) {
    m_robot->prepareSteps(translationDirection, numberOfSteps[1]);
  }
}
