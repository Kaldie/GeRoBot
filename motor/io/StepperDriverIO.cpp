// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "./StepperDriverIO.h"
#include <StepperDriver.h>

void StepperDriverIO::build() {
  LOG_DEBUG("Buildiing a stepperdriver!");
  std::string actuatorType = getNode().first_child().text().as_string();
  if (actuatorType != "STEPPERDRIVER") {
    LOG_ERROR("Actuator type is only stepper driver!!!!");
  }
  std::vector<int> pinVector = getIntList(m_node, "./PINS", 3);
  m_stepperDriver.reset(new StepperDriver(pinVector));
  m_stepperDriver->setDefaultDirection(
      getNodeFromPath("./DEFAULT_DIRECTION").text().as_string());
  LOG_DEBUG("Default direction is: " << m_stepperDriver->getDefaultDirection());
  m_stepperDriver->setHoldMotor(getNodeFromPath("./HOLD_MOTOR").text().as_bool());
  LOG_DEBUG("Hold motor: " << m_stepperDriver->getHoldMotor());
  m_stepperDriver->setMaxSpeed(getNodeFromPath("./MAX_SPEED").text().as_int());
  LOG_DEBUG("Max speed is: " << m_stepperDriver->getMaxSpeed());
  m_stepperDriver->setPullIn(getNodeFromPath("./PULL_IN").text().as_int());
  m_stepperDriver->setPullOut(getNodeFromPath("./PULL_OUT").text().as_int());
  m_stepperDriver->setIncremental(getNodeFromPath("./INCREMENT").text().as_int());
}


bool StepperDriverIO::update(const BaseMotor::MotorPointer i_motor) {
  bool hasSucceded(true);
  hasSucceded&=updatePins(i_motor->getCurrentPinState().getPinVector());
  getNodeFromPath("./DEFAULT_DIRECTION").text().set
    (i_motor->getDefaultDirection().c_str());
  getNodeFromPath("./HOLD_MOTOR").text().set(i_motor->getHoldMotor());
  // update stepperdriver specific stuff
  const StepperDriver::DriverPointer driver =
    std::static_pointer_cast<StepperDriver>(i_motor);
  
  getNodeFromPath("./MAX_SPEED").text().set(driver->getMaxSpeed());
  getNodeFromPath("./PULL_IN").text().set(driver->getPullIn());
  getNodeFromPath("./PULL_OUT").text().set(driver->getPullOut());
  getNodeFromPath("./INCREMENT").text().set(driver->getIncremental());
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


bool StepperDriverIO::createNode(pugi::xml_node *i_parent) {
  pugi::xml_node stepper = i_parent->append_child("ACTUATOR");
  stepper.append_child("ACTUATOR_TYPE").
    append_child(pugi::node_pcdata).set_value("STEPPERDRIVER");
  pugi::xml_node tmp;
  tmp = stepper.append_child("PINS");
  tmp.append_child("VALUE").append_child(pugi::node_pcdata).set_value("5");
  tmp.append_child("VALUE").append_child(pugi::node_pcdata).set_value("6");
  tmp.append_child("VALUE").append_child(pugi::node_pcdata).set_value("7");
  stepper.append_child("DEFAULT_DIRECTION").
    append_child(pugi::node_pcdata).set_value("CCW");
  stepper.append_child("HOLD_MOTOR").
    append_child(pugi::node_pcdata).set_value("true");
  stepper.append_child("MAX_SPEED").
    append_child(pugi::node_pcdata).set_value("300");
  stepper.append_child("PULL_IN").
    append_child(pugi::node_pcdata).set_value("190");
  stepper.append_child("PULL_OUT").
    append_child(pugi::node_pcdata).set_value("190");
    stepper.append_child("INCREMENTAL").
    append_child(pugi::node_pcdata).set_value("10");
  return true;
}


StepperDriverIO::StepperDriverIO(const pugi::xml_node& i_node)
    : XMLBuilder(i_node)
{}


