// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <StepperDriver.h>
#include <StateSequence.h>
#include <SequenceVector.h>

// Constructor
StepperDriver::StepperDriver()
  : BaseMotor({2, 3, 4})
{}


StepperDriver::StepperDriver(const PinVector& i_pinVector)
    : BaseMotor(i_pinVector)
{}


StepperDriver::StepperDriver(
    const PinVector& i_pinVector, const std::string& i_defaultDirection)
    : BaseMotor(i_pinVector, i_defaultDirection)
{}


bool StepperDriver::setEnable(const bool& i_setEnable) {
    PinState pinState = getCurrentPinState();
    int newState;
    if (i_setEnable) {
      newState = !DEFAULT_STATE;
    } else {
      newState = DEFAULT_STATE;
    }
    if (newState != pinState.getPinState(enablePin())) {
      pinState.update(enablePin(), newState);
      setCurrentPinState(pinState);
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
  PinState pinState = getCurrentPinState();
  int newPinValue = getPinValueForDirection(i_directionString);

  if (newPinValue != pinState.getPinState(directionPin())) {
    LOG_INFO("Current Pin Value: " << pinState.getPinState(directionPin()) <<
            "\t" << "New pin Value: " << newPinValue);
    if (newPinValue != pinState.getPinState(directionPin()))
      pinState.update(directionPin(), newPinValue);
    setCurrentPinState(pinState);
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
    pinStateVector.push_back(getCurrentPinState());

  PinState pinState = getCurrentPinState();
  pinState.update(stepPin(), !DEFAULT_STATE);
  pinStateVector.push_back(pinState);
  pinState.update(stepPin(), DEFAULT_STATE);
  pinStateVector.push_back(pinState);

  if (!getHoldMotor()) {
    // Disable the driver!!
    if (setEnable(false))
      pinStateVector.push_back(getCurrentPinState());
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
       i++) {
    LOG_DEBUG("Setting step: " << i);
    StateSequence stateSequence;
    moveStep(i_direction, &stateSequence);
    LOG_DEBUG("done setting step: " << i);
    if (!o_sequenceVector->addToSequence(stateSequence))
      o_sequenceVector->appendStateSequence(stateSequence,
                                           true);
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
