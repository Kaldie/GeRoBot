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


traceType SpeedController::steppedDistance() const {
  traceType steppedDistance = 0.0;
  for (const auto& stepItem : m_stepMap) {
    LOG_DEBUG("Distance per step: " << stepItem.first.lock()->distancePerStep());
    steppedDistance +=
      stepItem.first.lock()->distancePerStep() * stepItem.second;
  }
  return steppedDistance;
}


traceType SpeedController::getRobotSpeed(const int i_speed) const {
  traceType distance(steppedDistance());
  LOG_DEBUG("Stepped distance: " << distance);
  return distance / getMaximumConsecutiveSteps() * i_speed;
}


bool SpeedController::adviseSpeed(int* o_speed) const {
  validatedJoints();
  traceType currentRobotSpeed = getRobotSpeed(m_motorSpeed);

  // if the robot is running at the requested speed then it is ok
  if (currentRobotSpeed < m_robotSpeed * 1.05 &&
      currentRobotSpeed > m_robotSpeed * 0.95) {
    LOG_DEBUG("Current robot speed: " << currentRobotSpeed);
    LOG_DEBUG("IS OK!");
    *o_speed = m_motorSpeed;
    return false;
  }
  int maxSteps = getMaximumConsecutiveSteps();
  traceType distance = steppedDistance();
  // speed that the motors should operate on to provide the robot speed
  int  wantedMotorSpeed = maxSteps/ (distance / m_robotSpeed);
  LOG_DEBUG("Max steps: " <<  maxSteps);
  LOG_DEBUG("Stepped distance: " << distance);
  LOG_DEBUG("Current robot speed: " << currentRobotSpeed);
  LOG_DEBUG("Motor speed : " << m_motorSpeed);
  LOG_DEBUG("Wanted motor speed: " << wantedMotorSpeed);
  LOG_DEBUG("Wanted Robot speed: " << m_robotSpeed);

  // Check if that speed is possible
  if ( currentRobotSpeed < m_robotSpeed) {
    LOG_DEBUG("Speeding up!");
    *o_speed = getLimitingMotorSpeed(&BaseMotor::getMaximumSpeed, true);
    if (*o_speed > wantedMotorSpeed){
      *o_speed = wantedMotorSpeed;
    }
  } else {
    LOG_DEBUG("Breaking!!");
    *o_speed = getLimitingMotorSpeed(&BaseMotor::getMinimumSpeed, false);
    if (*o_speed < wantedMotorSpeed){
      *o_speed = wantedMotorSpeed;
    }
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
  int suggestedMotorSpeed = 
    (m_stepMap.begin()->first.lock()->getMotor()->*getLimitingSpeed)();
  int maxSteps(getMaximumConsecutiveSteps());
  int thisMotorSpeed, resultingSpeed;

  for (const auto& stepItem : m_stepMap) {
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
      LOG_DEBUG("Minimum speed is: " << mapItem.first.lock()->getMotor()->getMinimumSpeed());
      *speedyJoint = mapItem.first;
      return false;
    }
    if (mapItem.first.lock()->getMotor()->getMaximumSpeed() < i_speed) {
      LOG_DEBUG("Maximum speed is: " << mapItem.first.lock()->getMotor()->getMinimumSpeed());
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
  // The requested speed was not available
  // Try to find a common ground
  int minSpeed = 0;
  int maxSpeed = 111111111;
  int maxSteps = getMaximumConsecutiveSteps();
  for (const auto& mapItem : m_stepMap) {
    BaseMotor* motor = mapItem.first.lock()->getMotor();
    LOG_DEBUG("Number of steps this motor sets is: " << mapItem.second);
    LOG_DEBUG("Minimum speed of this motor is: "<< motor->getMinimumSpeed());
    LOG_DEBUG("Maximum speed of this motor is: "<< motor->getMaximumSpeed());
    LOG_DEBUG("This motor would be able to handle the motor speed: " <<
	      maxSteps << " / " <<  mapItem.second << " * " << 
	      motor->getMaximumSpeed() << " = " << 
	      maxSteps / mapItem.second * motor->getMaximumSpeed());
    minSpeed = std::max(motor->getMinimumSpeed(), minSpeed);
    maxSpeed = std::min(maxSteps / mapItem.second * motor->getMaximumSpeed(), 
			maxSpeed);
  }
  LOG_DEBUG("Lowest maximums speed is: " << maxSpeed);
  LOG_DEBUG("Highest minimum speed is: " << minSpeed);

  if (minSpeed > maxSpeed) {
    LOG_ERROR("Could not resolve the speed question!");
    return false;
  }
  if (i_findMax) {
    *o_speed = maxSpeed;
  } else {
    *o_speed = minSpeed;
  }
  return true;
}


void SpeedController::validatedJoints() const {
  for (const auto& mapItem : m_stepMap) {
    if (mapItem.first.expired()) {
      LOG_ERROR("Joint is unexpectely destructed!!");
    }
  }
}
