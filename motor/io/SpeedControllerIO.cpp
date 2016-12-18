// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <SpeedControllerIO.h>
#include <RelativeSpeedController.h>
#include <ConstantSpeedController.h>
#include <PrescribedSpeedController.h>

void SpeedControllerIO::build() {
  LOG_DEBUG("Building a SpeedController");
  resolveSpeedControllerType();
  m_speedController->setRobotSpeed(getNodeFromPath("./SPEED").text().as_double());
}


void SpeedControllerIO::resolveSpeedControllerType() {
  std::string type = getNodeFromPath("./TYPE").text().as_string();
  if (type.compare("Relative") == 0) {
    LOG_DEBUG("Buidling a relative speed controller!");
    m_speedController.reset(new RelativeSpeedController());
  } else if (type.compare("Constant") == 0) {
    LOG_DEBUG("Building a constant speed controller!");
    m_speedController.reset(new ConstantSpeedController());
  } else if (type.compare("Prescribed") == 0) {
    LOG_DEBUG("Building a prescribed speed controller");
    m_speedController.reset(new PrescribedSpeedController());
  }
}


SpeedControllerIO::SpeedControllerIO(const pugi::xml_node& i_node) {
  setNode(i_node);
}


bool SpeedControllerIO::update(const SpeedController::SpeedControllerPointer& i_pointer) {
  SpeedController::Type type = i_pointer->getType();
  if (type == SpeedController::Prescribed) {
    getNodeFromPath("./TYPE").text().set("Prescribed");
  } else if (type == SpeedController::Relative) {
    getNodeFromPath("./TYPE").text().set("Relative");
  } else if (type ==SpeedController::Constant) {
    getNodeFromPath("./TYPE").text().set("Constant");
  } else {
    LOG_ERROR("Unknown Speed controller type!");
  }
  getNodeFromPath("./SPEED").text().set(i_pointer->getRobotSpeed());
  return true;
}


bool SpeedControllerIO::createNode(pugi::xml_node* i_parent) {
  pugi::xml_node controllerNode = i_parent->append_child("SPEEDCONTROLLER");
  controllerNode.append_child("TYPE")
    .append_child(pugi::node_pcdata).set_value("Constant");
  controllerNode.append_child("SPEED")
    .append_child(pugi::node_pcdata).set_value("5");
  return true;
}
