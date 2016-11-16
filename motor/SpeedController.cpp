#include <macroHeader.h>
#include <BaseJoint.h>
#include <Trace.h>
#include "SpeedController.h"

SpeedController::SpeedController()
        : SpeedController("SpeedController", 0)
{}


SpeedController::SpeedController(std::string i_name)
        : m_name(i_name, 0){
}

SpeedController::SpeedController(std::string i_name, float i_robotSpeed)
        : m_name(i_name, 0),
          m_robotSpeed(i_robotSpeed){
}



void SpeedController::notifyStep(const BaseJoint::JointPointer& i_joint,
                                 const unsigned int& i_numberOfSteps) {
}


void SpeedController::acknowledgeSpeed(const unsigned int& i_speed) {
}


void SpeedController::prepareSpeedController(const Trace& i_trace,
                                            const JointController& i_controller) {
}
