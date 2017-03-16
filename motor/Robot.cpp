// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "Robot.h"
#include <JointController.h>
#include <SpeedController.h>
#include <ConstantSpeedController.h>
#include <BaseTraceCalculator.h>
#include <BaseMotor.h>
#include <PinState.h>
#include <Trace.h>
#include <RotationTraceCalculator.h>
#include <RotationTrace.h>
#include <MovementRegistrator.h>

Robot::Robot()
  : Robot(std::make_shared<JointController>(),
	  Point2D(0, 50))
{}


Robot::Robot(const JointController::JointControllerPointer& i_pointer)
  : Robot(i_pointer, Point2D(0, 50))
{}


Robot::Robot(const JointController::JointControllerPointer& i_pointer,
             const Point2D& i_currentPosition)
  : m_jointController(i_pointer),
    m_position(i_currentPosition),
    m_speedController(std::make_shared<ConstantSpeedController>()) {
  setSpeedController(m_speedController);
}


void Robot::setSpeedController(const std::shared_ptr<SpeedController>& i_speedController) {
  LOG_DEBUG("setSpeedController");
  /// unload the old movement registrator
  deregisterMovementRegistrator(m_speedController->getMovementRegistrator());
  /// set the class member
  m_speedController = i_speedController;
  /// load the new movement registrator
  registerMovementRegistrator(i_speedController->getMovementRegistrator());
}


bool Robot::hasValidConnection() {
  if (!m_jointController) {
    return false;
  }
  try {
    return m_jointController->getActuatorPointer()->hasConnection(true);
  }catch (std::runtime_error error) {
    if (strstr(error.what(), "Could not find any devices using: ") != nullptr) {
      LOG_DEBUG("Strstr said: " << strstr(error.what(), "Could not find any devices using: "));
      return false;
    } else {
      LOG_DEBUG("Going in the re-trow thingy");
      LOG_DEBUG("strstr said: " << strstr(error.what(), "Could not find any devices using: "));
      throw;
    }
  }
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
  baseTraceCalculator.calculateTrace(thisTrace);
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


void Robot::prepareSteps(const std::string& i_direction,
                         const int& i_numberOfSteps) {
  BaseJoint::JointPointer joint = m_jointController->resolveJoint(i_direction);
  // notify all the motion registrators that steps have been taken
  notifySteps(joint, i_numberOfSteps);

  // add the step to the sequence
  m_jointController->moveSteps(i_direction, i_numberOfSteps);
  int motorSpeed;
  if (m_speedController->adviseSpeed(&motorSpeed)) {
    LOG_DEBUG("Speed controler has a mandatory speed change.");
    // add a clean sequence to force the speed to be nice
    m_jointController->getSequenceVectorPointer()->addEmptySequence();
  }
  m_speedController->acknowledgeSpeed
    (motorSpeed,m_jointController->getSequenceVectorPointer());
}


void Robot::actuate() {
  LOG_DEBUG("Regular expression: " << 
	    m_jointController->getActuator().getSerialRegularExpresion());
  notifyActuation();
  // upload the current sequence
  m_jointController->uploadSequence(false);
  // Send the command to actuate the sequence
  m_jointController->actuate();
  // Update the robot position to the position previously calculated
  m_position = getVirtualPosition();
}


void Robot::restartTraceCalculation() {
  // reset the calculated pins
  m_jointController->resetPinStateSequence();
  // notify the other components that a pseudo actuation has taken place
  notifyActuation();
  // update the joint positions
  setPosition(m_position);

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


bool Robot::registerMovementRegistrator(std::shared_ptr<MovementRegistrator> i_registrator) {
  LOG_DEBUG("RegisterMovementRegistrator");
  if (m_movementRegistrators.find(i_registrator) != m_movementRegistrators.end()) {
    LOG_DEBUG("Registrator is allready known");
    return false;
  }
  m_movementRegistrators.insert(i_registrator);
  return true;
}


bool Robot::deregisterMovementRegistrator(std::shared_ptr<MovementRegistrator> i_registrator) {
  LOG_DEBUG("DeregisterMovementRegistrator");
  std::weak_ptr<MovementRegistrator> newRegistrator(i_registrator);
  if (m_movementRegistrators.find(newRegistrator) == m_movementRegistrators.end()) {
    LOG_DEBUG("Registrator is not known");
    return false;
  }
  m_movementRegistrators.erase(newRegistrator);
  return true;
}


void Robot::notifySteps(std::shared_ptr<BaseJoint> i_joint,
					    const int& i_numberOfSteps) const {
  for (const auto& registor: m_movementRegistrators) {
    if (auto shared_registrator = registor.lock()) {
      shared_registrator->notifySteps(i_joint, i_numberOfSteps);
    }
  }
}


void Robot::notifyActuation() const {
  for (const auto& registor: m_movementRegistrators) {
    if (auto shared_registrator = registor.lock()) {
      shared_registrator->notifyActuation();
    }
  }
  m_speedController->setCurrentSequenceVectorPosition(0);
}
