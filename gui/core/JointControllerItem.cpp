#include <macroHeader.h>
#include <JointController.h>
#include "JointControllerItem.h"
#include "BaseJointItem.h"
#include "BaseActuatorItem.h"
#include <BaseJoint.h>

JointControllerItem::JointControllerItem(BaseRobotItem* parent,
																				 JointController* jointController):
	BaseRobotItem("JointController",parent),
	m_jointController(jointController)
{
	LOG_DEBUG(getElementName().toStdString());
	setNumberOfProperties(5);
}


bool JointControllerItem::construct(){
	bool worked(true);
	std::vector<MovementType> movementTypes{Rotational,Translational};
	for(int outerIndex=0;
			outerIndex<2;
			outerIndex++){
		JointController::JointPointerVector baseJointVector=m_jointController->getJoints(movementTypes[outerIndex]);
		for(auto iter=baseJointVector.begin();
				iter!=baseJointVector.end();
				iter++){
			LOG_DEBUG("Insert a join child");
			LOG_DEBUG((*iter)->getPosition());

			BaseJointItem* child = new BaseJointItem(this,(*iter));
			worked&=child->construct();
			if(!worked)
				LOG_WARNING("Joint child was not constructed well!");
			insertChild(0,child);
		}
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
