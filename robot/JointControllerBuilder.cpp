
#include "JointControllerBuilder.h"
#include "JointBuilder.h"
/*
#include <BaseJoint.h>
#include <ArduinoMotorDriver.h>
*/

void JointControllerBuilder::build()
{
  const pugi::xml_node x=getNode();
  LOG_DEBUG("Building a Joint controller!");
  LOG_DEBUG("Joint controller node: "<<std::string(x.name()));
  LOG_DEBUG("Magic offset:"<<x.offset_debug());

  addJoints(getNode().first_element_by_path("./JOINT"));
  m_jointController.setActuator(parseActuator(getNode().first_element_by_path("./ACTUATOR")));

  

  LOG_DEBUG("Magic offset:"<<x.offset_debug());
  LOG_DEBUG("Joint controller node: "<<std::string(x.parent().name()));
  
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
  std::string serialExpression=i_node.first_element_by_path("./REGULAR_EXPRESSION").text().as_string();
  LOG_DEBUG("Serial port regular expression is: "<<serialExpression);
  bool hasReducedSpeed=i_node.first_element_by_path("./REDUCED_SPEED").text().as_bool();
  ArduinoMotorDriver arduinoMotorDriver(serialExpression);
  arduinoMotorDriver.setReducedSpeed(hasReducedSpeed);
  return arduinoMotorDriver;
}


JointControllerBuilder::JointControllerBuilder(const pugi::xml_node& i_node):
  XMLBuilder(i_node)
{
  LOG_DEBUG("Creating a JointControllerBuilder with node: "<<i_node.name());
}
