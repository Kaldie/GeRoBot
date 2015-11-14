// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "./JointController.h"
#include "./SpeedController.h"
#include "./BaseMotor.h"


SpeedController::SpeedController()
  : SpeedController(0) {}


SpeedController::SpeedController(const int& i_defaultSpeed)
  : m_stepMap(),  m_motorSpeed(0), m_robotSpeed(i_defaultSpeed) {
}


void SpeedController::notifyStep(const BaseJoint::JointPointer& i_joint,
				 const int& i_numberOfSteps) {
 updateStepMap(WeakJoint(i_joint), i_numberOfSteps);
}


void SpeedController::updateStepMap(const WeakJoint& i_joint,
				    const int& i_numberOfSteps) {
  // update the step map
  if (m_stepMap.find(i_joint) == m_stepMap.end()) {
    m_stepMap[i_joint] = i_numberOfSteps;
  } else {
    m_stepMap[i_joint] += i_numberOfSteps;
  }
}


int SpeedController::getMaximumConsecutiveSteps() const {
  int maximumConsecutiveSteps = 0;
  for (const auto& item : m_stepMap) {
    if (maximumConsecutiveSteps < item.second) {
      maximumConsecutiveSteps = item.second;
    }
  }
  return maximumConsecutiveSteps;
}


traceType SpeedController::getRobotSpeed(const int i_speed) const {
  traceType steppedDistance = 0.0;
  for (const auto& stepItem : m_stepMap) {
    LOG_DEBUG("Distance per step: " << stepItem.first.lock()->distancePerStep());
    steppedDistance +=
      stepItem.first.lock()->distancePerStep() * stepItem.second;
  }
  // based on the steps currently known and the given speed i_speed
  // what is the robot speed
  LOG_DEBUG("Stepped distance: " << steppedDistance);
  return steppedDistance / getMaximumConsecutiveSteps() * i_speed;
}


bool SpeedController::adviseSpeed(int* o_speed) const {
  validatedJoints();
  traceType currentRobotSpeed = getRobotSpeed(m_motorSpeed);
  // if the robot is running at the requested speed then it is ok
  if (currentRobotSpeed * 0.95 < m_robotSpeed &&
      currentRobotSpeed * 1.05 > m_robotSpeed) {
    *o_speed = m_motorSpeed;
    return false;
  }
  if ( currentRobotSpeed < m_robotSpeed) {
    *o_speed = getLimitingMotorSpeed(&BaseMotor::getMaximumSpeed, true);
  } else {
    *o_speed = getLimitingMotorSpeed(&BaseMotor::getMinimumSpeed, false);
  }
  return true;
}


void SpeedController::acknowledgeSpeed(const int& i_speed) {
  // validate all the joints before starting
  validatedJoints();
  int resultingSpeed;
  m_motorSpeed = i_speed;
  int maxSteps(getMaximumConsecutiveSteps());
  for (auto& mapItem : m_stepMap) {
    resultingSpeed = i_speed * (static_cast<float>(mapItem.second)/ maxSteps);
    mapItem.first.lock()->getMotor()->setSpeed(resultingSpeed);
  }
    m_stepMap.clear();
}

int SpeedController::getLimitingMotorSpeed(int (BaseMotor::*getLimitingSpeed)() const,
					   const bool& i_findMaxAcceleration) const {
  if (m_stepMap.begin()->first.expired()) {
    LOG_ERROR("Joint has been unexpectly destructed!!");
  }
  int suggestedMotorSpeed = (m_stepMap.begin()->first.lock()->getMotor()->*getLimitingSpeed)();
  int maxSteps(getMaximumConsecutiveSteps());
  int thisMotorSpeed, resultingSpeed;
  for (const auto& stepItem : m_stepMap) {
    if (stepItem.first.expired()) {
      LOG_ERROR("Joint has been unexpectly destructed!!");
    }
    thisMotorSpeed = (stepItem.first.lock()->getMotor()->*getLimitingSpeed)();
    if (i_findMaxAcceleration) {
      resultingSpeed = thisMotorSpeed * (maxSteps / stepItem.second);
      if (resultingSpeed < suggestedMotorSpeed) {
	suggestedMotorSpeed = thisMotorSpeed;
      }
    } else {
      if (thisMotorSpeed > suggestedMotorSpeed) {
	suggestedMotorSpeed = thisMotorSpeed;
      }
    }
  }
  // determine all joints can handle this speed or find an alternative
  getAchievable(&suggestedMotorSpeed, i_findMaxAcceleration);
  return suggestedMotorSpeed;
}


bool SpeedController::
isAchievable(const int& i_speed, WeakJoint* speedyJoint) const {
  for(const auto& mapItem : m_stepMap) {
    if (mapItem.first.lock()->getMotor()->getMinimumSpeed() > i_speed) {
      *speedyJoint = mapItem.first;
      return false;
    }
    if (mapItem.first.lock()->getMotor()->getMaximumSpeed() < i_speed) {
      *speedyJoint = mapItem.first;
      return false;
    }
  }
  return true;
}


bool SpeedController::getAchievable
(int* o_speed, const bool& i_findMax) const {
  WeakJoint weakJoint;
  if (isAchievable(*o_speed, &weakJoint)) {
    return true;
  }
  // there is a joint which would not be able to handle the speed
  // Which speed can it handle
  int newSpeed;
  if (i_findMax) {
    newSpeed = weakJoint.lock()->getMotor()->getMaximumSpeed();
  } else {
    newSpeed = weakJoint.lock()->getMotor()->getMinimumSpeed();
  }
  if (isAchievable(newSpeed, &weakJoint)) {
    *o_speed = newSpeed;
  } else {
    LOG_ERROR("Could not resolve a speed question!");
  }
  return false;
}


void SpeedController::validatedJoints() const {
  for (const auto& mapItem : m_stepMap) {
    if (mapItem.first.expired()) {
      LOG_ERROR("Joint is unexpectely destructed!!");
    }
  }
}