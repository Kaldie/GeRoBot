// Ruud Cools [2016]

#include <macroHeader.h>
#include <ConstantSpeedController.h>
#include <JointController.h>
#include <BaseJoint.h>
#include <StepperDriver.h>
#include <BaseMotor.h>
#include <Trace.h>
#include <MovementRegistrator.h>

ConstantSpeedController::ConstantSpeedController()
  : ConstantSpeedController(0,0) {
}


ConstantSpeedController::ConstantSpeedController(const float& i_robotSpeed)
  : ConstantSpeedController(i_robotSpeed, 0) {
}


ConstantSpeedController::ConstantSpeedController(const float& i_robotSpeed,
						 const int& i_vectorPosition)
  : SpeedController(SpeedController::Type::Constant,
		    i_robotSpeed,
		    i_vectorPosition) {
}


/// This method will be called at the moment the robot needs to decied on some speed
bool ConstantSpeedController::adviseSpeed(int* o_speed) const {
  *o_speed = determineJointSpeed();
  return false;
}


int ConstantSpeedController::determineJointSpeed() const {
  BaseJoint::JointPointer joint;
  int currentMax = 0;
  for (const auto jointAndSteps : m_movementRegistrator->getLocalMap()) {
    if (joint = jointAndSteps.first.lock()) {
      if (currentMax < jointAndSteps.second) {
	joint = jointAndSteps.first.lock();
	currentMax = jointAndSteps.second;
      }
    }
  }
  if (joint->getMovementType() == BaseJoint::Rotational) { 
    LOG_DEBUG("Number of rotation steps: " << currentMax);
  } else if (joint->getMovementType() == BaseJoint::Translational) {
    LOG_DEBUG("Number of translation steps: " << currentMax);
  }
  return determineJointSpeed(joint);
}


int ConstantSpeedController::determineJointSpeed(const BaseJoint::JointPointer& i_joint) const {
  float movementPerStep;
  if (i_joint->getMovementType() == BaseJoint::Rotational) {
    traceType childLength = magnitude(i_joint->childPosition());
    movementPerStep = childLength * i_joint->getMovementPerStep();
  } else {
    movementPerStep = i_joint->getMovementPerStep();
  }
  return determineJointSpeed(i_joint, movementPerStep);
}


int ConstantSpeedController::determineJointSpeed(const BaseJoint::JointPointer& i_joint,
						 const traceType& i_movementPerStep) const {
  int driverSpeed;
  int requiredFrequency = m_robotSpeed / i_movementPerStep;
  int pullInFrequency = i_joint->getMotor()->getMaximumSpeed();
  if (requiredFrequency <= pullInFrequency) {
    driverSpeed = requiredFrequency;
  } else {
    driverSpeed = pullInFrequency;
  }
  LOG_DEBUG("Joint speed will be: " << driverSpeed);
  return driverSpeed;
}
