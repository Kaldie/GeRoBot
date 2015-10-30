// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "Robot.h"
#include <BaseTraceCalculator.h>
#include <BaseMotor.h>
#include <PinState.h>
#include <Trace.h>

int stepsMissed = 0;

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
  if (m_jointController) {
    if (m_jointController->getActuatorPointer()->
        getArduinoConnection().hasConnection()) {
      return true;
    }
  }
  return true;
}


traceType Robot::getMovementPerStep(const MovementType& i_movementType) const {
  return m_jointController->resolveJoint(i_movementType)->getMovementPerStep();
}


void Robot::goToPosition(const Point2D &i_position) {
  if (!m_jointController)
    LOG_ERROR("JointController is not set yet!");
  LOG_DEBUG("current position: " << m_position.x << m_position.y);
  BaseTraceCalculator baseTraceCalculator(this);
  Trace thisTrace(m_position, i_position);
  baseTraceCalculator.calculateTrace(thisTrace);
  LOG_DEBUG("new position: " << m_position.x << m_position.y);
  actuate();
}


void Robot::prepareSteps(const std::string& i_direction,
                         const int& i_numberOfSteps) {
  // predict the next step
  //  int preNumberOfSteps = m_jointController->getSequenceVector().numberOfSteps();
  m_jointController->resolveJoint(i_direction)->
    predictSteps(&m_virtualPosition, i_direction, i_numberOfSteps);
  // add the point to the traveled points
  m_traveledPoints.push_back(m_virtualPosition);
  // add the step to the sequence
  m_jointController->moveSteps(i_direction, i_numberOfSteps);
  //if (preNumberOfSteps >
  //  m_jointController->getSequenceVector().numberOfSteps() - (2 * i_numberOfSteps)) {
  //LOG_DEBUG("Step is wanted, however not set!" << std::endl <<
  //          "Current steps: " << preNumberOfSteps << ", new steps: " <<
  //          m_jointController->getSequenceVector().numberOfSteps());
  //++stepsMissed;
  //LOG_DEBUG("Current steps missed: " << stepsMissed);
}


void Robot::actuate() {
  LOG_DEBUG("Steps missed: " << stepsMissed);
  // upload the current sequence
  m_jointController->uploadSequence(false);
  // Send the command to actuate the sequence
  m_jointController->actuate();
  // reset the traveledPoints vector
  std::vector<Point2D> empty;
  m_traveledPoints.swap(empty);
  // After the actuation, the virtual position is the actual position
  m_position = m_virtualPosition;
}


void Robot::setIdle(const bool& i_setIdle) {
  // update the actuators of the joints
  // so that the hold motor is set the proper value
  StateSequence newState;
  for (auto& joint : m_jointController->getJointPointerVector()) {
    joint->getMotor()->setHoldMotor(i_setIdle);
    newState.addToSequence(joint->getMotor()->getCurrentPinState());
  }
  SequenceVector newVector;
  newVector.appendStateSequence(newState, false);
  // Replace the StateSequenceVector
  m_jointController->setSequenceVector(newVector);
  // upload and actuate
  actuate();
}


void Robot::addToSequence(const StateSequence& i_sequence) {
  SequenceVector* sequenceVector = m_jointController->getSequenceVectorPointer();
  if (!sequenceVector->addToSequence(i_sequence))
    sequenceVector->appendStateSequence(i_sequence, false);
}
