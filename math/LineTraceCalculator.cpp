// Copyright [2014] Ruud Cools

#include <macroHeader.h>
#include <Point2D.h>
#include <PinState.h>
#include <BaseJoint.h>
#include <JointController.h>
#include <LineTraceCalculator.h>
#include <Trace.h>


LineTraceCalculator::LineTraceCalculator()
    : BaseTraceCalculator()
{}


LineTraceCalculator::LineTraceCalculator
(Robot* i_robotPointer)
    : BaseTraceCalculator(i_robotPointer)
{}


LineTraceCalculator::LineTraceCalculator
(Robot* i_robotPointer,
 const traceType& i_tolerance)
    : BaseTraceCalculator(i_robotPointer,
                          i_tolerance)
{}


void LineTraceCalculator::calculateTrace(const Trace& i_trace) {
  // Setting the capacity of the vector to hold all the data
  int i = 0;
  bool hasStepped(true);
  do {
    hasStepped = calculateStep(i_trace);
    i++;
  }while(hasStepped);  // and i<1000);
}


bool LineTraceCalculator::calculateStep(const Trace& i_trace) const {
  // keeping track if the robot is orded to translate or rotated
  bool hasStepped = false;
  // should rotate?
  if (shouldRotate(i_trace,
                   m_robot->getVirtualPosition())) {
    // add rotation to the delay list and correct translations
    prepareRotation(i_trace);
    hasStepped = true;
  } else if (shouldTranslate(i_trace,
                             m_robot->getVirtualPosition())) {
    prepareTranslation(i_trace);
    hasStepped = true;
  }
  return hasStepped;
}


void LineTraceCalculator::prepareRotation(const Trace& i_trace)const {
  // Rotation direction
  std::string direction =
    i_trace.getRotationDirectionToEndPoint(m_robot->getVirtualPosition());
  // user needs to know :)
  LOG_INFO("Rotating: " << direction);
  // Correct the rotation
  correctRotation(i_trace);
  // Prepare a step
  m_robot->prepareSteps(direction, 1);
  if (getWriteLog())
    writeToStepLog(direction, 1, m_robot->getVirtualPosition());


}

void LineTraceCalculator::prepareTranslation(const Trace& i_trace) const {
  // identify the direction to move to
  std::string direction =
    i_trace.getTranslationDirectionToEndPoint(m_robot->getVirtualPosition());
  // Correct the rotation, if nessesary!
  correctTranslation(i_trace);
  // Predict the step
  LOG_INFO("Translating: " << direction);
  m_robot->prepareSteps(direction, 1);
  if (getWriteLog())
    writeToStepLog(direction, 1, m_robot->getVirtualPosition());
}


bool LineTraceCalculator::correctRotation(const Trace& i_trace) const {
  Point2D intersectingPoint;
  Point2D currentRobotPosition = m_robot->getVirtualPosition();
  try {
    intersectingPoint =
      i_trace.intersectingPoint(currentRobotPosition);
  }
  catch(std::runtime_error) {
    LOG_DEBUG("Found no intersection reverting the old position!");
    return false;
  }
  LOG_DEBUG("Current position: " << currentRobotPosition.x
            << " , "<< currentRobotPosition.y);
  LOG_DEBUG("Intersecting point: " << intersectingPoint.x << " , "<< intersectingPoint.y);
  // The distance to the enpoint after the correction is applied*/
  traceType distenceEndPointIntersectingPoint =
      Magnitude(intersectingPoint - i_trace.getEndPoint());
  // The distance traveled in this correction
  traceType distanceBeginPointIntersectingPoint =
      Magnitude(intersectingPoint - currentRobotPosition);
  // calculate the correction distance and destination after correction
  traceType jointPointDifference;
  const Point2D* destinationPoint;
  if (distenceEndPointIntersectingPoint < distanceBeginPointIntersectingPoint) {
    /*if the distance needed in the next step is smaller then the distance we travel in this correction step
      Don't overshoot
    */
    jointPointDifference = Magnitude(i_trace.getEndPoint()-currentRobotPosition);
    destinationPoint=&i_trace.getEndPoint();
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
  // calculate the number of steps needed to correct
  int numberOfSteps = std::floor((jointPointDifference) / m_robot->
                                 getMovementPerStep(Translational));

  if (numberOfSteps>0) {
    // calculate the direction and prepare them
    std::string translationDirection=
        i_trace.getTranslationDirection(currentRobotPosition, *destinationPoint);
    LOG_INFO("Number of correction steps: " << numberOfSteps << " in the " <<
             translationDirection << " direction");
    m_robot->prepareSteps(translationDirection, numberOfSteps);
    if (getWriteLog())
      writeToStepLog(translationDirection, numberOfSteps, m_robot->getVirtualPosition());
    return true;
  } else {
    return false;
  }
}

bool LineTraceCalculator::correctTranslation(const Trace& i_trace) const {
  /* The distance to the enpoint after the correction is applied*/
  Point2D intersectingPoint;
  Point2D currentRobotPosition = m_robot->getVirtualPosition();
  try {
    intersectingPoint = i_trace.intersectingPoint(currentRobotPosition);
  }
  catch(std::runtime_error) {
    LOG_DEBUG("Found no intersection reverting the old position!");
    return false;
  }

  if (intersectingPoint == Point2D(0, 0))
    return false;

  traceType currentRotation = currentRobotPosition.getAlpha();
  traceType intersectingAngle = intersectingPoint.getAlpha();
  traceType endPointAngle = i_trace.getEndPoint().getAlpha();

  LOG_INFO("Distination point: " << intersectingPoint.x <<
           ", " << intersectingPoint.y);
  LOG_INFO("Current robot rotation: " << currentRotation*180.0/PI);
  LOG_INFO("DistinationPoint rotation: " << intersectingAngle*180.0/PI);

  /* The distance to the enpoint after the correction is applied*/
  traceType distenceEndPointIntersectingPoint =
      std::abs(endPointAngle-intersectingAngle);
  // The distance traveled in this correction
  traceType distanceBeginPointIntersectingPoint =
      std::abs(currentRotation-intersectingAngle);
  LOG_INFO("Joint angle difference: " <<
           distanceBeginPointIntersectingPoint*(PI/180));
  // Anglular distance for the correction
  traceType jointPointDifference;
  // Position the joint is aiming for
  Point2D destinationPoint;
  if (distenceEndPointIntersectingPoint < distanceBeginPointIntersectingPoint) {
    /*if the distance needed in the next step is smaller then the distance we travel in this correction step
      Don't overshoot
    */
    jointPointDifference = distanceBeginPointIntersectingPoint;
    destinationPoint = i_trace.getEndPoint();
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
  // calculate the number of steps needed to correct
  int numberOfSteps = std::floor(jointPointDifference/
                                 m_robot->getMovementPerStep(Rotational));

  if (numberOfSteps>0) {
    // direction of correction
    std::string rotationDirection =
      i_trace. getRotationDirection(currentRobotPosition, destinationPoint);
    m_robot->prepareSteps(rotationDirection, numberOfSteps);
    if (getWriteLog())
      writeToStepLog(rotationDirection, numberOfSteps, m_robot->getVirtualPosition());
    return true;
  }
  return false;
}
