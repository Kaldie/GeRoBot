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
  for (pugi::xml_node jointNode = getNodeFromPath("./JOINT");
       jointNode;
       jointNode = jointNode.next_sibling("JOINT")) {
    JointIO jointIO(jointNode);
    jointIO.build();
    jointTupleVector.push_back
      (std::make_tuple(jointNode, jointIO.getJointPointer()));
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
    if (childString == "null"){
      continue;
    }
    std::string decendentName;
    for (const auto& decendentTuple : i_jointTupleVector) {
      decendentName = std::get<0>(decendentTuple).attribute("name").as_string();
      if (decendentName == childString) {
        std::get<1>(jointTuple)->setChild
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
  JointTupleVector vector = updateJointNodes();
  hasSucceeded &= updateDependancies(vector);
  return hasSucceeded;
}


bool JointControllerIO::updateActuatorNode() {
  pugi::xml_node actuatorNode = getNodeFromPath("./ACTUATOR");
  getNodeFromPath(actuatorNode, "./REGULAR_EXPRESSION").text().
      set(m_jointController.getActuator().
          getSerialRegularExpresion().c_str());
  return true;
}


JointControllerIO::JointTupleVector JointControllerIO::updateJointNodes() {
  updateNumberOfJointNode();
  pugi::xml_node jointNode = getNodeFromPath("JOINT");
  JointController::JointPointerVector vector 
  =m_jointController.getJointPointerVector();
  JointTupleVector jointTupleVector;
  unsigned int jointNumber(1);
  for(auto jointItr = vector.begin();
      jointItr != vector.end();
      ++jointItr) {
    JointIO jointIO(jointNode);
    jointIO.update(*jointItr);
    jointTupleVector.push_back
      (std::make_tuple(jointIO.getNode(),*jointItr));
    // set the name for the joint
    jointNode.attribute("name").set_value
      (("Joint " + std::to_string(jointNumber)).c_str());
    ++jointNumber;
    // if the current joint is not the last
    if (jointItr + 1 != vector.end()) {
    // get a new joint node to update
      getNextJointNode(&jointNode);
    }
  }
  return jointTupleVector;
}


void JointControllerIO::updateNumberOfJointNode() {
  unsigned int numberOfJoints = 
    m_jointController.getJointPointerVector().size();
  unsigned int numberOfXMLJoints = 0;
  for (pugi::xml_node node = m_node.child("JOINT");
       node;
       node = node.next_sibling("JOINT")) {
      ++numberOfXMLJoints;
      LOG_DEBUG(node.name());
  }
  LOG_DEBUG("Number of Joints: " << numberOfJoints);
  LOG_DEBUG("Number of JointNode: " << numberOfXMLJoints);
  // No update necesary, they are of equal length
  if (numberOfJoints == numberOfXMLJoints) {
    return;
  } 
  // remove XML nodes
  if (numberOfJoints < numberOfXMLJoints) {
    while (numberOfJoints < numberOfXMLJoints) {
      if (removeLastJointNode()) {
	--numberOfXMLJoints;
      } else {
	LOG_ERROR("Could not remove Joint node!");
      }
    }
    return;
  } 
  // Add xml nodes
  if (numberOfJoints > numberOfXMLJoints) {
    while (numberOfJoints > numberOfXMLJoints) {
      if (!addJointNode()) {
	LOG_ERROR("Creation of node went wrong!");
      }
      ++numberOfXMLJoints;
    }
    return;
  }
}


bool JointControllerIO::removeLastJointNode() {
  for (pugi::xml_node jointNode = m_node.last_child();
       jointNode;
       jointNode = jointNode.previous_sibling()) {
    if (strcmp(jointNode.name(),"JOINT") == 0) {
      m_node.remove_child(jointNode);
      return true;
    }
  }
  return false;
}


bool JointControllerIO::updateDependancies
(const JointControllerIO::JointTupleVector& i_vector) {
  BaseJoint::JointPointer childJoint;
  LOG_DEBUG("Update Dependancies!");
  for (const auto& tuple : i_vector) {
    LOG_DEBUG("Check a joint");
    childJoint = std::get<1>(tuple)->getChild();
    if (childJoint) {
      LOG_DEBUG("Has a child!");
      // this joint has a child
      for (const auto& tupleWithChild : i_vector) {
	if (std::get<1>(tupleWithChild) == childJoint) {
	  // fond the corrisponding child
	  std::get<0>(tuple).child("CHILD").text() =
	    (std::get<0>(tupleWithChild).attribute("name").value());
	  break;
	}
      }
    }
  }
  return true;
}


bool JointControllerIO::addJointNode() {
  if (JointIO::createNode(&m_node)) {
    return true;
  } else {
    return false;
  }
}


JointControllerIO::JointControllerIO(const pugi::xml_node& i_node)
    : XMLBuilder(i_node) {
  LOG_DEBUG("Creating a JointControllerIO with node: " << i_node.name());
}


void JointControllerIO::getNextJointNode(pugi::xml_node* i_node) const {
  pugi::xml_node newNode = i_node->next_sibling("JOINT");
  if (!newNode){
   LOG_ERROR("Expected an Joint node, did not find any!");
  }
  *i_node = newNode;
}
