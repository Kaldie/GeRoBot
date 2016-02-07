// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "./JointController.h"
#include "./SpeedController.h"
#include "./BaseMotor.h"
#include "./StepperDriver.h"

SpeedController::SpeedController()
  : SpeedController(0)
{}


SpeedController::SpeedController(const traceType& i_robotSpeed)
  : m_jointMap({}),
    m_motorFrequency(0),
    m_robotSpeed(i_robotSpeed),
    m_achievedSpeeds{0},
    m_averageElements(5)
{}


bool SpeedController::adviseSpeed(int* o_speed) const {
  // check if the speed needs to be changed
  float currentSpeed = calculateCurrentSpeed();
  if (m_robotSpeed * 0.95 <= currentSpeed &&
      m_robotSpeed * 1.05 >= currentSpeed) {
    LOG_DEBUG("No need to change the speed");
    *o_speed = m_motorFrequency;
    return false;
  }
  if (m_robotSpeed * 1.05 < currentSpeed) {
    LOG_DEBUG("Breaking is neeeded!");
    *o_speed = adviceAction(currentSpeed,
                            false);
  }
  if (m_robotSpeed * 0.95 > currentSpeed) {
    LOG_DEBUG("Acceleration is needed!");
    *o_speed = adviceAction(currentSpeed,
                            true);
    LOG_DEBUG("advised speed " << *o_speed);
  }
  return true;
}


void SpeedController::notifyStep(const BaseJoint::JointPointer& i_joint,
                                 const unsigned int& i_numberOfSteps) {
  BaseJoint::WeakJointPointer joint(i_joint);
  // set the motor speed to the pull in frequency
  // to ensure calculations works as expected
  auto element = m_jointMap.find(joint);
  if (element == m_jointMap.end()) {
    BaseMotor* motor = i_joint->getMotor();
    // Add this joint
    m_jointMap[joint] = JointInfo
      {0,                                            // number of steps
       {static_cast<float>(motor->getSpeed())},      // front_list frequency
       {},                                           // front list missed steps
       false};                                       // updated the missed step list
    // check if the motor frequency is initialised
    if (m_motorFrequency == 0) {
      initialiseMotorFrequency(i_joint);
    }
  }
  updateJointMapElement(&m_jointMap[joint], i_numberOfSteps);
}


void SpeedController::acknowledgeSpeed(const unsigned int& i_frequency) {
  if (i_frequency == 0) {
    LOG_ERROR("Speed of 0 is not allowed!");
  }
  // update the achieved speed list based on this input
  m_motorFrequency = i_frequency;
  m_achievedSpeeds.emplace_front(calculateCurrentSpeed());
  // number of steps the most active join sets
  int numberOfSteps(currentNumberOfSteps());
  // for each joint
  LOG_DEBUG("Acknowledging speed of " << i_frequency);
  for (auto& element : m_jointMap) {
    JointInfo* jointInfo = &element.second;
    // update the isUpdated
    jointInfo->m_missedStepIsUpdated = false;
    // get a shared joint pointer
    BaseJoint::JointPointer joint(element.first.lock());
    if (!joint) { LOG_ERROR("Could not get the shared pointer!");}
    if (jointInfo->m_numberOfSteps > 0) {
      // update the joint info if the joint was not idle
      // calculate the number of missed steps for the acknodged speed
      float missedSteps =
        jointInfo->m_missedSteps.front() * joint->getMotor()->getSpeed() / i_frequency;
      // calculate the frequency of the motor
      float newFrequency = std::round(jointInfo->m_numberOfSteps /
                                      (numberOfSteps + missedSteps) * i_frequency);
      // update the joint
      joint->getMotor()->setSpeed(newFrequency);
#ifdef DEBUG
      if (jointInfo->m_missedSteps.front() != 0) {
        LOG_DEBUG("Total number of set steps is: " << numberOfSteps);
        LOG_DEBUG("Has set : " << jointInfo->m_numberOfSteps << " number of steps.");
        LOG_DEBUG("This joint has missed steps: " <<  missedSteps);
        LOG_DEBUG("The resulting speed is thus: " << newFrequency);
      }
#endif
      // update the frequency list
      jointInfo->m_frequency.emplace_front(newFrequency);
      trimList(&jointInfo->m_frequency, m_averageElements);
      // reset the number of steps
      jointInfo->m_numberOfSteps = 0;
    } else {
      // update the number of steps if the joint was idle
      float missedSteps =
        static_cast<float>(joint->getMotor()->getSpeed()) / i_frequency * numberOfSteps;
      jointInfo->m_numberOfSteps -= round(missedSteps);
      LOG_DEBUG("Frequency: " << i_frequency);
      LOG_DEBUG("Number of steps in this session: " << numberOfSteps);
      LOG_DEBUG("Current motor speed of this joint: " << joint->getMotor()->getSpeed());
      LOG_DEBUG("Found Idle joint. Increased the missed steps with : " <<
                missedSteps);
    }
  }
}


float SpeedController::estimateCurrentSpeed() const {
  return average(m_achievedSpeeds);
}


float SpeedController::calculateCurrentSpeed() const {
  float currentSpeed = getCurrentStepedDistance() / currentNumberOfSteps()
    * m_motorFrequency;
  LOG_DEBUG("Distance traveled during these steps: " << getCurrentStepedDistance());
  LOG_DEBUG("Number of steps: " << currentNumberOfSteps());
  LOG_DEBUG("Current frequency: " << m_motorFrequency);
  LOG_DEBUG("Given a speed of: " << currentSpeed);
  return currentSpeed;
}


float SpeedController::getCurrentStepedDistance() const {
  float distanceTraveled = 0;
  for (const auto& jointElement : m_jointMap) {
    if (jointElement.second.m_numberOfSteps > 0) {
      distanceTraveled +=
        jointElement.first.lock()->distancePerStep() *
        jointElement.second.m_numberOfSteps;
    }
  }
  return distanceTraveled;
}


void SpeedController::updateJointMapElement
(JointInfo* i_info, const int i_numberOfSteps) {
  // update the missed step list
  if (!i_info->m_missedStepIsUpdated) {
    if (i_info->m_numberOfSteps < 0) {
      // add the new missed steps
      i_info->m_missedSteps.emplace_front
        (std::abs(i_info->m_numberOfSteps));
    } else {
      i_info->m_missedSteps.emplace_front(0);
     }
    // check the bounds, update if needed
    trimList(&i_info->m_missedSteps, m_averageElements);
    // set  that we updated the missed step list
    i_info->m_missedStepIsUpdated = true;
  }
  // update the m_number of steps
  if (i_info->m_numberOfSteps > 0) {
    i_info->m_numberOfSteps += i_numberOfSteps;
  } else {
    i_info->m_numberOfSteps = i_numberOfSteps;
  }
}


float SpeedController::adviceAction(const float& i_estimatedSpeed,
                                    const bool& i_accelerate) const {
  float requestedFrequency
    (m_robotSpeed / i_estimatedSpeed * m_motorFrequency);
  LOG_DEBUG("motor freq: " << m_motorFrequency);
  LOG_DEBUG("m_robotSpeed: " << m_robotSpeed);
  LOG_DEBUG("i_estimatedSpeed: " << i_estimatedSpeed);
  LOG_DEBUG("The resquested frequency is: " << requestedFrequency);
  int lowerBound, upperBound;
  if (!getFrequencyBounds(&lowerBound, &upperBound)) {
    LOG_DEBUG("Lower bound is: " << lowerBound <<
              " Upper bound is: " << upperBound);
    LOG_ERROR("Could not solve breaking question!");
  }
  if (i_accelerate) {
    if (upperBound < requestedFrequency) {
      return upperBound;
    } else {
      return requestedFrequency;
    }
  } else {
    if (lowerBound > requestedFrequency) {
      return lowerBound;
    } else {
      return requestedFrequency;
    }
  }
}


int SpeedController::currentNumberOfSteps() const {
  int numberOfSteps = 0;
  for (const auto& element : m_jointMap) {
    if (numberOfSteps < element.second.m_numberOfSteps) {
      numberOfSteps = element.second.m_numberOfSteps;
    }
  }
  return numberOfSteps;
}


template <class T>
void SpeedController::average(const std::forward_list<T>& i_forwardList,
                              T* o_average) {
  unsigned int i = 0;
  T summedValue = 0;
  for (const auto& element : i_forwardList) {
    ++i;
    summedValue += element;
  }
  if (i == 0) {
    *o_average = 0;
  } else {
    *o_average = summedValue / i;
  }
}


template <class T>
T SpeedController::average(const std::forward_list<T>& i_forwardList) {
  T value;
  average(i_forwardList, &value);
  return value;
}


template <class T>
void SpeedController::trimList(std::forward_list<T>* i_forwardList,
                               const int& i_size) {
    if (std::distance(i_forwardList->begin(),
                      i_forwardList->end()) > i_size) {
      auto begin = i_forwardList->begin();
      for (int i = 0;
           i < i_size;
           ++i) {
        ++begin;
      }
      i_forwardList->erase_after(begin, i_forwardList->end());
    }
}


float SpeedController::getMaximumFrequency(const bool& i_excludeIdle /*= false*/) const {
  float max = std::numeric_limits<float>::min();
  for (const auto element : m_jointMap) {
    if (i_excludeIdle && element.second.m_numberOfSteps <= 0) {
      continue;
    }
    if (!element.second.m_frequency.empty()) {
      if (max < element.second.m_frequency.front()) {
        max = element.second.m_frequency.front();
      }
    }
  }
  return max;
}


float SpeedController::getMinimumFrequency(const bool& i_excludeIdle /*= false*/) const {
  float min = std::numeric_limits<float>::max();
  for (const auto element : m_jointMap) {
    if (i_excludeIdle && element.second.m_numberOfSteps <= 0) {
      continue;
    }
    if (min > element.second.m_frequency.front()) {
      min = element.second.m_frequency.front();
    }
  }
  return min;
}


bool SpeedController::getFrequencyBounds(int* o_lowerBound,
                                         int* o_upperBound) const {
  *o_lowerBound = std::numeric_limits<int>::min();
  *o_upperBound = std::numeric_limits<int>::max();
  BaseMotor* motor;
  int maxSpeed, minSpeed;
  float averageMissedSteps;
  for (const auto& element : m_jointMap) {
    average(element.second.m_missedSteps, &averageMissedSteps);
    motor = element.first.lock()->getMotor();
    maxSpeed = motor->getMaximumSpeed() *
      (averageMissedSteps + 1);
    if (element.second.m_numberOfSteps > 0) {
      minSpeed = motor->getMinimumSpeed() *
        currentNumberOfSteps() / element.second.m_numberOfSteps;
    } else {
      float tmpMinSpeed;
      average(element.second.m_frequency, &tmpMinSpeed);
      minSpeed = static_cast<int>(tmpMinSpeed);
    }
      if (maxSpeed < *o_upperBound) {
      *o_upperBound = maxSpeed;
    }
    if (minSpeed > *o_lowerBound) {
      *o_lowerBound = minSpeed;
    }
  }
  // check if it is withing bounds
  if (*o_lowerBound > *o_upperBound) {
    return false;
  }
  return true;
}


void SpeedController::initialiseMotorFrequency(const BaseJoint::JointPointer& i_joint) {
  StepperDriver* motor = dynamic_cast<StepperDriver*>(i_joint->getMotor());
  if (motor) {
    m_motorFrequency = motor->getPullIn();
    LOG_DEBUG("Set motor frequency to pull in: "
              << m_motorFrequency);
  } else {
    LOG_ERROR("Could not resolve the motor pointer");
  }
}
