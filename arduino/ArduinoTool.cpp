// Copyright [2015] Ruud Cools

#include <macroHeader.h>
#include "./ArduinoTool.h"
#include <Robot.h>


/// Empty constructor
ArduinoTool::ArduinoTool() :
  ArduinoTool(nullptr, 0, 0 , 0 , 0, false) {
}


/// Regular constructor, othertimes are 0
ArduinoTool::ArduinoTool(const Robot::RobotPointer& i_robot,
				     const int& i_switchOnTime,
				     const int& i_pinNumber,
				     const bool& i_offState)
  : ArduinoTool(i_robot, i_switchOnTime, 0 , 0 , i_pinNumber, i_offState) {
}


/// Full fledged
ArduinoTool::ArduinoTool(const Robot::RobotPointer& i_robot,
				     const int& i_switchOnTime,
				     const int& i_switchOffTime,
				     const int& i_coolDownTime,
				     const int& i_pinNumber,
				     const bool& i_offState)
  : m_robot(i_robot), m_switchOnTime(i_switchOnTime),
    m_switchOffTime(i_switchOffTime), m_coolDownTime(i_coolDownTime),
    m_pin(i_pinNumber), m_offState(i_offState) {
}


void ArduinoTool::changeState(const bool& i_state) {
  if (m_pin > 3) {
    LOG_ERROR("Pin number cannot be higher then 3!");
  }
  bool pinState;
  if (i_state) {
    pinState = !m_offState;
  } else {
    pinState = m_offState;
  }
  PinState primaryState;
  primaryState.update(8 + m_pin, pinState);
  // get the sequence vector which will set the tool to the state and
  // make sure arduino waits long enough
  StateSequence stateSequence = createStateSequence(primaryState, getDelay(i_state));
  m_robot->addToSequence(stateSequence);
}


int ArduinoTool::getDelay(const bool& i_state) const {
  // if the state is on, and there is no cool down time defined
  if (i_state) {
    if (m_coolDownTime == 0) {
      // just return the switch on time
      return m_switchOnTime;
    }
  }
  // If the new state is off, return the switchoff time
  if (!i_state) {
      return m_switchOffTime;
  }
  // if the tool is switched on, and the there is a
  // cool down time we need to calculate if the tool is cold
  // If yes, we need cooldown time,
  // If no, we dont need it.
  if (calculateOffTime() < m_coolDownTime) {
    return 0;
  }
  return m_switchOnTime;
}


StateSequence ArduinoTool::createStateSequence(const PinState& i_state,
                                               const int& i_delay) {
  int numberOfSteps = 0;
  int frequency, estimateNumberOfReps;
  float delay = static_cast<float>(i_delay);
  do {
    // this loop estimates the number of steps
    frequency = 190;  // 2K frequency to start with
    numberOfSteps += 1;
    do {
      // estimates the frequency
      frequency -= 10;
      estimateNumberOfReps = delay * (frequency / 100) / numberOfSteps - 0.5;
      estimateNumberOfReps += 1;
    } while (estimateNumberOfReps > (1 << 15) && frequency > 10);
  } while (estimateNumberOfReps > (1 << 15) && numberOfSteps <= 10);
  // check if the delay can be made
  if (numberOfSteps > 10) {
    LOG_ERROR("Was not possible to create a Sequence to ensure delay");
  }
  //Make delay
  StateSequence stateSequence;
  for (int i = 0;
       i < numberOfSteps;
       ++i) {
    stateSequence.addToSequence(i_state, true);
  }
  stateSequence.setNumberOfRepetitions(estimateNumberOfReps);
  stateSequence.setSpeed(frequency);
  return stateSequence;
}


int ArduinoTool::calculateOffTime() const {
  SequenceVector sequenceVector(m_robot->getJointController()->getSequenceVector());
  float currentTime = 0.0;
  PinStateVector pinStateVector;
  const auto rend = sequenceVector.getSequenceVector().rend();
  for (auto stateSequence = sequenceVector.getSequenceVector().rbegin();
       stateSequence != rend;
       ++stateSequence) {
    currentTime += stateSequence->getIntegerSequence().size() *
      stateSequence->getNumberOfRepetitions() / (stateSequence->getSpeed() * 10);
    // Search for the tool pin on
    for (const auto& pinState : stateSequence->getPinStateVector()) {
      if (pinState.hasPin(8 + m_pin)) {
        if (pinState.getPinState(8 + m_pin) != m_offState)
          return static_cast<int>(currentTime);
      }
    }
  }
  return static_cast<int>(currentTime);
}
