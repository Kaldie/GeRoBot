#include "BaseMotorItem.h"
#include "PinStateItem.h"

BaseMotorItem::BaseMotorItem(BaseRobotItem* i_parent,
														 BaseMotor* i_baseMotor):
	BaseRobotItem("BaseMotor",i_parent),
	m_baseMotor(i_baseMotor)
{
	setNumberOfProperties(3);
}

QVariant BaseMotorItem::data(int i_row,
														 int i_column) const {
	if(i_row==static_cast<int>(BaseMotorProperty::ElementName))
		return BaseRobotItem::data(i_row,i_column);

	if(i_column>1)
		return false;

	else if(i_row==static_cast<int>(BaseMotorProperty::DefaultDirection))
		if(i_column==0)
			return QVariant(QString("Default Direction"));
		else
			return QVariant(QString(m_baseMotor->getDefaultDirection().c_str()));
	
	else if(i_row==static_cast<int>(BaseMotorProperty::HoldMotor))
		if(i_column==0)
			return QVariant(QString("Hold Motor"));
		else
			return m_baseMotor->getHoldMotor();
	
	else
		return QVariant();
	
}

bool BaseMotorItem::setData(int i_row,
														int i_column,
														const QVariant& i_value){
	if(!m_baseMotor)
		return false;

	if(i_row==static_cast<int>(BaseMotorProperty::ElementName))
		return BaseRobotItem::setData(i_row,i_column,i_value);

	if(i_column==0)
		return false;

	if(i_column==1)
		return false;

	if(i_row==static_cast<int>(BaseMotorProperty::DefaultDirection)){
		m_baseMotor->setDefaultDirection(i_value.toString().toStdString());
		return true;
	}
		else if(i_row==static_cast<int>(BaseMotorProperty::HoldMotor)){
		m_baseMotor->setHoldMotor(i_value.toBool());
		return true;
	}
	else
		return false;
}


bool BaseMotorItem::addPinStateItem(){
	if(!m_baseMotor)
		return false;
	
	//Create a pin state item with this as the parent
	PinStateItem* pinStateItemPointer = new PinStateItem(this,m_baseMotor->getPinStatePointer());
	pinStateItemPointer->construct();
	insertChild(0,pinStateItemPointer);	
	return true;
}

bool BaseMotorItem::construct(){
	return addPinStateItem();
}
