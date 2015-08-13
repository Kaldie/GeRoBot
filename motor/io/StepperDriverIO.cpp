// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "./StepperDriverIO.h"

void StepperDriverIO::build() {
  LOG_DEBUG("Buildiing a stepperdriver!");
  std::string actuatorType = getNode().first_child().text().as_string();
  if (actuatorType != "STEPPERDRIVER")
    LOG_ERROR("Actuator type is only stepper driver!!!!");

  std::vector<int> pinVector = getIntList(getNode(),
                                          "./PINS",
                                          3);

  StepperDriver stepperDriver(pinVector);
  stepperDriver.setDefaultDirection(
      getNodeFromPath("./DEFAULT_DIRECTION").text().as_string());
  LOG_DEBUG("Default direction is: " << stepperDriver.getDefaultDirection());
  stepperDriver.setHoldMotor(getNodeFromPath("./HOLD_MOTOR").text().as_bool());
  LOG_DEBUG("Hold motor: " << stepperDriver.getHoldMotor());
  setStepperDriver(stepperDriver);
}


bool StepperDriverIO::update(const BaseMotor* i_stepperDriver) {
  bool hasSucceded(true);
  hasSucceded&=updatePins(i_stepperDriver->getCurrentPinState().getPinVector());
  getNodeFromPath("./DEFAULT_DIRECTION").text().set(
      i_stepperDriver->getDefaultDirection().c_str());

  getNodeFromPath("./HOLD_MOTOR").text().set(i_stepperDriver->getHoldMotor());
  return hasSucceded;
}


bool StepperDriverIO::updatePins(const PinVector& i_pinVector) {
  // Update pin numbers
  pugi::xml_node pinValueNode = getNodeFromPath("./PINS/VALUE");
  PinVector::const_iterator pinIterator = i_pinVector.begin();
  int i = 0;
  for (;
       pinValueNode;
       ++pinIterator, pinValueNode = pinValueNode.next_sibling(), i++) {
    LOG_DEBUG("Pin Value: " << pinValueNode.text().as_string()
              << " on position: " << i);
    if (pinIterator == i_pinVector.end()) {
      LOG_DEBUG("Number of pins in pin vector: " << i_pinVector.size());
      LOG_DEBUG("Number of pins seen in xml file: " << i);
      LOG_ERROR("Number of pins in the xml file is bigger" <<
                "then the pins defined in the StepperDriver");
    }
    pinValueNode.text().set(*pinIterator);
  }

  if (pinIterator != i_pinVector.end())
    LOG_ERROR("Number of pins defined in the StepperDriver" <<
              "is bigger then the xml file!");
  return true;
}


StepperDriverIO::StepperDriverIO(const pugi::xml_node& i_node)
    : XMLBuilder(i_node)
{}


