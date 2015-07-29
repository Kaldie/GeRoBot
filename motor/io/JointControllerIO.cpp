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
  // Adding all the joints that
  LOG_DEBUG("Adding Joints");

  JointPointer pointer;
  for (pugi::xml_node jointNode = getNodeFromPath("./JOINT");
      jointNode;
      jointNode = jointNode.next_sibling()) {
    LOG_DEBUG(jointNode.name());

    // if the sibling is named Joint, we are in bissnuss
    if(std::string(jointNode.name()) == "JOINT") {
      LOG_DEBUG("adding a joint!!");
      parseJoint(jointNode);
    }
  }
}


void JointControllerIO::parseJoint(const pugi::xml_node& i_jointNode) {
  JointIO jointIO(i_jointNode);
  jointIO.build();
  getJointController().addJoint(jointIO.getJointPointer());
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
  bool hasReducedSpeed =
      getNodeFromPath(i_node, "./REDUCED_SPEED").text().as_bool();
  ArduinoMotorDriver arduinoMotorDriver(serialExpression);
  //  arduinoMotorDriver.setReducedSpeed(hasReducedSpeed);
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
      set(getJointController().
          getActuator().getSerialRegularExpresion().c_str());

  //  getNodeFromPath(actuatorNode, "./REDUCED_SPEED").text().
  //   set(getJointController().getActuator().getReducedSpeed());
  return true;
}


bool JointControllerIO::updateJointNodes() {
  bool hasSucceeded(true);
  JointPointerVector rotationalJointVector =
      m_jointController.getJoints(Rotational);

  JointPointerVector translationalJointVector =
      m_jointController.getJoints(Translational);
  JointPointer jointPointer;

  // remove all joints first
  for (pugi::xml_node jointNode = getNodeFromPath("./JOINT");
      jointNode;
      jointNode = jointNode.next_sibling()) {
    std::string movementType =
        getNodeFromPath(jointNode, "./MOVEMENT_TYPE").text().as_string();

    if (movementType == "ROTATIONAL") {
      if (rotationalJointVector.size() < 1)
        LOG_ERROR("Number of rotational joint in the previous" <<
                  "version does not corrispond to the new number!");
      jointPointer = rotationalJointVector.back();
      rotationalJointVector.pop_back();
    } else if (movementType == "TRANSLATIONAL") {
      if (translationalJointVector.size() < 1)
        LOG_ERROR("Number of translational joint in the" <<
                  "previous version does not corrispond to the new number!");
      jointPointer = translationalJointVector.back();
      translationalJointVector.pop_back();
    }


    JointIO jointIO(jointNode);
    hasSucceeded &= jointIO.update(jointPointer);
  }
  return hasSucceeded;
}


JointControllerIO::JointControllerIO(const pugi::xml_node& i_node)
    : XMLBuilder(i_node) {
  LOG_DEBUG("Creating a JointControllerIO with node: " << i_node.name());
}

