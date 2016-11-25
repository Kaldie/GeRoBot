// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "Robot.h"
#include <SpeedController.h>
#include <ConstantSpeedController.h>
#include <BaseTraceCalculator.h>
#include <BaseMotor.h>
#include <PinState.h>
#include <Trace.h>
#include <RotationTraceCalculator.h>
#include <RotationTrace.h>
#include <Point2D.h>

int stepsMissed = 0;

Robot::Robot()
  : Robot( nullptr,  Point2D(0, 50))
{}


Robot::Robot(const JointController::JointControllerPointer& i_pointer)
  : Robot(i_pointer, Point2D(0, 50))
{}


Robot::Robot(const JointController::JointControllerPointer& i_pointer,
             const Point2D& i_currentPosition)
  : m_jointController(i_pointer),
    m_position(i_currentPosition),
    m_speedController(std::make_shared<ConstantSpeedController>()) {
}


bool Robot::hasValidConnection() {
  if (m_jointController) {
    return m_jointController->getActuatorPointer()->hasConnection(true);
  }
  return false;
}


traceType Robot::getMovementPerStep(const BaseJoint::MovementType& i_movementType) const {
  return m_jointController->resolveJoint(i_movementType)->getMovementPerStep();
}


void Robot::goToPosition(const Point2D &i_position) {
  if (!m_jointController)
    LOG_ERROR("JointController is not set yet!");
  LOG_DEBUG("current position: " << m_position.x << m_position.y);
  BaseTraceCalculator baseTraceCalculator(this);
  Trace thisTrace(m_position, i_position);
  baseTraceCalculator.calculateTrace(&thisTrace);
  LOG_DEBUG("new position: " << getVirtualPosition().x << getVirtualPosition().y);
  actuate();
}


void Robot::setPosition(const Point2D& i_position) {
  if (m_jointController->getNumberOfJoints() != 2) {
    LOG_ERROR("Cannot set the position if the number of joints is more then 2");
  }
  m_jointController->resolveJoint(BaseJoint::MovementType::Translational)
    ->setPosition(magnitude(i_position));
  m_jointController->resolveJoint(BaseJoint::MovementType::Rotational)
    ->setPosition(i_position.getAngleToOrigin());
  m_position = i_position;
}


const Point2D Robot::getVirtualPosition() const {
  return m_jointController->getRootJoint()->childPosition();
}



void Robot::traceCalculation(const Trace::TracePointer& i_trace) {
  std::unique_ptr<BaseTraceCalculator> traceCalculator;
  if (i_trace->getTraceType() == Trace::TraceType::Curve) {
    traceCalculator = std::unique_ptr<BaseTraceCalculator>(new RotationTraceCalculator(this));
  } else if (i_trace->getTraceType() == Trace::TraceType::Line) {
    traceCalculator = std::unique_ptr<BaseTraceCalculator>(new LineTraceCalculator(this));
  } else {
    LOG_ERROR("Could not resolve trace type!");
  }
  m_speedController->prepareSpeedController(*i_trace, *m_jointController);
  traceCalculator->calculateTrace(i_trace.get());
}


void Robot::prepareSteps(const std::string& i_direction,
                         const int& i_numberOfSteps) {
  // predict the next step
  BaseJoint::JointPointer joint = m_jointController->resolveJoint(i_direction);
#ifdef DEBUG
  m_traveledPoints.push_back(getVirtualPosition());
#endif
  // add the step to the sequence
  m_jointController->moveSteps(i_direction, i_numberOfSteps);
  m_speedController->notifyStep(joint, i_numberOfSteps);
  int motorSpeed;
  if (m_speedController->adviseSpeed(&motorSpeed)) {
    LOG_DEBUG("Speed controler has a mandatory speed change.");
    // add a clean sequence to force the speed to be nice
    m_jointController->getSequenceVectorPointer()->addEmptySequence();
    m_speedController->acknowledgeSpeed(motorSpeed);
  }
}


void Robot::actuate() {
  LOG_DEBUG("Steps missed: " << stepsMissed);
  LOG_DEBUG("Regular expression: " << m_jointController->getActuator().getSerialRegularExpresion());
  // upload the current sequence
  m_jointController->uploadSequence(false);
  // Send the command to actuate the sequence
  m_jointController->actuate();
  // reset the traveledPoints vector
  std::vector<Point2D> empty;
  m_traveledPoints.swap(empty);
  m_position = getVirtualPosition();
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
