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
      m_jointController(nullptr),
      m_translationTolerance(0.01),
      m_rotationTolerance(0.01),
      m_writeLog(false),
      m_logFileName("./stepLog.log")
{}

BaseTraceCalculator::BaseTraceCalculator(JointController* i_jointController)
    : m_tolerance(0.01),
      m_jointController(i_jointController),
      m_translationTolerance(0.01),
      m_rotationTolerance(0.01),
      m_writeLog(false),
      m_logFileName("./stepLog.log") {
  setTolerances();
}

BaseTraceCalculator::BaseTraceCalculator(JointController* i_jointController,
                                         const double& i_tolerance)
    : m_tolerance(i_tolerance),
      m_jointController(i_jointController),
      m_translationTolerance(0.01),
      m_rotationTolerance(0.01),
      m_writeLog(false),
      m_logFileName("./stepLog.log") {
  setTolerances();
}

// assign operator
BaseTraceCalculator::BaseTraceCalculator(const BaseTraceCalculator& obj) {
  LOG_INFO("Copy constructor");
  m_jointController = obj.m_jointController;
  m_tolerance = obj.m_tolerance;
  m_writeLog = obj.m_writeLog;
  m_logFileName = obj.m_logFileName;
  m_rotationTolerance = obj.m_rotationTolerance;
  m_translationTolerance = obj.m_translationTolerance;
}


bool BaseTraceCalculator::hasJointController() const {
  if (m_jointController == nullptr)
    return false;
  else
    return true;
}

void BaseTraceCalculator::setTolerances() {
  if (hasJointController()) {
    try {
      m_rotationTolerance = m_jointController
          ->getJoint(Rotational)->getMovementPerStep();}
    catch(int e) {
      LOG_INFO("Could not find a rotaional joint" <<
               "tolerance is left at default!");}

    try {
      m_translationTolerance = m_jointController->
          getJoint(Translational)->getMovementPerStep();}
    catch(int e) {
      LOG_INFO("Could not find a translational joint," <<
               "tolerance is left at default!");}
  }
}


std::vector<int> BaseTraceCalculator::getNumberOfSteps(
    const Trace* i_trace,
    const Point2D& i_position) const {
  if (!hasJointController())
    LOG_ERROR("Does not have a joint controller!");
  
  Point2D endPoint = i_trace->getEndPoint();
  LOG_INFO("Translational movement per step: "<<
           static_cast<double>(
               m_jointController->getJoint(Translational)->
               getMovementPerStep()));

  LOG_INFO("Rotationalal movement per step: "<<
           m_jointController->getJoint(Rotational)->getMovementPerStep());

  // Magnitude difference / movement per step of Translational joint
  int numberOfTranslationSteps =
      std::abs(Magnitude(i_position)-Magnitude(endPoint))/
      (m_jointController->getJoint(Translational)->getMovementPerStep());

  // Rotational difference / movement per step of Rotational joint
  int numberOfRotationSteps =
      (std::abs(i_position.getAlpha()-endPoint.getAlpha())*(180/PI))/
      (m_jointController->getJoint(Rotational)->getMovementPerStep());

   return std::vector<int> {numberOfRotationSteps, numberOfTranslationSteps} ;
}


void BaseTraceCalculator::writeToStepLog(const std::string i_direction,
                                         const int i_numberOfSteps) const {
  std::ofstream stepLogFile(m_logFileName, std::ios::app);
  if (stepLogFile.good())
    stepLogFile << i_direction << ", " << i_numberOfSteps << std::endl;
}

bool BaseTraceCalculator::shouldTranslate(const Trace& i_trace,
                                          const Point2D &i_point2D) const {
  /*
    Translate if the magnitude differs between the current point and the endpoint.
  */	
  double pointMagnitude = Magnitude(i_point2D);
  double endPointMagnitude = Magnitude(i_trace.getEndPoint());
  double difference = std::abs(pointMagnitude-endPointMagnitude);
  bool shouldTranslate;
  
  if (difference>m_translationTolerance)
    shouldTranslate = true;

  else if (difference >
           (getJointController()->
            getJoint(Translational)->
            getMovementPerStep() / 2.0))
    shouldTranslate = true;

  else
    shouldTranslate = false;

#ifdef DEBUG
  if (!shouldTranslate) {
    LOG_INFO("Not translating!! ");
    LOG_INFO("Current magnitude: " << pointMagnitude << ". ");
    LOG_INFO("Wanted magnitude: " << endPointMagnitude);
    LOG_INFO("diff: " << pointMagnitude-endPointMagnitude);
  }
#endif
  return shouldTranslate;
}

bool BaseTraceCalculator::shouldRotate(const Trace& i_trace,
                                       const Point2D &i_point2D) const {
  double pointAngle = i_point2D.getAlpha()*180/PI;
  double endPointAngle = i_trace.getEndPoint().getAlpha()*180/PI;
  double difference = std::abs(pointAngle-endPointAngle);
  bool shouldRotate;

  if (difference>m_rotationTolerance)
    shouldRotate = true;
  else if (difference>
           (getJointController()->
            getJoint(Rotational)->
            getMovementPerStep()/2.0))
    shouldRotate = true;
  else
    shouldRotate = false;

#ifdef DEBUG
  if (!shouldRotate) {
    LOG_INFO("Not rotating!!");
    LOG_INFO("current angle: " << pointAngle << " ");
    LOG_INFO("wanted angle: " << endPointAngle);
    LOG_INFO("diff: " << difference);
  }
#endif
  return shouldRotate;
}

void BaseTraceCalculator::calculateTrace(const Trace* i_trace,
                                         Point2D& i_startPoint) {
  if (!hasJointController()) {
    LOG_ERROR("Does not have a joint controller!");
  }
  LOG_INFO("Current robot position: " <<
           i_startPoint.x << ", " << i_startPoint.y);

  LOG_INFO("Going to position: " <<
           i_trace->getEndPoint().x << ", " << i_trace->getEndPoint().y);

  std::string rotationDirection = i_trace->
      getRotationDirectionToEndPoint(i_startPoint);

  std::string translationDirection = i_trace->
      getTranslationDirectionToEndPoint(i_startPoint);

  std::vector<int> numberOfSteps = getNumberOfSteps(i_trace, i_startPoint);
  LOG_INFO("\nNumber of rotation steps: " << numberOfSteps[0] << "\n"
           "Number of translation steps: " << numberOfSteps[1]);

  if (numberOfSteps[0] > 0) {
    getJointController()->getJoint(Rotational)->predictSteps(
        i_startPoint,
        rotationDirection,
        numberOfSteps[0]);

    getJointController()->moveSteps(
        getJointController()->getJoint(Rotational),
        rotationDirection,
        numberOfSteps[0],
        true);
  }

  if (numberOfSteps[1] > 0) {
    getJointController()->getJoint(Translational)->
        predictSteps(i_startPoint,
                     translationDirection,
                     numberOfSteps[1]);

    getJointController()->moveSteps(
        getJointController()->getJoint(Translational),
        translationDirection,
        numberOfSteps[1],
        true);
  }
  }
