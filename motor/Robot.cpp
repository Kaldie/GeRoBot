// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "Robot.h"
#include <BaseTraceCalculator.h>
#include <PinState.h>
#include <Trace.h>

Robot::Robot()
  : Robot( nullptr, 30, Point2D(0, 50))
{}


Robot::Robot(const JointController::JointControllerPointer& i_pointer)
  : Robot(i_pointer, 30, Point2D(0, 50))
{}


Robot::Robot(const JointController::JointControllerPointer& i_pointer,
             const int& i_speed)
  : Robot(i_pointer, i_speed, Point2D(0, 50))
{}


Robot::Robot(const JointController::JointControllerPointer& i_pointer,
             const int& i_speed,
             const Point2D& i_currentPosition)
  : m_jointController(i_pointer),
    m_speed(i_speed),
    m_position(i_currentPosition),
    m_virtualPosition(i_currentPosition) {
}


bool Robot::hasValidConnection() {
  if (m_jointController)
    return m_jointController->getActuatorPointer()->
      getArduinoConnection().hasConnection();
  return false;
}


traceType Robot::getMovementPerStep(const MovementType& i_movementType) const {
  return m_jointController->resolveJoint(i_movementType)->getMovementPerStep();
}


void Robot::goToPosition(const Point2D &i_position) {
  if (!m_jointController)
    LOG_ERROR("JointController is not set yet!");
  LOG_DEBUG("current position: " << m_position.x << m_position.y);
  Trace thisTrace(m_position, i_position);
  BaseTraceCalculator baseTraceCalculator(this);
  baseTraceCalculator.calculateTrace(thisTrace);
  LOG_DEBUG("new position: " << m_position.x << m_position.y);
  m_jointController->actuate();
}


void Robot::prepareSteps(const std::string& i_direction,
                         const int& i_numberOfSteps) {
  // predict the next step
  m_jointController->resolveJoint(i_direction)->
    predictSteps(&m_virtualPosition, i_direction, i_numberOfSteps);
  // add the point to the traveled points
  m_traveledPoints.push_back(m_virtualPosition);
  // add the step to the sequence
  m_jointController->moveSteps(i_direction, i_numberOfSteps);
}
