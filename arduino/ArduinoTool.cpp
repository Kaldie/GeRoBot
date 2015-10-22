

/// Empty constructor
ArduinoPlasmaTool::ArduinoTool() :
  ArduinoTool(nullptr, 0, 0 , 0 , 0, LOW) {
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
  : m_robotPointer(i_robot), m_switchOnTime(i_switchOnTime),
    m_switchOffTime(i_switchOffTime), m_coolDownTime(i_coolDownTime),
    m_pinNumber(i_pinNumber), m_offState(i_offState) {
}


void ArduinoTool::changeState(const bool& i_state) {
  if (m_pinState > 3) {
    LOG_ERROR("Pin number cannot be higher then 3!");
  }
  bool pinState;
  if (i_state) {
    pinState = !m_offState;
  } else {
    pinState = m_offState;
  }
  PinState primaryState;
  primaryState.update(m_pinNumber << 8, pinState);
  // get the sequence vector which will set the tool to the state and
  // make sure arduino waits long enough
  SequenceVector sequenceVector = createStates(primaryState, getDelay(i_state));
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

  // if the tool is switched on, and the there is a cool down time we need to calculate if the tool is cold\
  // If yes, we need cooldown time,
  // If no, we dont need it.
  int offTime = calculateOffTime
    (m_robot->getJointController()->getSequenceVector());
  //
  if (offTime < m_coolDownTime) {
    return 0;
  } else {
    return m_switchOnTime;
  }
}


SequenceVector ArduinoTool::createDelayStates(const PinState& i_state,
					      const int& i_delay) {
  int frequency = 100; // equals 2000 hertz
  frequency /= (i_delay / (1 << 15)) + 1; //

  int steps = 1, reps = 1;
  int maxSteps = 10, maxReps = 1 << 15;
  int thisDelay;
  do {

    thisDelay = steps * reps / frequency;
  } while (thisDelay < i_delay)
    StateSequence stateSequence =
  SequenceVector vector;
  msPerStep = i_delay / 100;
  if (

  StateSequence stateSequence(


}
