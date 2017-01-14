#include <macroHeader.h>
#include <BaseJoint.h>
#include <Trace.h>
#include "SpeedController.h"
#include <SequenceVector.h>
#include <JointController.h>
#include <MovementRegistrator.h>

SpeedController::SpeedController()
  : SpeedController(Type::None, 0, 0) {
}


SpeedController::SpeedController(const Type& i_type)
  : SpeedController(i_type, 0, 0) {
}


SpeedController::SpeedController(const Type& i_type, const float& i_robotSpeed)
  : SpeedController(i_type, i_robotSpeed, 0) {
}


SpeedController::SpeedController(const Type& i_type,
				 const float& i_robotSpeed,
				 const int& i_currentSequenceVectorPosition)
  : m_type(i_type),
    m_robotSpeed(i_robotSpeed),
    m_currentSequenceVectorPosition(i_currentSequenceVectorPosition),
    m_movementRegistrator(std::make_shared<MovementRegistrator>()) {
}


void SpeedController::acknowledgeSpeed(const unsigned int& i_speed,
                                       SequenceVector* i_sequenceVector) {
  LOG_DEBUG("Set motor speed is: " << i_speed);
  i_sequenceVector->modifySpeed(i_speed, m_currentSequenceVectorPosition);
  m_currentSequenceVectorPosition = i_sequenceVector->getSequenceVector().size();
}


std::weak_ptr<MovementRegistrator> SpeedController::getRegistrator() const {
  return std::weak_ptr<MovementRegistrator>(m_movementRegistrator);
}


/*
SpeedController::JointStepVector SpeedController::estimateSteps
(const Trace& i_trace, const JointController& i_controller) {

  if(i_controller.getNumberOfJoints() != 2) {
    LOG_ERROR("Currently only works with 2 joint");
  }
  int numberOfSteps;
  JointStepVector jointStepVector;
  BaseJoint::JointPointer currentJoint = i_controller.resolveJoint(BaseJoint::Rotational);
  numberOfSteps = abs
    (i_trace.getStartPoint().getAngleBetweenPoints(i_trace.getEndPoint())
     / currentJoint->getMovementPerStep());

  jointStepVector.push_back
    (std::make_tuple(currentJoint, numberOfSteps));
  
  currentJoint = i_controller.resolveJoint(BaseJoint::Translational);
  numberOfSteps = abs(magnitude(i_trace.getEndPoint()) - magnitude(i_trace.getStartPoint()) /
        currentJoint->getMovementPerStep());

  jointStepVector.push_back(std::make_tuple(currentJoint, numberOfSteps));
  return jointStepVector;  
}
*/

