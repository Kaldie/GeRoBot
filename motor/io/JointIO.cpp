// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <TranslationalJoint.h>
#include <RotationalJoint.h>
#include <StepperDriver.h>
#include "./JointIO.h"
#include "./StepperDriverIO.h"


void JointIO::build() {
  LOG_DEBUG("Building a joint");
  if (!isValidNode(m_node) ) {
    LOG_ERROR("Not a valid joint node!");
  }
  makeSharedJoint();
  //Movement per step
  m_jointPointer->setMovementPerStep
    (getNodeFromPath(m_node, "./MOVEMENT_PER_STEP").text().as_double());
  //Position
  m_jointPointer->setPosition
    (getNodeFromPath(m_node, "./DEFAULT_POSITION").text().as_double());
  LOG_DEBUG("Default position: " << m_jointPointer->getPosition());
  // Correct gradians to radians
  if (m_jointPointer->getMovementType() == BaseJoint::Rotational) {
    m_jointPointer->setMovementPerStep
      (m_jointPointer->getMovementPerStep() * PI / 180);
    m_jointPointer->setPosition
      (m_jointPointer->getPosition() * PI / 180);
  }
  //StepperDriver
  *m_jointPointer->getMotor() =
    parseStepperDriver(getNodeFromPath(m_node, "./ACTUATOR"));
  //Range
  m_jointPointer->setRange(getDoubleList(m_node, "./RANGE", 2));
  handleConversionMap();
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
  if (i_jointPointer->getMovementType() == BaseJoint::Rotational)
    getNodeFromPath("./MOVEMENT_TYPE").text().set("ROTATIONAL");
  else if (i_jointPointer->getMovementType() == BaseJoint::Translational)
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
  DirectionConversionMap directionConversionMap
    (i_jointPointer->getDirectionConversionMap());

  pugi::xml_node directionConversionNode = getNodeFromPath
    ("./DIRECTION_CONVERSION_MAP/VALUE");

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
    if (std::string(attribute.name()) == "name") {
      continue;
    }
    if (std::string(attribute.name()) == "type" and
        std::string(attribute.value()) != "StepperDriver") {
      return false;
    }
  }
  return true;
}


void JointIO::makeSharedJoint() {
  m_jointPointer.reset();
  std::string movementType =
    getNodeFromPath(m_node, "./MOVEMENT_TYPE").text().as_string();
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


bool JointIO::createNode(pugi::xml_node* i_parent) {
  pugi::xml_node jointNode = i_parent->append_child("JOINT");
  jointNode.append_child("CHILD")
    .append_child(pugi::node_pcdata).set_value("null");
  jointNode.append_child("MOVEMENT_PER_STEP")
    .append_child(pugi::node_pcdata).set_value("0.1");
  jointNode.append_child("MOVEMENT_TYPE")
    .append_child(pugi::node_pcdata).set_value("ROTATIONAL");
  jointNode.append_child("DEFAULT_POSITION")
    .append_child(pugi::node_pcdata).set_value("90");
  pugi::xml_node tmp;
  // create range node
  tmp = jointNode.append_child("RANGE");
  tmp.append_child("VALUE")
    .append_child(pugi::node_pcdata).set_value("0");
  tmp.append_child("VALUE")
    .append_child(pugi::node_pcdata).set_value("360");
  // create direction conversion map
  tmp = jointNode.append_child("DIRECTION_CONVERSION_MAP");
  tmp.append_child("VALUE")
    .append_child(pugi::node_pcdata).set_value("CCW,CCW");
  tmp.append_child("VALUE")
    .append_child(pugi::node_pcdata).set_value("CW,CW");
  StepperDriverIO::createNode(&jointNode);
  return true;
}
