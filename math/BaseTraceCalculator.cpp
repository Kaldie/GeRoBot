// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <Point2D.h>
#include <BaseTraceCalculator.h>
#include <Robot.h>
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
    m_rotationTolerance = m_robot->getMovementPerStep(BaseJoint::Rotational) * 2.0;
    m_translationTolerance = m_robot->getMovementPerStep(BaseJoint::Translational) * 2.0;
  }
  m_tolerance = std::min(m_rotationTolerance, m_translationTolerance);
}


std::vector<int> BaseTraceCalculator::getNumberOfSteps
(const Trace& i_trace,
 const Point2D& i_position) const {
  if (!hasRobot()) {
    LOG_ERROR("Does not have a joint controller!");
  }
  Point2D endPoint = i_trace.getEndPoint();
  LOG_INFO("Translational movement per step: "<<
           m_robot->getMovementPerStep(BaseJoint::Translational));
  LOG_INFO ("Rotationalal movement per step: "<<
            m_robot->getMovementPerStep(BaseJoint::Rotational));
  // Magnitude difference / movement per step of Translational joint
  int numberOfTranslationSteps =
      std::abs(magnitude(i_position)-magnitude(endPoint))/
    (m_robot->getMovementPerStep(BaseJoint::Translational));
  // Rotational difference / movement per step of Rotational joint
  int numberOfRotationSteps =
    (std::abs(i_position.getAlpha()-endPoint.getAlpha())) /
    m_robot->getMovementPerStep(BaseJoint::Rotational);
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
  traceType pointMagnitude = magnitude(i_point2D);
  traceType endPointMagnitude = magnitude(i_trace.getEndPoint());
  traceType difference = std::abs(endPointMagnitude - endPointMagnitude);
  LOG_INFO("Current magnitude: " << pointMagnitude << ". ");
  LOG_INFO("Wanted magnitude: " << endPointMagnitude);
  LOG_INFO("diff: " << difference);
  LOG_INFO("translation tolerance: " << m_translationTolerance);
  if (difference > m_translationTolerance) {
    LOG_INFO("Not translating!! ");
    return true;
  } else {
    return false;
  }
}


bool BaseTraceCalculator::shouldRotate(const Trace& i_trace,
                                       const Point2D &i_point2D) const {
  traceType difference = std::abs(i_point2D.getAlpha()-
                                  i_trace.getEndPoint().getAlpha());
  LOG_INFO("current angle: " << i_point2D.getAlpha() * 180 / PI << " ");
  LOG_INFO("wanted angle: " << i_trace.getEndPoint().getAlpha() * 180 / PI);
  LOG_INFO("diff: " << difference * 180 / PI);
  if (difference > m_rotationTolerance) {
    return true;
  } else {
    LOG_INFO("Not rotating!!");
    return false;
  }
}


void BaseTraceCalculator::calculateTrace(const Trace& i_trace) {
  if (!hasRobot()) {
    LOG_ERROR("Does not have a joint controller!");
  }
  Point2D currentVirtualPosition = m_robot->getVirtualPosition();
  LOG_INFO("Current Virtual Position: " << currentVirtualPosition.x <<
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
    if (m_writeLog) {
      writeToStepLog(rotationDirection, numberOfSteps[0],m_robot->getVirtualPosition());
    }
  }
  if (numberOfSteps[1] > 0) {
    m_robot->prepareSteps(translationDirection, numberOfSteps[1]);
    if (m_writeLog) {
      writeToStepLog(translationDirection, numberOfSteps[1],m_robot->getVirtualPosition());
    }
  }
}
