// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <TranslationalJoint.h>
#include <RotationalJoint.h>
#include <StepperDriver.h>
#include "./JointIO.h"
#include "./StepperDriverIO.h"


void JointIO::build() {
  LOG_DEBUG("Building a joint");
  pugi::xml_node jointNode = getNode();

  for (pugi::xml_attribute attribute = jointNode.first_attribute();
      attribute;
      attribute = attribute.next_attribute()) {
    if (std::string(attribute.name()) == "type" and
        std::string(attribute.value()) == "StepperDriver") {
      LOG_DEBUG("Found 1");

      JointPointer jointPointer;

      std::string movementType =
          getNodeFromPath(jointNode, "./MOVEMENT_TYPE").text().as_string();

      if (movementType == "ROTATIONAL") {
        LOG_DEBUG("Building a rotational joint!");
        RotationalJoint<StepperDriver> joint;
        jointPointer = joint.clone();
      } else if (movementType == "TRANSLATIONAL") {
        LOG_DEBUG("Building a translational joint!");
        TranslationalJoint<StepperDriver> joint;
        jointPointer = joint.clone();
      } else {
        LOG_ERROR("Movement type: " << movementType << " is not correct!");
      }
      jointPointer->setMovementPerStep(
          getNodeFromPath(jointNode, "./MOVEMENT_PER_STEP").text().as_double());
      jointPointer->setPosition(
          getNodeFromPath(jointNode, "./DEFAULT_POSITION").text().as_double());

      LOG_DEBUG("Joint type: " << jointPointer->getMovementType());
      LOG_DEBUG("Default position: " << jointPointer->getPosition());

      *(jointPointer->getMotor()) =
          parseStepperDriver(getNodeFromPath(jointNode, "./ACTUATOR"));

      std::vector<double> range = getDoubleList(jointNode, "./RANGE", 2);
      jointPointer->setRange(range);
      std::vector<std::string> directionConversionStringVector
          = getStringList(jointNode,
                          "./DIRECTION_CONVERSION_MAP",
                          2);
      DirectionConversionMap directionMap;
      int delimPosition;
      for (auto itr = directionConversionStringVector.begin();
          itr != directionConversionStringVector.end();
          itr++) {
        delimPosition=(*itr).find_first_of(",");
        LOG_DEBUG("Direction " << (*itr).substr(0, delimPosition)<<
                  " will be converted to " << (*itr).substr(delimPosition+1));
        directionMap[(*itr).substr(0, delimPosition)] =
            (*itr).substr(delimPosition+1);
      }

      jointPointer->setDirectionConversionMap(directionMap);

      setJointPointer(jointPointer);
      LOG_DEBUG("Construction of the pointer is finished!");
    }
    LOG_DEBUG("Joint Build is finshed!");
  }
}

StepperDriver JointIO::parseStepperDriver(const pugi::xml_node& i_node) {
  StepperDriverIO stepperDriverIO(i_node);
  stepperDriverIO.build();
  return stepperDriverIO.getStepperDriver();
}


JointIO::JointIO(const pugi::xml_node& i_node) {
  setNode(i_node);
}


bool JointIO::update(const JointPointer& i_jointPointer) {
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
