// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <RobotIO.h>
#include <JointControllerIO.h>
#include <Robot.h>
#include <JointController.h>
#include <SpeedControllerIO.h>
#include <SpeedController.h>


RobotIO::RobotIO(const std::string& i_fileName)
    : XMLBuilder(i_fileName) {
  m_robotPointer.reset(new Robot);
  XMLBuilder::build();
}


void RobotIO::build() {
  // need to call the xml builder build function to load the root node of the document given by the file name.
  LOG_DEBUG("Building a robot from file name: " << getFileName());
  LOG_DEBUG("Root node is: " << getNode().name());
  m_robotPointer->setJointController
    (parseJointController(getNodeFromPath("./ROBOT/JOINTCONTROLLER")));
  m_robotPointer->setSpeedController
    (parseSpeedController(getNodeFromPath("./ROBOT/SPEEDCONTROLLER")));
}


Robot::RobotPointer RobotIO::buildRobot() {
  build();
  return m_robotPointer;
}


JointController::JointControllerPointer RobotIO::parseJointController(
    const pugi::xml_node& i_node) {
  JointControllerIO jointControllerIO(i_node);
  jointControllerIO.build();
  return jointControllerIO.getJointController();
}


SpeedController::SpeedControllerPointer RobotIO::parseSpeedController
( const pugi::xml_node& i_node) {
  SpeedControllerIO speedController(i_node);
  speedController.build();
  return speedController.getSpeedController();
}


void RobotIO::displayTree() {
  LOG_DEBUG("Finding root node!");
  pugi::xml_node node = getNode();
  LOG_DEBUG("Root node found!");

  if (node.first_child())
    LOG_DEBUG("First child found: " << node.first_child().name());
  else
    LOG_DEBUG("No first child found!");

  for (pugi::xml_node tool = node.first_child();
       tool;
       tool = tool.first_child()) {
    LOG_DEBUG("Node info: ");
    LOG_DEBUG(tool.name());
    LOG_DEBUG(tool.child_value());
  }
}


void RobotIO::setRobotPointer(Robot::RobotPointer* i_robotPointer) {
  m_robotPointer.swap(*i_robotPointer);
}


bool RobotIO::update(const Robot::RobotPointer& i_robotPointer) {
  bool hasUpdated(true);
  m_robotPointer = i_robotPointer;
  hasUpdated &= updateJointController(m_robotPointer->getJointController());
  hasUpdated &= updateSpeedController(m_robotPointer->getSpeedController());
  return hasUpdated;
}


bool RobotIO::updateJointController
(const JointController::JointControllerPointer& i_jointController) {
  JointControllerIO jointControllerIO
    (getNodeFromPath("./ROBOT/JOINTCONTROLLER"));
  return jointControllerIO.update(m_robotPointer->getJointController());
}


bool RobotIO::updateSpeedController
(const SpeedController::SpeedControllerPointer& i_jointController) {
  SpeedControllerIO speedControllerIO
    (getNodeFromPath("./ROBOT/SPEEDCONTROLLER"));
  return speedControllerIO.update(m_robotPointer->getSpeedController());
}


bool RobotIO::store(const std::string& i_fileName) {
  // check if this one is the owner of the document tree!
  if (!getHasLoaded())
    return false;
  return XMLBuilder::store(i_fileName.c_str());
}
    
