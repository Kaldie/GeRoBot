#include <macroHeader.h>
#include <BaseJoint.h>
#include <Trace.h>
#include "SpeedController.h"
#include <SequenceVector.h>

SpeedController::SpeedController()
  : SpeedController("SpeedController", 0)
{}


SpeedController::SpeedController(std::string i_name)
  : SpeedController(i_name, 0, 0){
}

SpeedController::SpeedController(std::string i_name, float i_robotSpeed)
  : SpeedController(i_name, i_robotSpeed, 0){
}

SpeedController::SpeedController(std::string i_controllerName,
                float i_robotSpeed,
                int i_currentSequenceVectorPosition)
  : m_name(i_controllerName),
    m_robotSpeed(i_robotSpeed),
    m_currentSequenceVectorPosition(i_currentSequenceVectorPosition) {
    }



void SpeedController::notifyStep(const BaseJoint::JointPointer& i_joint,
                                 const unsigned int& i_numberOfSteps) {
}


void SpeedController::acknowledgeSpeed(const unsigned int& i_speed,
                                       SequenceVector* i_sequenceVector) {
  i_sequenceVector->modifySpeed(i_speed, m_currentSequenceVectorPosition);
  m_currentSequenceVectorPosition = i_sequenceVector->getSequenceVector().size();

}


void SpeedController::prepareSpeedController(const Trace& i_trace,
                                            const JointController& i_controller) {
}
