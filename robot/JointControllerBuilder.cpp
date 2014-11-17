#include <macroHeader.h>
#include "JointControllerBuilder.h"
#include "JointBuilder.h"

void JointControllerBuilder::build()
{
  LOG_DEBUG("Building a Joint controller!");
  LOG_DEBUG("Joint controller node: "<<std::string(getNode().name()));

  addJoints();
  m_jointController.setActuator(parseActuator(getNodeFromPath("./ACTUATOR")));

  LOG_DEBUG("Joint controller node: "<<std::string(getNode().name()));
}


void JointControllerBuilder::addJoints(){
  //Adding all the joints that 
  LOG_DEBUG("Adding Joints");

  JointPointer pointer;
  for(pugi::xml_node jointNode=getNodeFromPath("./JOINT");
      jointNode;
      jointNode=jointNode.next_sibling()){
    LOG_DEBUG(jointNode.name());

    // if the sibling is named Joint, we are in bissnuss
    if(std::string(jointNode.name())=="JOINT"){
      LOG_DEBUG("adding a joint!!");
      parseJoint(jointNode);
    }
  }
}


void JointControllerBuilder::parseJoint(const pugi::xml_node& i_jointNode){
  
  JointBuilder jointBuilder(i_jointNode);
  jointBuilder.build();
  getJointController().addJoint(jointBuilder.getJointPointer());
}


ArduinoMotorDriver JointControllerBuilder::parseActuator(const pugi::xml_node& i_node) {
  if (std::string(getNodeFromPath(i_node, "./TYPE").text().as_string())
      != "Arduino")
    LOG_ERROR("None other arduino actuators are defined yet!");

  std::string serialExpression = getNodeFromPath(i_node,
                                               "./REGULAR_EXPRESSION").text().as_string();
  LOG_DEBUG("Serial port regular expression is: " << serialExpression);
  bool hasReducedSpeed = getNodeFromPath(i_node,"./REDUCED_SPEED").text().as_bool();
  ArduinoMotorDriver arduinoMotorDriver(serialExpression);
  arduinoMotorDriver.setReducedSpeed(hasReducedSpeed);
  return arduinoMotorDriver;
}


bool JointControllerBuilder::update(const JointController& i_jointController){
	bool hasSucceeded(true);
	setJointController(i_jointController);
 	hasSucceeded&=updateActuatorNode();
	hasSucceeded&=updateJointNodes();
	return hasSucceeded;
}


bool JointControllerBuilder::updateActuatorNode(){
	pugi::xml_node actuatorNode = getNodeFromPath("./ACTUATOR");

	getNodeFromPath(actuatorNode,"./REGULAR_EXPRESSION").text().
		set(getJointController().getActuator().getSerialRegularExpresion().c_str());

	getNodeFromPath(actuatorNode,"./REDUCED_SPEED").text().
		set(getJointController().getActuator().getReducedSpeed());
	return true;
}


bool JointControllerBuilder::updateJointNodes(){
	
	bool hasSucceeded(true);
	JointPointerVector rotationalJointVector=m_jointController.getJoints(Rotational);
	JointPointerVector translationalJointVector=m_jointController.getJoints(Translational);
	JointPointer jointPointer;
	
	//remove all joints first
  for(pugi::xml_node jointNode=getNodeFromPath("./JOINT");
      jointNode;
      jointNode=jointNode.next_sibling()){
		
		std::string movementType=getNodeFromPath(jointNode,"./MOVEMENT_TYPE").text().as_string();
		
		if(movementType=="ROTATIONAL"){
			if(rotationalJointVector.size()<1)
				LOG_ERROR("Number of rotational joint in the previous version does not corrispond to the new number!");
			jointPointer=rotationalJointVector.back();
			rotationalJointVector.pop_back();
		}	

		else if(movementType=="TRANSLATIONAL"){
			if(translationalJointVector.size()<1)
				LOG_ERROR("Number of translational joint in the previous version does not corrispond to the new number!");
			jointPointer=translationalJointVector.back();
			translationalJointVector.pop_back();
		}	
		
	
		JointBuilder jointBuilder(jointNode);
		hasSucceeded&=jointBuilder.update(jointPointer);		
	}
	
	return hasSucceeded;
	
}


JointControllerBuilder::JointControllerBuilder(const pugi::xml_node& i_node):
  XMLBuilder(i_node){
	LOG_DEBUG("Creating a JointControllerBuilder with node: "<<i_node.name());
}

