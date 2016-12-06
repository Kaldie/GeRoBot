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
  *o_speed = (m_maxSpeed - m_minSpeed) / (robotSpeed / 100.0) + m_minSpeed;
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
  BaseMotor* motor = i_joint->getMotor();
  if (m_minSpeed < motor->getMinimumSpeed()) {
    m_minSpeed = motor->getMinimumSpeed();
  }
  if (m_maxSpeed > motor->getMaximumSpeed()) {
    m_maxSpeed = motor->getMaximumSpeed();
  }
}


void RelativeSpeedController::prepareSpeedController(const BaseJoint::JointPointer& i_joint) {
  notifyStep(i_joint, 0);
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
