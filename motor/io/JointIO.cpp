// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <TranslationalJoint.h>
#include <RotationalJoint.h>
#include <StepperDriver.h>
#include "./JointIO.h"
#include "./StepperDriverIO.h"


void JointIO::build() {
  LOG_DEBUG("Building a joint");
  if (!isValidNode(m_node)) {
    LOG_ERROR("Not a valid joint node!");
  }
  makeSharedJoint();
  //Movement per step
  m_jointPointer->setMovementPerStep
    (getNodeFromPath(jointNode, "./MOVEMENT_PER_STEP").text().as_double());
  //Position
  m_jointPointer->setPosition
    (getNodeFromPath(m_node, "./DEFAULT_POSITION").text().as_double());
  LOG_DEBUG("Default position: " << m_jointPointer->getPosition());
  //StepperDriver
  *(m_jointPointer->getMotor()) =
    parseStepperDriver(getNodeFromPath(m_node, "./ACTUATOR"));
  //Range
  m_jointPointer->setRange(getDoubleList(m_node, "./RANGE", 2));
  handleDirectionMap();
  LOG_DEBUG("Construction of the pointer is finished!");
  LOG_DEBUG("Joint Build is finshed!");
}


StepperDriver JointIO::parseStepperDriver(const pugi::xml_node& i_node) {
  StepperDriverIO stepperDriverIO(i_node);
  stepperDriverIO.build();
  return stepperDriverIO.getStepperDriver();
}


JointIO::JointIO(const pugi::xml_node& i_node) {
  setNode(i_node);
}


bool JointIO::update(const BaseJoint::JointPointer& i_jointPointer) {
  if (i_jointPointer->getMovementType() == Rotational)
    getNodeFromPath("./MOVEMENT_TYPE").text().set("ROTATIONAL");
  else if (i_jointPointer->getMovementType() == Translational)
    getNodeFromPath("./MOVEMENT_TYPE").text().set("TRANSLATIONAL");
  else
    LOG_ERROR("Unknown movement type!");

  // Set movement per set
  getNodeFromPath("./MOVEMENT_PER_STEP")
    .text().set(i_jointPointer->getMovementPerStep());
  // Set current position
  getNodeFromPath("./DEFAULT_POSITION")
    .text().set(i_jointPointer->getPosition());

  // Set range to node
  pugi::xml_node rangeNode(getNodeFromPath("./RANGE/VALUE"));
  rangeNode.text().set(i_jointPointer->getRange()[0]);
  rangeNode.next_sibling().text().set(i_jointPointer->getRange()[1]);

  // Direction conversion
  DirectionConversionMap directionConversionMap(
                                                i_jointPointer->getDirectionConversionMap());

  pugi::xml_node directionConversionNode = getNodeFromPath(
                                                           "./DIRECTION_CONVERSION_MAP/VALUE");

  for (auto itr = directionConversionMap.begin();
       itr != directionConversionMap.end();
       itr++, directionConversionNode = directionConversionNode.next_sibling()) {
    directionConversionNode.text().set((itr->first+","+itr->second).c_str());
  }

  StepperDriverIO stepperDriverIO(getNodeFromPath("./ACTUATOR"));
  bool hasSucceeded(true);
  hasSucceeded&=stepperDriverIO.update(i_jointPointer->getMotor());

  return hasSucceeded;
}


bool JointIO::isValidNode(const pugi::xml_node& i_node) const {
  for (pugi::xml_attribute attribute = i_node.first_attribute();
       attribute;
       attribute = attribute.next_attribute()) {
    if (std::string(attribute.name()) != "type" and
        std::string(attribute.value()) != "StepperDriver") {
      return false;
    }
  }
  return true;
}


void JointIO::makeSharedJoint() {
  std::string movementType =
    getNodeFromPath(jointNode, "./MOVEMENT_TYPE").text().as_string();
  if (movementType == "ROTATIONAL") {
    LOG_DEBUG("Building a rotational joint!");
    m_jointPointer = std::make_shared<RotationalJoint<StepperDriver>>();
  } else if (movementType == "TRANSLATIONAL") {
    LOG_DEBUG("Building a translational joint!");
    m_jointPointer = std::make_shared<TranslationalJoint<StepperDriver>>();
  } else {
    LOG_ERROR("Movement type: " << movementType << " is not correct!");
  }
  LOG_DEBUG("Joint type: " << m_jointPointer->getMovementType());
}


void JointIO::handleConversionMap() {
  std::vector<std::string> directionConversionStringVector
    (getStringList(m_node, "./DIRECTION_CONVERSION_MAP", 2));
  DirectionConversionMap directionMap;
  int delimPosition;
  for (auto itr = directionConversionStringVector.begin();
       itr != directionConversionStringVector.end();
       itr++) {
    delimPosition=(*itr).find_first_of(",");
    LOG_DEBUG("Direction " << (*itr).substr(0, delimPosition));
    LOG_DEBUG(" will be converted to " << (*itr).substr(delimPosition+1));
    directionMap[(*itr).substr(0, delimPosition)] =
      (*itr).substr(delimPosition+1);
  }
  m_jointPointer->setDirectionConversionMap(directionMap);
}
