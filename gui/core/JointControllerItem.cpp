#include <macroHeader.h>
#include <JointController.h>
#include "JointControllerItem.h"
#include "BaseJointItem.h"
#include "BaseActuatorItem.h"
#include <BaseJoint.h>

JointControllerItem::JointControllerItem
(BaseRobotItem* parent,
 const JointController::JointControllerPointer& jointController)
  : BaseRobotItem("JointController",parent),
    m_jointController(jointController) {
  LOG_DEBUG(getElementName().toStdString());
  setNumberOfProperties(5);
    }


bool JointControllerItem::construct() {
  bool worked(true);
  for(auto& jointPointer : m_jointController->getJointPointerVector()) {
    LOG_DEBUG("Insert a join child");
    LOG_DEBUG(jointPointer->getPosition());
    BaseJointItem* child = new BaseJointItem(this,(jointPointer));
    worked&=child->construct();
    if(!worked)
      LOG_WARNING("Joint child was not constructed well!");
    insertChild(0,child);
  }
  return worked && addArduinoActuator();
}


bool JointControllerItem::addArduinoActuator(){
  if(!m_jointController)
    return false;

  LOG_DEBUG("Add Arduino Actuator");
  BaseActuatorItem* child = new BaseActuatorItem(this,m_jointController->getActuatorPointer());
  child->construct();
  insertChild(0,child);
  return true;
}
