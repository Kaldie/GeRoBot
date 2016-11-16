// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "./PrescribedSpeedController.h"
#include "./JointController.h"
#include "./PrescribedSpeedController.h"
#include "./BaseMotor.h"
#include "./StepperDriver.h"
#include <list>

PrescribedSpeedController::PrescribedSpeedController()
  : PrescribedSpeedController(0)
{}


PrescribedSpeedController::PrescribedSpeedController(const traceType& i_robotSpeed)
  : m_jointMap({}),
    m_motorFrequency(0),
    m_robotSpeed(i_robotSpeed),
    m_achievedSpeeds{0},
    m_averageElements(5),
    m_isConservative(false)
    {}

bool PrescribedSpeedController::adviseSpeed(int* o_speed) const {
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
    LOG_DEBUG("Advised speed " << *o_speed);
  }
  return true;
}


void PrescribedSpeedController::notifyStep(const BaseJoint::JointPointer& i_joint,
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
       0,                                            // number of steps taken
       false};                                       // updated the missed step list
    // check if the motor frequency is initialised
    if (m_motorFrequency == 0) {
      initialiseMotorFrequency(i_joint);
    }
  }
  updateJointMapElement(&m_jointMap[joint], i_numberOfSteps);
}


void PrescribedSpeedController::acknowledgeSpeed(const unsigned int& i_frequency) {
  if (i_frequency == 0) {
    LOG_ERROR("Speed of 0 is not allowed!");
  }
  // update the achieved speed list based on this input
  LOG_DEBUG("Acknowledging speed of " << i_frequency);
  m_motorFrequency = i_frequency;
  m_achievedSpeeds.emplace_front(calculateCurrentSpeed());
  // number of steps the most active join sets
  int numberOfSteps(currentNumberOfSteps());
  BaseJoint::JointPointer joint;
  // for each joint
  for (auto& element : m_jointMap) {
    JointInfo* jointInfo = &element.second;
    // update the isUpdated
    jointInfo->m_missedStepIsUpdated = false;
    // get a shared joint pointer
    joint = element.first.lock();
    if (!joint) { LOG_ERROR("Could not get the shared pointer!");}
    if (jointInfo->m_numberOfSteps > 0) {
      // update the joint info if the joint was not idle
      // calculate the number of missed steps for the acknodged speed
      float missedSteps = jointInfo->m_missedSteps.front() *
        joint->getMotor()->getSpeed() / m_motorFrequency;
      // calculate the frequency of the motor
      float newFrequency = std::round(jointInfo->m_numberOfSteps /
                                      (numberOfSteps + missedSteps) * i_frequency);
      // update the joint
      joint->getMotor()->setSpeed(newFrequency);
      // update the frequency and number of steps taken list
      jointInfo->m_frequency.emplace_front(newFrequency);
      jointInfo->m_driverFrequency = m_motorFrequency;
      trimList(&jointInfo->m_frequency, m_averageElements);
      // reset the number of steps
      jointInfo->m_numberOfSteps = 0;
#ifdef DEBUG
      if (jointInfo->m_missedSteps.front() != 0) {
        LOG_DEBUG("Total number of set steps is: " << numberOfSteps);
        LOG_DEBUG("Has set : " << jointInfo->m_numberOfSteps << " number of steps.");
        LOG_DEBUG("This joint has missed steps: " <<  missedSteps);
        LOG_DEBUG("The resulting speed is thus: " << newFrequency);
      }
#endif
    } else {
      // update the number of steps if the joint was idle
      float missedSteps = static_cast<float>(joint->getMotor()->getSpeed()) /
        m_motorFrequency * numberOfSteps;
      jointInfo->m_numberOfSteps -= round(missedSteps);
      LOG_DEBUG("Frequency: " << i_frequency);
      LOG_DEBUG("Number of steps in this session: " << numberOfSteps);
      LOG_DEBUG("Current motor speed of this joint: " << joint->getMotor()->getSpeed());
      LOG_DEBUG("Found Idle joint. Increased the missed steps with : " <<
                missedSteps);
    }
  }
}


float PrescribedSpeedController::estimateCurrentSpeed() const {
  return average(m_achievedSpeeds);
}


float PrescribedSpeedController::calculateCurrentSpeed() const {
  float currentSpeed = getCurrentStepedDistance() / currentNumberOfSteps()
    * m_motorFrequency;
  LOG_DEBUG("Distance traveled during these steps: " << getCurrentStepedDistance());
  LOG_DEBUG("Number of steps: " << currentNumberOfSteps());
  LOG_DEBUG("Current frequency: " << m_motorFrequency);
  LOG_DEBUG("Given a speed of: " << currentSpeed);
  return currentSpeed;
}


float PrescribedSpeedController::getCurrentStepedDistance() const {
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


void PrescribedSpeedController::updateJointMapElement
(JointInfo* i_info, const int i_numberOfSteps) {
  // update the missed step list
  if (!i_info->m_missedStepIsUpdated) {
    if (i_info->m_numberOfSteps < 0) {
      // add the new missed steps
      i_info->m_missedSteps.emplace_front
        (-1 * i_info->m_numberOfSteps);
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


float PrescribedSpeedController::adviceAction(const float& i_estimatedSpeed,
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
  LOG_DEBUG("Lower bound is: " << lowerBound);
  LOG_DEBUG("Upper bound is: " << upperBound);
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


int PrescribedSpeedController::currentNumberOfSteps() const {
  int numberOfSteps = 0;
  for (const auto& element : m_jointMap) {
    if (numberOfSteps < element.second.m_numberOfSteps) {
      numberOfSteps = element.second.m_numberOfSteps;
    }
  }
  return numberOfSteps;
}


template <class T>
void PrescribedSpeedController::average(const std::forward_list<T>& i_forwardList,
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
T PrescribedSpeedController::average(const std::forward_list<T>& i_forwardList) {
  T value;
  average(i_forwardList, &value);
  return value;
}


template <class T>
void PrescribedSpeedController::trimList(std::forward_list<T>* i_forwardList,
                               const int& i_size) {
  if (std::distance(i_forwardList->begin(),
                    i_forwardList->end()) > i_size) {
    auto cutOfPoint = i_forwardList->begin();
    for (int i = 0;
         i < i_size;
         ++i) {
      ++cutOfPoint;
    }
    i_forwardList->erase_after(cutOfPoint, i_forwardList->end());
  }
}


bool PrescribedSpeedController::getFrequencyBounds(int* o_lowerBound,
                                         int* o_upperBound) const {
  *o_lowerBound = std::numeric_limits<int>::min();
  *o_upperBound = std::numeric_limits<int>::max();
  float tmpLowerBound, tmpUpperBound;
  for (auto element : m_jointMap) {
    BaseMotor* motor = element.first.lock()->getMotor();
    if (element.second.m_numberOfSteps <= 0) {
      LOG_DEBUG("Getting bounds of idle motor!");
      boundsOfIdleMotor(&tmpLowerBound, &tmpUpperBound,
                        element.second, motor);
    } else {
      LOG_DEBUG("Getting bounds of working motor!");
      boundsOfWorkingMotor(&tmpLowerBound, &tmpUpperBound,
                           element.second, motor);
    }
    if (tmpUpperBound < *o_upperBound) {
      *o_upperBound = tmpUpperBound;
    }
    if (tmpLowerBound > *o_lowerBound) {
      *o_lowerBound = tmpLowerBound;
    }
  }
  // check if it is withing bounds
  if (*o_lowerBound > *o_upperBound) {
    return false;
  }
  return true;
}


void PrescribedSpeedController::boundsOfWorkingMotor(float* o_lowerBound,
                                           float* o_upperBound,
                                           const JointInfo& i_jointInfo,
                                           const BaseMotor* i_motor) const {
  float averageMissedSteps;
  average(i_jointInfo.m_missedSteps, &averageMissedSteps);
  // ratio to which this motor works and the hardest working motor
  float utilisationFactor =
    static_cast<float>(i_jointInfo.m_numberOfSteps) / currentNumberOfSteps();
  *o_upperBound = static_cast<float>(i_motor->getMaximumSpeed()) *
    (averageMissedSteps + utilisationFactor);
  *o_lowerBound = i_motor->getMinimumSpeed() * utilisationFactor;
  LOG_DEBUG("Maximum speed is: " << i_motor->getMaximumSpeed());
  LOG_DEBUG("Minimum speed is: " << i_motor->getMinimumSpeed());
  LOG_DEBUG("Utilisation factor is: " << utilisationFactor);
  LOG_DEBUG("Average missed steps: " << averageMissedSteps);
}


void PrescribedSpeedController::boundsOfIdleMotor(float* o_lowerBound,
                                        float* o_upperBound,
                                        const JointInfo& i_jointInfo,
                                        BaseMotor* i_motor) const {
  int currentSpeedMotor = i_motor->getSpeed();
  i_motor->setSpeed(i_jointInfo.m_driverFrequency);
  float changeRatio =
    estimateNextElement(i_jointInfo.m_frequency) / i_jointInfo.m_frequency.front();
  *o_lowerBound = i_motor->getMinimumSpeed() * changeRatio;
  *o_upperBound = i_motor->getMaximumSpeed() * changeRatio;
  i_motor->setSpeed(currentSpeedMotor);
  LOG_DEBUG("The change ratio is: " << changeRatio);
}


void PrescribedSpeedController::initialiseMotorFrequency(const BaseJoint::JointPointer& i_joint) {
  BaseMotor* motor = i_joint->getMotor();
  if (motor) {
    m_motorFrequency = motor->getMaximumSpeed();
    LOG_DEBUG("Set motor frequency to pull in: "
              << m_motorFrequency);
  } else {
    LOG_ERROR("Could not resolve the motor pointer");
  }
}


template <class T>
float PrescribedSpeedController::estimateNextElement(const std::forward_list<T>& i_forwardList,
                                           const int& i_numberOfDerivatives /*3*/) const {
  if (i_forwardList.empty()) {
    LOG_ERROR("Could not send an empty list to estimate next element");
  }
  //  unsigned int sizeOfList = i_forwardList.size();
  std::forward_list<T> values(i_forwardList);
  std::forward_list<T> deravetive;
  T averageDirevative;
  float nextElement(0.0);
  float fraction(1.0);
  T nextValue(0);
  bool isValid(false);
  for (int numberOfDerivatives = 1;
       numberOfDerivatives <= i_numberOfDerivatives;
       ++numberOfDerivatives) {
    // adjust fraction
    fraction *= 1/numberOfDerivatives;
    // determine the element 1 before the end
   auto listIterator =  values.begin();
    while (true) {
      nextValue = *listIterator;
      ++listIterator;
      // Continue loop while next element is not the end
      if (listIterator == i_forwardList.end()) {
        break;
      }
      deravetive.emplace_front(nextValue - *listIterator);
      isValid = true;
    }
    // check if we found at least 1 value
    if (!isValid) {
      break;
    }
    // calculate the average derivative
    average(deravetive, &averageDirevative);
    // if it changes alot the the first one
    if (averageDirevative > deravetive.front() * 1.05 or
        averageDirevative < deravetive.front() * 0.95) {
      nextElement += static_cast<float>(deravetive.front()) * fraction;
    } else {
      nextElement += static_cast<float>(averageDirevative) * fraction;
    }
    values = deravetive;
    deravetive.clear();
    isValid = false;
  }
  return nextElement;
}
