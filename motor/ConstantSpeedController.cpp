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


ConstantSpeedController::ConstantSpeedController(const float& i_robotSpeed)
  : ConstantSpeedController(i_robotSpeed, 0) {
}


ConstantSpeedController::ConstantSpeedController(const float& i_robotSpeed,
						 const int& i_vectorPosition)
  : SpeedController(SpeedController::Type::Constant, i_robotSpeed, i_vectorPosition),
    m_jointSpeed(0) {
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

  // select the frequency, take the pull in frequency if the robot speed is to high to handle
  int requiredFrequency = m_robotSpeed / movementPerStep;
  int pullInFrequency = joint->getMotor()->getMaximumSpeed();
  if (requiredFrequency <= pullInFrequency) {
    m_jointSpeed = requiredFrequency;
  } else {
    m_jointSpeed = pullInFrequency;
  }
  LOG_DEBUG("Joint speed will be: " << m_jointSpeed);
}
