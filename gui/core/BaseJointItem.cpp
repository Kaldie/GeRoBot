#include <macroHeader.h>
#include "BaseJointItem.h"
#include "BaseMotorItem.h"
#include <BaseJoint.h>

const QList<QString> BaseJointItem::propertyList({"Position","MovementPerStep","LowerRange","UpperRange","MovementType"});

BaseJointItem::BaseJointItem(BaseRobotItem* i_parent,
														 JointPointer i_joint):
	BaseRobotItem("BaseJoint",i_parent),
	m_jointPointer(i_joint)
{
	LOG_DEBUG(getElementName().toStdString());
	setNumberOfProperties(6);
	LOG_DEBUG("Current position: "<<m_jointPointer->getPosition());
}


QVariant BaseJointItem::getPropertyData(int i_row,int i_column) const {
	if(i_row==BaseJointItem::propertyList.indexOf("Position")){
		if(i_column==1)
			return QVariant(m_jointPointer->getPosition());
		else
			return QVariant(QString("Position"));
	}
	
	else if(i_row==BaseJointItem::propertyList.indexOf("MovementPerStep")){
		if(i_column==1)
			return QVariant(m_jointPointer->getMovementPerStep());
		else
			return QVariant(QString("Movment per step"));
	}
	
	else if(i_row==BaseJointItem::propertyList.indexOf("LowerRange")){
		if(i_column==1)
			return QVariant(m_jointPointer->getRange()[0]);
		else
			return QVariant(QString("Lower position range"));
	}

	else if(i_row==BaseJointItem::propertyList.indexOf("UpperRange")){
		if(i_column==1)
			return QVariant(m_jointPointer->getRange()[1]);
		else
			return QVariant(QString("Upper position range"));
	}
	
	else if(i_row==BaseJointItem::propertyList.indexOf("MovementType")){
		if(i_column==1){
            BaseJoint::MovementType movementType=m_jointPointer->getMovementType();
			if(movementType==1)
				return QVariant(QString("Rotational"));
			else if(movementType==2)
				return QVariant(QString("Translational"));
			else
				return QVariant();
		}
		else
			return QVariant(QString("Movement type"));
	}
	
	else
		return QVariant();
}		

bool BaseJointItem::setPropertyData(int i_row,int i_column,const QVariant& i_data){
	if(i_column!=1)
		return false;

	bool isConverted=bool(false);
	bool* hasConverted = &isConverted;
	
	if(i_row==BaseJointItem::propertyList.indexOf("Position"))
		m_jointPointer->setPosition(i_data.toDouble(hasConverted));

	else if(i_row==BaseJointItem::propertyList.indexOf("MovementPerStep"))
		m_jointPointer->setMovementPerStep(i_data.toDouble(hasConverted));
	
	else if(i_row==BaseJointItem::propertyList.indexOf("LowerRange")){
		float upperRange=m_jointPointer->getRange()[1];
		float lowerRange=i_data.toDouble(hasConverted);
		m_jointPointer->setRange({lowerRange,upperRange});
	}
	else if(i_row==BaseJointItem::propertyList.indexOf("UpperRange")){
		float lowerRange=m_jointPointer->getRange()[0];
		float upperRange=i_data.toDouble(hasConverted);
		m_jointPointer->setRange({lowerRange,upperRange});
	}

	else if(i_row==BaseJointItem::propertyList.indexOf("MovementType")){
        BaseJoint::MovementType movementType;
		
		if("Rotational"==i_data.toString().toStdString()){
            movementType = BaseJoint::Rotational;
			isConverted=true;
		}
		else if("Translational"==i_data.toString().toStdString()){
            movementType=BaseJoint::Translational;
			isConverted=(*hasConverted);
		}
		else
			return false;
		m_jointPointer->setMovementType(movementType);
	}
	return isConverted;
}		

bool BaseJointItem::construct(){
	return addBaseMotor() && createChilderen(BaseJointItem::propertyList);
}

bool BaseJointItem::addBaseMotor(){
	if(!m_jointPointer)
		return false;

	LOG_DEBUG("Add base motor child");
	//Create a base motor item with this as the parent
	BaseMotorItem* baseMotorItem= new BaseMotorItem(this,(m_jointPointer->getMotor()));
	baseMotorItem->construct();
	insertChild(0,baseMotorItem);	
	return true;
}
