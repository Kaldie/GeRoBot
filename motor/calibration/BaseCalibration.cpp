// Copyright [2017] Ruud Cools

#include <macroHeader.h>
#include <BaseCalibration.h>
#include <BaseJoint.h>
#include <CalibrationOutput.h>


BaseCalibration::BaseCalibration(const std::shared_ptr<BaseJoint>& i_joint,
				 const std::shared_ptr<Robot>& i_robot,
				 const BaseCalibration::Type& i_type)
  : m_finished(false),
    m_joint(i_joint),
    m_robot(i_robot),
    m_output(std::make_shared<CalibrationOutput>()),
    m_type(i_type) {
}
  

std::string BaseCalibration::getJointDirection(const bool& i_isInner) const {
  if (m_joint->getMovementType() == BaseJoint::Rotational) {
    if (i_isInner) {
      return "CW";
    } else {
      return "CCW";
    }
  } else if (m_joint->getMovementType() == BaseJoint::Translational) {
    if (i_isInner) {
      return "IN";
    } else {
      return "OUT";
    }
  } else {
    LOG_ERROR("Could not resolve joint type!");
  }
}
