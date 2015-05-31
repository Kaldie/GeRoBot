// Copyright [2014] Ruud Cools

#include <macroHeader.h>
#include <Point2D.h>
#include <PinState.h>
#include <BaseJoint.h>
#include <JointController.h>
#include <LineTraceCalculator.h>
#include <Trace.h>


LineTraceCalculator::LineTraceCalculator()
    : BaseTraceCalculator(),
      m_hasRotated(true),
      m_hasTranslated(true)
{}


LineTraceCalculator::LineTraceCalculator(JointController* i_jointController)
    : BaseTraceCalculator(i_jointController),
      m_hasRotated(true),
      m_hasTranslated(true)
{}


LineTraceCalculator::LineTraceCalculator(JointController* i_jointController,
                                         const float& i_tolerance)
    : BaseTraceCalculator(i_jointController,
                          i_tolerance),
      m_hasRotated(true),
      m_hasTranslated(true)
{}

// assign operator
LineTraceCalculator::LineTraceCalculator(const LineTraceCalculator& rhs) {
  LOG_INFO("Copy constructor");
  this->setJointController(rhs.getJointController());
  this->setTolerance(rhs.getTolerance());
}


void LineTraceCalculator::calculateTrace(const Trace* i_trace,
                                         Point2D& i_point) {
  // Setting the capacity of the vector to hold all the data
  std::vector<int> numberOfSteps = getNumberOfSteps(i_trace, i_point);
  std::vector<int>::iterator maximumNumberOfSteps =
      std::max_element(numberOfSteps.begin(),
                       numberOfSteps.end());
  int i = 0;
  bool hasStepped(true);
  do {
    hasStepped = calculateStep(i_trace, i_point);
    i++;
  }while(hasStepped);  // and i<1000);
}


bool LineTraceCalculator::calculateStep(const Trace* i_trace,
                                        Point2D& i_point2D) const {
  // keeping track if the robot is orded to translate or rotated
  bool hasStepped = false;
  // should rotate?
  if (shouldRotate(*i_trace, i_point2D)) {
    // add rotation to the delay list and correct translations
    prepareRotation(i_trace,
                    i_point2D);
    hasStepped = true;
  } else if (shouldTranslate(*i_trace, i_point2D)) {
    prepareTranslation(i_trace, i_point2D);
    hasStepped = true;
  }
  return hasStepped;
}


void LineTraceCalculator::prepareRotation(const Trace* i_trace,
                                          Point2D& i_point2D)const {
  // Rotating
  //    m_hasRotated=true;
  // Rotation direction
  std::string direction = i_trace->getRotationDirectionToEndPoint(i_point2D);

  // See where we end up after rotation
  getJointController()->getJoint(Rotational)->predictStep(i_point2D, direction);

  if (getWriteLog())
    writeToStepLog(direction, 1);

  // user needs to know :)
  LOG_INFO("Rotating: " << direction);

  // Correct the rotation
  bool hasCorrectionSteps = correctRotation(i_trace,
                                          i_point2D);

  getJointController()->moveStep(getJointController()->getJoint(Rotational),
                                 direction,
                                 hasCorrectionSteps);
}

void LineTraceCalculator::prepareTranslation(const Trace* i_trace,
                                             Point2D& i_point2D) const {
  //    m_hasTranslated=true;
  // identify the direction to move to
  std::string direction = i_trace->getTranslationDirectionToEndPoint(i_point2D);

  // Predict the step
  getJointController()->getJoint(Translational)->
      predictStep(i_point2D, direction);

  if (getWriteLog())
    writeToStepLog(direction, 1);

  LOG_INFO("Translating: " << direction);

  // Correct the rotation, if nessesary!
  bool hasCorrectionSteps = correctTranslation(i_trace, i_point2D);

  getJointController()->moveStep(getJointController()->getJoint(Translational),
                                 direction,
                                 hasCorrectionSteps);
}


bool LineTraceCalculator::correctRotation(const Trace* i_trace,
                                          Point2D& i_point2D) const {
  float jointPointDifference;
  const Point2D* destinationPoint;

  Point2D intersectingPoint;

  try {
    intersectingPoint = i_trace->intersectingPoint(i_point2D);
  }
  catch(std::runtime_error) {
    LOG_DEBUG("Found no intersection reverting the old position!");
    return false;
  }

  /* The distance to the enpoint after the correction is applied*/
  double distenceEndPointIntersectingPoint =
      Magnitude(intersectingPoint-i_trace->getEndPoint());

  /* The distance traveled in this correction*/
  double distanceBeginPointIntersectingPoint =
      Magnitude(intersectingPoint-i_point2D);

  if (distenceEndPointIntersectingPoint < distanceBeginPointIntersectingPoint) {
    /*if the distance needed in the next step is smaller then the distance we travel in this correction step
      Don't overshoot
    */
    jointPointDifference = Magnitude(i_trace->getEndPoint()-i_point2D);
    destinationPoint=&(i_trace->getEndPoint());
  } else {
    /*
      Otherwise we assume that there is enough distance, 
      so overshooting the correction decrease the absolute distance between the wanted and final result
      This is done by spreading out the error over both the positive as negative side of the line
    */
    jointPointDifference = distanceBeginPointIntersectingPoint;
    destinationPoint=&intersectingPoint;
    jointPointDifference*=2.0;
  }

  LOG_INFO("Destination point is: " <<
           destinationPoint->x << ", " << destinationPoint->y);

  int numberOfSteps = std::floor((jointPointDifference)/getJointController()->
                               getJoint(Translational)->getMovementPerStep());

  if (numberOfSteps>0) {
    std::string translationDirection=
        i_trace->getTranslationDirection(i_point2D, *destinationPoint);

    getJointController()->getJoint(Translational)->
        predictSteps(i_point2D, translationDirection, numberOfSteps);

    getJointController()->moveSteps(getJointController()->
                                    getJoint(Translational),
                                    translationDirection,
                                    numberOfSteps);

    LOG_INFO("Number of correction steps: " <<
             numberOfSteps << " in the " <<
             translationDirection << " direction");
    if (getWriteLog())
      writeToStepLog(translationDirection, numberOfSteps);
    return true;

  } else {
    return false;
  }
}


bool LineTraceCalculator::correctTranslation(const Trace* i_trace,
                                             Point2D& i_point2D) const {
  /* The distance to the enpoint after the correction is applied*/

  Point2D intersectingPoint;

  try {
    intersectingPoint = i_trace->intersectingPoint(i_point2D);
  }
  catch(std::runtime_error) {
    LOG_DEBUG("Found no intersection reverting the old position!");
    return false;
  }

  if (intersectingPoint == Point2D(0, 0))
    return false;

  double currentRotation = i_point2D.getAlpha();
  double intersectingAngle = intersectingPoint.getAlpha();
  double endPointAngle = i_trace->getEndPoint().getAlpha();

  LOG_INFO("Distination point: " << intersectingPoint.x <<
           ", " << intersectingPoint.y);
  LOG_INFO("Current robot rotation: " << currentRotation*180.0/PI);
  LOG_INFO("DistinationPoint rotation: " << intersectingAngle*180.0/PI);

  /* The distance to the enpoint after the correction is applied*/
  double distenceEndPointIntersectingPoint =
      std::abs(endPointAngle-intersectingAngle);

  // The distance traveled in this correction
  double distanceBeginPointIntersectingPoint =
      std::abs(currentRotation-intersectingAngle);

  // double distinationPointRotation=intersectingPoint.getAlpha();
  // double jointAngleDifference=currentRotation-distinationPointRotation;

  LOG_INFO("Joint angle difference: " <<
           distanceBeginPointIntersectingPoint*(PI/180));

  // Anglular distance for the correction
  double jointPointDifference;

  // Position the joint is aiming for
  Point2D destinationPoint;

  if (distenceEndPointIntersectingPoint < distanceBeginPointIntersectingPoint) {
    /*if the distance needed in the next step is smaller then the distance we travel in this correction step
      Don't overshoot
    */
    jointPointDifference = distanceBeginPointIntersectingPoint;
    destinationPoint = i_trace->getEndPoint();
  } else {
    /*
      Otherwise we assume that there is enough distance, 
      so overshooting the correction decrease the absolute distance between the wanted and final result
      This is done by spreading out the error over both the positive as negative side of the line
    */
    jointPointDifference = distanceBeginPointIntersectingPoint;
    destinationPoint = intersectingPoint;
    jointPointDifference*=2.0;
  }

  int numberOfSteps = std::floor(jointPointDifference/
                               getJointController()->getJoint(Rotational)->
                               getMovementPerStep()*(PI/180.0));

  if (numberOfSteps>0) {
    std::string rotationDirection = i_trace->
        getRotationDirection(i_point2D,
                             destinationPoint);

    getJointController()->getJoint(Rotational)->predictSteps(i_point2D,
                                                             rotationDirection,
                                                             numberOfSteps);

    getJointController()->moveSteps(getJointController()->getJoint(Rotational),
                                    rotationDirection,
                                    numberOfSteps);

    if (getWriteLog())
      writeToStepLog(rotationDirection, numberOfSteps);

    return true;
  }
  return false;
}

bool LineTraceCalculator::hasStepped() {
  return (m_hasTranslated or m_hasRotated);
}

