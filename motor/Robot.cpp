// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "Robot.h"
#include <BaseTraceCalculator.h>
#include <PinState.h>
#include <Trace.h>

Robot::Robot()
  : Robot( nullptr, 0, Point2D(0, 50))
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
    m_currentPosition(i_currentPosition) {
}


void Robot::goToPosition(const Point2D &i_position) {
  if (!m_jointController)
    LOG_ERROR("JointController is not set yet!");
  LOG_DEBUG("current position: " << m_currentPosition.x << m_currentPosition.y);
  Trace thisTrace(m_currentPosition, i_position);
  BaseTraceCalculator baseTraceCalculator(m_jointController);
  baseTraceCalculator.calculateTrace(&thisTrace,
                                     m_currentPosition);

  LOG_DEBUG("new position: " << m_currentPosition.x << m_currentPosition.y);
  m_jointController->actuate();
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


void Robot::predictSteps(const std::string& i_direction,
                         const int& i_numberOfSteps,
                         Point2D* i_position) const {
  m_jointController->resolveJoint(i_direction)->
    predictSteps(i_position,
                 i_direction,
                 i_numberOfSteps);
}
