/*
  Robot is the basis of the project.
  Robot is the root of a object which looks like: Robot->Joint->Actuator

  Robot will also be calculating the traces
  To calculating traces we need info on the joints
  These traces will be handled by the actuators of the joints    
*/

#include <macroHeader.h>
#include "Robot.h"
#include <BaseTraceCalculator.h>
#include <PinState.h>
#include <Trace.h>

Robot::Robot()
    : m_speed(0),
      m_currentPosition {0, 50}
{}

void Robot::goToPosition(const Point2D &i_position) {
  Trace thisTrace(m_currentPosition, i_position);
  LOG_DEBUG("current position: " << m_currentPosition.x << m_currentPosition.y);
  BaseTraceCalculator baseTraceCalculator(&m_jointController);
  baseTraceCalculator.calculateTrace(&thisTrace,
                                     m_currentPosition);
  
  LOG_DEBUG("new position: " << m_currentPosition.x << m_currentPosition.y);
  getJointController().actuate();
}

bool Robot::hasValidConnection() {
  return m_jointController.getActuatorPointer()->hasConnection();
}
