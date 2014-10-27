#include <JointController.h>
#include "JointControllerItem.h"
#include "BaseJointItem.h"

JointControllerItem::JointControllerItem(BaseRobotItem* parent,
																				 JointController* jointController):
	BaseRobotItem("JointController",parent),
	m_jointController(jointController)
{
	setNumberOfProperties(5);
}


QVariant JointControllerItem::data(int i_row,
														 int i_column) const {
	if(i_row==static_cast<int>(JointControllerProperty::ElementName))
		return BaseRobotItem::data(i_row,i_column);
	
	else
		return QVariant();
	
}


bool JointControllerItem::setData(int i_row,
																	int i_column,
																	const QVariant& i_value){
	if(!m_jointController)
		return false;

	if(i_row==static_cast<int>(JointControllerProperty::ElementName))
		return BaseRobotItem::setData(i_row,i_column,i_value);

	else
		return false;
}

bool JointControllerItem::construct(){
	bool worked(true);
	std::vector<MovementType> movementTypes{Rotational,Translational};
	for(int outerIndex=0;
			outerIndex<2;
			outerIndex++){
		JointPointerVector baseJointVector=m_jointController->getJoints(movementTypes[outerIndex]);
		for(JointPointerVector::iterator iter=baseJointVector.begin();
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
	return worked;
}
