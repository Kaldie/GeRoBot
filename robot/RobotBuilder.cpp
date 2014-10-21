#include "RobotBuilder.h"
#include "JointControllerBuilder.h"

RobotBuilder::RobotBuilder(const std::string& i_fileName):
  XMLBuilder(i_fileName),
  m_robot()  
{}


void RobotBuilder::build(){
  //need to call the xml builder build function to load the root node of the document given by the file name.
  XMLBuilder::build();
  LOG_DEBUG("Building a robot from file name: "<<getFileName());
  LOG_DEBUG("Root node is: "<<getNode().name());
  
  m_robot.getJointController()=parseJointController(getNodeFromPath("./ROBOT/JOINTCONTROLLER"));
}

    
JointController RobotBuilder::parseJointController(const pugi::xml_node& i_node){
  JointControllerBuilder jointControllerBuilder(i_node);
  jointControllerBuilder.build();  
  return jointControllerBuilder.getJointController();
}


void RobotBuilder::displayTree(){
  LOG_DEBUG("Finding root node!");
  pugi::xml_node node=getNode();
  LOG_DEBUG("Root node found!");
  
  if(node.first_child())
    LOG_DEBUG("First child found: "<<node.first_child().name());
  else
    LOG_DEBUG("No first child found!");


  for (pugi::xml_node tool =node.first_child(); tool; tool=tool.first_child()){
    LOG_DEBUG("Node info: ");
    LOG_DEBUG(tool.name());
    LOG_DEBUG(tool.child_value());
    //std::cout<<tool.path()<<std::endl;
    }
}
