// Ruud Cools [2016]

#include <macroHeader.h>
#include <ConstantSpeedController.h>
#include <JointController.h>
#include <StepperDriver.h>
#include <BaseMotor.h>
#include <Trace.h>

ConstantSpeedController::ConstantSpeedController()
  : ConstantSpeedController(0,0) {
}


ConstantSpeedController::ConstantSpeedController(float i_speed)
  : ConstantSpeedController(i_speed, 0) {
}

ConstantSpeedController::ConstantSpeedController(float i_robotSpeed, float i_motorSpeed)
  : SpeedController("ConstantSpeedController", i_robotSpeed),
    m_jointSpeed(i_motorSpeed) {
}

/// This method will be called at the moment the robot needs to decied on some speed
bool ConstantSpeedController::adviseSpeed(int* o_speed) {
  *o_speed = m_jointSpeed;
  return false;
}

/**
 * This is call just before a algorithm determins movement of the robot
 */
void ConstantSpeedController::prepareSpeedController(
  const Trace& i_trace,
  const JointController& i_controller) {

  int estimateNumberOfRotationSteps = abs(
    i_trace.getStartPoint().getAngleBetweenPoints(i_trace.getEndPoint()) /
    (i_controller.resolveJoint(BaseJoint::Rotational))->getMovementPerStep());
  int estimateNumberOfTranslationSteps = abs(
    magnitude(i_trace.getEndPoint() - i_trace.getStartPoint()) /
    i_controller.resolveJoint(BaseJoint::Translational)->getMovementPerStep());
  LOG_DEBUG("Number of rotation steps: " << estimateNumberOfRotationSteps);
  LOG_DEBUG("Number of translation steps: " << estimateNumberOfTranslationSteps);
  // get the joint who sees the most work
  BaseJoint::JointPointer joint;
  traceType movementPerStep;
  if (estimateNumberOfRotationSteps > estimateNumberOfTranslationSteps) {
    // rotation is predominant
     joint = i_controller.resolveJoint(BaseJoint::Rotational);
     float averageMagnitude =
       (magnitude(i_trace.getEndPoint()) + magnitude(i_trace.getStartPoint())) / 2;
     movementPerStep = averageMagnitude * joint->getMovementPerStep();
  } else {
    joint = i_controller.resolveJoint(BaseJoint::Translational);
    movementPerStep = joint->getMovementPerStep();
  }

  // select the frequency, take the pull in frequency if the robot speed is to high to handle
  int requiredFrequency = m_robotSpeed / movementPerStep;
  int pullInFrequency = joint->getMotor()->getMaximumSpeed();
  if (requiredFrequency <= pullInFrequency) {
    m_jointSpeed = requiredFrequency;
  } else {
    m_jointSpeed = pullInFrequency;
  }
}
