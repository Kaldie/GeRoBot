// Copyright [2015] Ruud Cools

#include <macroHeader.h>
#include "JointControllerIO.h"
#include "JointIO.h"

void JointControllerIO::build() {
  LOG_DEBUG("Building a Joint controller!");
  LOG_DEBUG("Joint controller node: " <<
            std::string(getNode().name()));
  addJoints();
  m_jointController.setActuator(parseActuator(getNodeFromPath("./ACTUATOR")));
  LOG_DEBUG("Joint controller node: " <<
            std::string(getNode().name()));
}


void JointControllerIO::addJoints(){
  LOG_DEBUG("Adding Joints");
  // get all the joints from the xml file
  JointTupleVector jointTupleVector = buildAllJoints();
  // Resolve the dependancies
  resolveDependencies(jointTupleVector);
  // And finaly add them to tje joint controller
  for (const auto& jointTuple : jointTupleVector) {
    m_jointController.addJoint
      (std::get<1>(jointTuple));
  }
}


JointControllerIO::JointTupleVector JointControllerIO::buildAllJoints() const {
  BaseJoint::JointPointer jointPointer;
  JointTupleVector jointTupleVector;
  std::string identificationString;
  for (pugi::xml_node jointNode = getNodeFromPath("./JOINT");
      jointNode;
      jointNode = jointNode.next_sibling()) {
    LOG_DEBUG(jointNode.name());
    // if the sibling is named Joint
    if(std::string(jointNode.name()) == "JOINT") {
      LOG_DEBUG("adding a joint!!");
      JointIO jointIO(jointNode);
      jointIO.build();
      jointTupleVector.push_back
        (std::make_tuple(jointNode,                    // pugi::xml_node
                         jointIO.getJointPointer()));   // BaseJoint::JointPointer
    }
  }
  return jointTupleVector;
}


void JointControllerIO::resolveDependencies
(const JointTupleVector& i_jointTupleVector) const {
  BaseJoint::JointPointer jointPointer;
  std::string parentString;
  std::string childString;
  pugi::xml_node jointNode;
  for (const auto& jointTuple : i_jointTupleVector) {
    jointNode = std::get<0>(jointTuple);
    childString = getNodeFromPath(jointNode, "CHILD").text().as_string();
    jointPointer = std::get<1>(jointTuple);
    std::string decendentName;
    for (const auto& decendentTuple : i_jointTupleVector) {
      decendentName = getNodeFromPath(std::get<0>(decendentTuple),
                                      "IDENTIFICATION").text().as_string();
      if (decendentName == childString) {
        jointPointer->setChild
          (std::get<1>(decendentTuple));
        break;
      }
    }
  }
}


BaseJoint::JointPointer JointControllerIO::parseJoint(const pugi::xml_node& i_jointNode) {
  JointIO jointIO(i_jointNode);
  jointIO.build();
  return jointIO.getJointPointer();
}


ArduinoMotorDriver JointControllerIO::parseActuator(
    const pugi::xml_node& i_node) {
  if (std::string(getNodeFromPath(i_node, "./TYPE").text().as_string())
      != "Arduino")
    LOG_ERROR("None other arduino actuators are defined yet!");

  std::string serialExpression =
      getNodeFromPath(i_node,
                      "./REGULAR_EXPRESSION").text().as_string();
  LOG_DEBUG("Serial port regular expression is: " << serialExpression);
  ArduinoMotorDriver arduinoMotorDriver(serialExpression);
  return arduinoMotorDriver;
}


bool JointControllerIO::update(
    const JointController& i_jointController) {
  bool hasSucceeded(true);
  setJointController(i_jointController);
  hasSucceeded &= updateActuatorNode();
  hasSucceeded &= updateJointNodes();
  return hasSucceeded;
}


bool JointControllerIO::updateActuatorNode() {
  pugi::xml_node actuatorNode = getNodeFromPath("./ACTUATOR");

  getNodeFromPath(actuatorNode, "./REGULAR_EXPRESSION").text().
      set(m_jointController.getActuator().
          getSerialRegularExpresion().c_str());
  return true;
}


bool JointControllerIO::updateJointNodes() {
  bool hasSucceeded(true);
  JointController::JointPointerVector jointVector =
    m_jointController.getJointPointerVector();
  // current found joint pointer
  BaseJoint::JointPointer jointPointer;
  // current movement type
  BaseJoint::MovementType movementType;
  std::string movementString;
  for (pugi::xml_node jointNode = getNodeFromPath("./JOINT");
      jointNode;
      jointNode = jointNode.next_sibling()) {
    movementString = getNodeFromPath(jointNode, "./MOVEMENT_TYPE").text().as_string();
    if (movementString.compare(std::string("Rotational")) != 0) {
      movementType = BaseJoint::Rotational;
    } else if (movementString.compare("Translational") != 0) {
      movementType = BaseJoint::Translational;
    }
    for (auto& jointPointer : jointVector) {
      if (jointPointer->getMovementType() == movementType) {
        jointVector.erase(std::remove(jointVector.begin(), jointVector.end(), jointPointer));
        break;
      }
    }
    // check if the proper joint is found!
    if (jointPointer->getMovementType() != movementType) {
      LOG_ERROR("Could not find a joint with the proper movement type!");
    }
    // create an IO instance
    JointIO jointIO(jointNode);
    // try to update it
    hasSucceeded &= jointIO.update(jointPointer);
  }
  return hasSucceeded;
}


JointControllerIO::JointControllerIO(const pugi::xml_node& i_node)
    : XMLBuilder(i_node) {
  LOG_DEBUG("Creating a JointControllerIO with node: " << i_node.name());
}

