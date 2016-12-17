// Ruud Cools [2016]

#include <macroHeader.h>
#include <ConstantSpeedController.h>
#include <JointController.h>
#include <BaseJoint.h>
#include <StepperDriver.h>
#include <BaseMotor.h>
#include <Trace.h>

ConstantSpeedController::ConstantSpeedController()
  : ConstantSpeedController(0,0) {
}


ConstantSpeedController::ConstantSpeedController(const float& i_robotSpeed)
  : ConstantSpeedController(i_robotSpeed, 0) {
}


ConstantSpeedController::ConstantSpeedController(const float& i_robotSpeed,
						 const int& i_vectorPosition)
  : SpeedController(SpeedController::Type::Constant, i_robotSpeed, i_vectorPosition),
    m_frequency(0) {
}


/// This method will be called at the moment the robot needs to decied on some speed
bool ConstantSpeedController::adviseSpeed(int* o_speed) const {
  *o_speed = m_frequency;
  return false;
}


/**
 * This is call just before a algorithm determins movement of the robot
 */
void ConstantSpeedController::prepareSpeedController(
  const Trace& i_trace,  const JointController& i_controller) {
  SpeedController::JointStepVector jointStepVector = estimateSteps(i_trace, i_controller);
  BaseJoint::JointPointer joint;
  int currentMax = 0;
  for (const auto jointAndSteps : jointStepVector) {
    if (currentMax < std::get<1>(jointAndSteps)) {
      joint = std::get<0>(jointAndSteps);
      currentMax = std::get<1>(jointAndSteps);
    }
  }
  if (joint->getMovementType() == BaseJoint::Rotational) { 
    LOG_DEBUG("Number of rotation steps: " << currentMax);
  } else if (joint->getMovementType() == BaseJoint::Translational) {
    LOG_DEBUG("Number of translation steps: " << currentMax);
  }

  traceType movementPerStep;
  if (joint->getMovementType() == BaseJoint::Rotational) {
     float averageMagnitude =
       (magnitude(i_trace.getEndPoint()) + magnitude(i_trace.getStartPoint())) / 2;
     movementPerStep = averageMagnitude * joint->getMovementPerStep();
  } else {
    movementPerStep = joint->getMovementPerStep();
  }
  updateJointSpeed(joint, movementPerStep);
}


void ConstantSpeedController::prepareSpeedController(const BaseJoint::JointPointer& i_joint) {
  float movementPerStep;
  if (i_joint->getMovementType() == BaseJoint::Rotational) {
    traceType childLength = magnitude(i_joint->childPosition());
    movementPerStep = childLength * i_joint->getMovementPerStep();
  } else {
    movementPerStep = i_joint->getMovementPerStep();
  }
  updateJointSpeed(i_joint, movementPerStep);
}


void ConstantSpeedController::updateJointSpeed(const BaseJoint::JointPointer& i_joint,
					       const traceType& i_movementPerStep) {
 int requiredFrequency = m_robotSpeed / i_movementPerStep;
  int pullInFrequency = i_joint->getMotor()->getMaximumSpeed();
  if (requiredFrequency <= pullInFrequency) {
    m_frequency = requiredFrequency;
  } else {
    m_frequency = pullInFrequency;
  }
  LOG_DEBUG("Joint speed will be: " << m_frequency);
}


