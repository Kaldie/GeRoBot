// Ruud Cools [2016]
#include <macroHeader.h>
#include <RelativeSpeedController.h>
#include <BaseJoint.h>
#include <BaseMotor.h>


RelativeSpeedController::RelativeSpeedController()
  :RelativeSpeedController(0, 0) {
}


RelativeSpeedController::RelativeSpeedController(const float& speed )
  : RelativeSpeedController(speed, 0) {
}


/// Constructor with a speed defined
RelativeSpeedController::RelativeSpeedController(const float& speed, const int& i_vectorPosition )
  : SpeedController(SpeedController::Type::Relative, speed, i_vectorPosition),
    m_minSpeed(0),
    m_maxSpeed(std::numeric_limits<int>::max()) {
}


bool RelativeSpeedController::adviseSpeed(int* o_speed) const {
  int robotSpeed = m_robotSpeed;
  if (robotSpeed < 1) {
    robotSpeed = 1;
  } else if (robotSpeed > 100) {
    robotSpeed = 100;
  }
  *o_speed = (m_maxSpeed - m_minSpeed) * (robotSpeed / 100.0) + m_minSpeed;
  return false;
}


bool RelativeSpeedController::adviseSpeed(int* o_speed, const int& i_minSteed, const int& i_maxSpeed) const {
  int robotSpeed = m_robotSpeed;
  if (robotSpeed < 1) {
    robotSpeed = 1;
  } else if (robotSpeed > 100) {
    robotSpeed = 100;
  }
  *o_speed = (i_maxSpeed - i_minSteed) * (robotSpeed / 100.0) + i_minSteed;
  return false;
}


void RelativeSpeedController::acknowledgeSpeed(const unsigned int& i_speed,
					       SequenceVector* i_sequenceVector) {
  SpeedController::acknowledgeSpeed(i_speed, i_sequenceVector);
  m_minSpeed = 0;
  m_maxSpeed = std::numeric_limits<int>::max();
}


void RelativeSpeedController::notifyStep(const BaseJoint::JointPointer& i_joint,
					 const unsigned int& i_numberOfSteps) {
  determineMotorSpeed(i_joint, &m_minSpeed, &m_maxSpeed);
}


void RelativeSpeedController::prepareSpeedController(const BaseJoint::JointPointer& i_joint) {
  notifyStep(i_joint, 0);
}


traceType  RelativeSpeedController::determineRobotSpeed
(const BaseJoint::JointPointer& i_joint) const {
  int speed, minSpeed(0), maxSpeed(std::numeric_limits<int>::max());
  determineMotorSpeed(i_joint, &minSpeed, &maxSpeed);
  adviseSpeed(&speed, minSpeed, maxSpeed);
  return estimateRobotSpeed(i_joint, speed);  
}


void RelativeSpeedController::determineMotorSpeed(const BaseJoint::JointPointer& i_joint,
						  int* io_minSpeed,
						  int* io_maxSpeed) const {
  BaseMotor::MotorPointer motor = i_joint->getMotor();
  if (*io_minSpeed < motor->getMinimumSpeed()) {
    *io_minSpeed = motor->getMinimumSpeed();
  }
  if (*io_maxSpeed > motor->getMaximumSpeed()) {
    *io_maxSpeed = motor->getMaximumSpeed();
  }
}


traceType RelativeSpeedController::estimateRobotSpeed(const BaseJoint::JointPointer& i_joint,
						      const int& i_motorSpeed) const {
  traceType movementPerStep;
  if (i_joint->getMovementType() == BaseJoint::MovementType::Translational) {
    movementPerStep = i_joint->getMovementPerStep();
  } else if (i_joint->getMovementType() == BaseJoint::MovementType::Rotational) {
    movementPerStep = i_joint->getMovementPerStep() * 180/ PI;
    LOG_DEBUG("Rotational speed is: " << movementPerStep);
    LOG_DEBUG("Motor speed is: " << i_motorSpeed);
  }
   return movementPerStep * i_motorSpeed * 10.0;    
}


/*
Not included in this class, it is still here for future reference
void RelativeSpeedController::calculateMinMaxSpeed(const JointController& i_controller,
						   const Trace& i_trace) {
  m_minSpeed = 0;
  m_maxSpeed = std::numeric_limits<int>::max();
  BaseJoint::JointPointer joint;
  BaseMotor* motor;
  SpeedController::JointStepVector jointStepVector = estimateSteps(i_trace, i_controller);
  for (const auto jointStep : jointStepVector) {
    if (std::get<1>(jointStep) == 0) {
      continue;
    }
    motor = std::get<0>(jointStep)->getMotor();
    if (m_maxSpeed > motor->getMaximumSpeed()) {
      m_maxSpeed = motor->getMaximumSpeed();
    }
    if (m_minSpeed < motor->getMinimumSpeed()) {
      m_minSpeed = motor->getMinimumSpeed();
    }
  }
}
*/



