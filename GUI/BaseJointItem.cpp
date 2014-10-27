#include "BaseJointItem.h"
#include "BaseMotorItem.h"
#include <BaseJoint.h>

BaseJointItem::BaseJointItem(BaseRobotItem* i_parent,
														 JointPointer i_joint):
	BaseRobotItem("BaseJoint",i_parent),
	m_jointPointer(i_joint)
{
	setNumberOfProperties(6);
}


QVariant BaseJointItem::data(int i_row,int i_column) const {
	if(i_row==static_cast<int>(BaseJointProperty::ElementName))
		return BaseRobotItem::data(i_row,i_column);
	
	if(i_row==static_cast<int>(BaseJointProperty::Position)){
		if(i_column==1)
			return QVariant(m_jointPointer->getPosition());
		else
			return QVariant(QString("Position"));
	}
	
	else if(i_row==static_cast<int>(BaseJointProperty::MovementPerStep)){
		if(i_column==1)
			return QVariant(m_jointPointer->getMovementPerStep());
		else
			return QVariant(QString("Movment per step"));
	}
	
	else if(i_row==static_cast<int>(BaseJointProperty::LowerRange)){
		if(i_column==1)
			return QVariant(m_jointPointer->getRange()[0]);
		else
			return QVariant(QString("Lower position range"));
	}

	else if(i_row==static_cast<int>(BaseJointProperty::UpperRange)){
		if(i_column==1)
			return QVariant(m_jointPointer->getRange()[1]);
		else
			return QVariant(QString("Upper position range"));
	}
	
	else if(i_row==static_cast<int>(BaseJointProperty::MovementType)){
		if(i_column==1){
			MovementType movementType=m_jointPointer->getMovementType();
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

bool BaseJointItem::setData(int i_row,int i_column,const QVariant& i_data){
	if(i_row==static_cast<int>(BaseJointProperty::ElementName))
		return BaseRobotItem::setData(i_row,i_column,i_data);
	
	if(i_column!=1)
		return false;

	bool isConverted=bool(false);
	bool* hasConverted = &isConverted;
	
	if(i_row==static_cast<int>(BaseJointProperty::Position))
		m_jointPointer->setPosition(i_data.toDouble(hasConverted));

	else if(i_row==static_cast<int>(BaseJointProperty::MovementPerStep))
		m_jointPointer->setMovementPerStep(i_data.toDouble(hasConverted));
	
	else if(i_row==static_cast<int>(BaseJointProperty::LowerRange)){
		float upperRange=m_jointPointer->getRange()[1];
		float lowerRange=i_data.toDouble(hasConverted);
		m_jointPointer->setRange({lowerRange,upperRange});
	}
	else if(i_row==static_cast<int>(BaseJointProperty::UpperRange)){
		float lowerRange=m_jointPointer->getRange()[0];
		float upperRange=i_data.toDouble(hasConverted);
		m_jointPointer->setRange({lowerRange,upperRange});
	}

	else if(i_row==static_cast<int>(BaseJointProperty::MovementType)){
		MovementType movementType;
		
		if("Rotational"==i_data.toString().toStdString()){
			movementType=Rotational;
			isConverted=true;
		}
		else if("Translational"==i_data.toString().toStdString()){
			movementType=Translational;
			isConverted=(*hasConverted);
		}
		else
			return false;
		m_jointPointer->setMovementType(movementType);
	}
	return isConverted;
}		

bool BaseJointItem::construct(){
	return addBaseMotor();
}

bool BaseJointItem::addBaseMotor(){
	if(m_jointPointer)
		return false;
	
	//Create a base motor item with this as the parent
	BaseMotorItem* baseMotorItem= new BaseMotorItem(this,(m_jointPointer->getMotor()));
	baseMotorItem->construct();
	insertChild(0,baseMotorItem);	
	return true;
}
