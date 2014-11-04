#include <macroHeader.h>
#include "JointControllerBuilder.h"
#include "JointBuilder.h"
/*
#include <BaseJoint.h>
#include <ArduinoMotorDriver.h>
*/

void JointControllerBuilder::build()
{
  const pugi::xml_node JointControllerNode=getNode();
  LOG_DEBUG("Building a Joint controller!");
  LOG_DEBUG("Joint controller node: "<<std::string(JointControllerNode.name()));
  LOG_DEBUG("Magic offset:"<<JointControllerNode.offset_debug());

  addJoints(getNodeFromPath("./JOINT"));
  m_jointController.setActuator(parseActuator(getNodeFromPath("./ACTUATOR")));

  

  LOG_DEBUG("Magic offset:"<<JointControllerNode.offset_debug());
  LOG_DEBUG("Joint controller node: "<<std::string(JointControllerNode.name()));
  
}


void JointControllerBuilder::addJoints(const pugi::xml_node& i_firstJointNode){
  //Adding all the joints that 
  LOG_DEBUG("Adding Joints");

  JointPointer pointer;
  for(pugi::xml_node jointNode=i_firstJointNode;
      jointNode;
      jointNode=jointNode.next_sibling())
    {
      LOG_DEBUG(jointNode.name());

      //if the sibling is named Joint, we are in bissnuss
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


ArduinoMotorDriver JointControllerBuilder::parseActuator(const pugi::xml_node& i_node)
{
  std::string serialExpression=getNodeFromPath(i_node,"./REGULAR_EXPRESSION").text().as_string();
  LOG_DEBUG("Serial port regular expression is: "<<serialExpression);
  bool hasReducedSpeed=getNodeFromPath(i_node,"./REDUCED_SPEED").text().as_bool();
  ArduinoMotorDriver arduinoMotorDriver(serialExpression);
  arduinoMotorDriver.setReducedSpeed(hasReducedSpeed);
  return arduinoMotorDriver;
}


JointControllerBuilder::JointControllerBuilder(const pugi::xml_node& i_node):
  XMLBuilder(i_node)
{
  LOG_DEBUG("Creating a JointControllerBuilder with node: "<<i_node.name());
}
