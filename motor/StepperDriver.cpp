// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <StepperDriver.h>
#include <StateSequence.h>
#include <SequenceVector.h>

// Constructor
StepperDriver::StepperDriver()
  : StepperDriver({2, 3, 4},
		  "CW",
		  200,
		  200,
		  500) {
}


StepperDriver::StepperDriver(const PinVector& i_pinVector)
  : StepperDriver(i_pinVector,
		  "CW",
		  200,
		  200,
		  500) {
}

StepperDriver::StepperDriver(const PinVector& i_pinVector,
			     const std::string& i_defaultDirection,
			     const int& i_pullIn,
			     const int& i_pullOut,
			     const int& i_max)
  : BaseMotor(BaseMotor::MotorType::StepperDriver, i_pinVector, i_defaultDirection),
    m_pullIn(i_pullIn),
    m_pullOut(i_pullOut),
    m_maxSpeed(i_max),
    m_incremental(10) {
}


bool StepperDriver::setEnable(const bool& i_setEnable) {
    int newState;
    if (i_setEnable) {
      newState = !DEFAULT_STATE;
    } else {
      newState = DEFAULT_STATE;
    }
    if (newState != m_currentPinState.getPinState(enablePin())) {
      m_currentPinState.update(enablePin(), newState);
      return true;
    } else {
      LOG_DEBUG("No need to change the pin state!");
      return false;
    }
}


int StepperDriver::getPinValueForDirection(
    const std::string &i_directionString) {
    /*
      Function will give the direction pin value given a directionEnum
      First instance this function will be used at the StepperDriver
    */

  if (i_directionString == getDefaultDirection())
    return DEFAULT_STATE;
  else
    // reverse the default state
    return static_cast<int>(!static_cast<bool>(DEFAULT_STATE));
}


bool StepperDriver::setDirection(const std::string& i_directionString) {
  int newPinValue = getPinValueForDirection(i_directionString);
  if (newPinValue != m_currentPinState.getPinState(directionPin())) {
    LOG_INFO("Current Pin Value: " << m_currentPinState.getPinState(directionPin()) <<
            "\t" << "New pin Value: " << newPinValue);
      m_currentPinState.update(directionPin(), newPinValue);
    return true;
  } else {
    return false;
  }
}


void StepperDriver::moveStep(
    const std::string& i_direction,
    StateSequence* i_stateSequence) {

  PinStateVector pinStateVector;
  // Enable the driver!
  if (setEnable(true) | setDirection(i_direction))
    pinStateVector.push_back(m_currentPinState);

  m_currentPinState.update(stepPin(), !DEFAULT_STATE);
  pinStateVector.push_back(m_currentPinState);
  m_currentPinState.update(stepPin(), DEFAULT_STATE);
  pinStateVector.push_back(m_currentPinState);

  if (!getHoldMotor()) {
    // Disable the driver!!
    if (setEnable(false))
      pinStateVector.push_back(m_currentPinState);
  }

  if (!i_stateSequence->addToSequence(pinStateVector))
    LOG_ERROR("Could not add the pin state vector to the sequence!");
}


void StepperDriver::moveSteps(
    const std::string& i_direction,
    const int& i_numberOfSteps,
    SequenceVector* o_sequenceVector) {
  // check that we start with a non empty sequenceVector
  if (o_sequenceVector->numberOfSequences() == 0) {
    StateSequence stateSequence;
    o_sequenceVector->appendStateSequence(stateSequence, true);
  }

  // get the current hold after step status
  bool holdMotorAfterSteps = getHoldMotor();
  // set the motor
  setHoldMotor(true);

  for (int i = 0;
       i< i_numberOfSteps;
       ++i) {
    LOG_DEBUG("Setting step: " << i);
    StateSequence stateSequence;
    moveStep(i_direction, &stateSequence);
    LOG_DEBUG("done setting step: " << i);
    if (!o_sequenceVector->addToSequence(stateSequence)){
      LOG_DEBUG("Could not add newly created sequence to o_sequencevector");
      o_sequenceVector->appendStateSequence(stateSequence,
                                           false);
    }
  }

  setHoldMotor(holdMotorAfterSteps);
  if (!getHoldMotor()) {
    LOG_DEBUG("No power on coils!");
    // if we needed to change the state of the enable pin
    if (setEnable(false)) {
      //Try to force it on the last sequence of the sequence vector
      if (!o_sequenceVector->addToSequence(getCurrentPinState(), true)) {
        // if it failes add it to as a seperate state sequence
        o_sequenceVector->appendStateSequence
          (StateSequence(300, 1 , PinStateVector({getCurrentPinState()})),true);
      }
    }
  }
}


int StepperDriver::getMaximumSpeed() const {
  // if it was statonary
  if (m_speed == 0) {
    return m_pullIn;
  }
  // Suggest 1/m_incremental of the diff between minimum and max + current
  int suggestion = m_speed + m_incremental;
  if (suggestion < m_pullIn) {
    return m_pullIn;
  }
  if (suggestion >= m_maxSpeed) {
    return m_maxSpeed;
  } else {
    return suggestion;
  }
}


int StepperDriver::getMinimumSpeed() const {
  if (m_speed <= m_pullOut) {
    return 0;
  }
  // Suggest the current speed minus a bit
  int suggestion = m_speed -  m_incremental;
  if (suggestion < m_pullOut) {
    return m_pullOut;
  } else {
    return suggestion;
  }
}


void StepperDriver::displayPinState(const PinState& i_pinState) const {
  std::string pinName;
  for (auto itr = i_pinState.getPinVector().begin();
      itr != i_pinState.getPinVector().end();
      itr++) {
    if ((*itr) == enablePin())
      pinName = "Enable Pin";
    else if ((*itr) == directionPin())
      pinName="Direction Pin";
    else if ((*itr) == stepPin())
      pinName="Step Pin";
    else
      pinName="unidentified pin: " + std::to_string(*itr);

    LOG_DEBUG("State of: " << pinName << ", " << i_pinState.getPinState(*itr));
  }
}

void StepperDriver::displayPinState() const {
  displayPinState(getCurrentPinState());
}


void StepperDriver::getMotorStatus(const PinState& i_pinState,
				   bool* i_isEnabled,
				   std::string* i_rotationDirection) const {
  *i_isEnabled = (DEFAULT_STATE != i_pinState.getPinState(enablePin()));
  LOG_DEBUG("Numeric value of state is: " << i_pinState.getNumericValue());
  LOG_DEBUG("Stepper is active?" << *i_isEnabled);
  LOG_DEBUG("State of the direction pin is: " << i_pinState.getPinState(directionPin()));
  if (DEFAULT_STATE == i_pinState.getPinState(directionPin())) {
    LOG_DEBUG("Resolved direction is default direction");
    *i_rotationDirection = getDefaultDirection();
  } else {
    LOG_DEBUG("Resolved direction is not default direction");
    LOG_DEBUG("getDefaultDirection() is: " << getDefaultDirection());
    if (getDefaultDirection().compare("CCW") == 0) {
      LOG_DEBUG("Current rotation direction is thus: CW");
      *i_rotationDirection = "CW";
    } else {
      LOG_DEBUG("Current rotation direction is thus: CCW");
      *i_rotationDirection = "CCW";
    }
  }
}
