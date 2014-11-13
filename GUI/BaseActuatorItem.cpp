#include <macroHeader.h>
#include "BaseActuatorItem.h"
#include "PinStateItem.h"

const QList<QString> BaseActuatorItem::propertyList({"SerialRegularExpression","ReducedSpeed","BufferSize"});

BaseActuatorItem::BaseActuatorItem(BaseRobotItem* i_parent,
																	 ArduinoMotorDriver* i_arduinoMotorDriver):
	BaseRobotItem("BaseActuatorItem",i_parent),
	m_arduinoMotorDriver(i_arduinoMotorDriver)
{
	LOG_DEBUG(getElementName().toStdString());
	setNumberOfProperties(2);
}

QVariant BaseActuatorItem::getPropertyData(int i_row,
														 int i_column) const {
	if(i_column>1)
		return false;

	else if(i_row==BaseActuatorItem::propertyList.indexOf("SerialRegularExpression"))
		if(i_column==0)
			return QVariant(QString("Serial regular expression"));
		else
			return QVariant(QString(m_arduinoMotorDriver->getSerialRegularExpresion().c_str()));
	
	else if(i_row==BaseActuatorItem::propertyList.indexOf("ReducedSpeed"))
		if(i_column==0)
			return QVariant(QString("Hold motor"));
		else
			return QVariant(m_arduinoMotorDriver->getReducedSpeed());

	else if(i_row==BaseActuatorItem::propertyList.indexOf("BufferSize"))
		if(i_column==0)
			return QVariant(QString("Buffer size"));
		else
			return QVariant(m_arduinoMotorDriver->getBuffer().size());
	
	else
		return QVariant();
	
}

bool BaseActuatorItem::setPropertyData(int i_row,
																		int i_column,
																		const QVariant& i_value){
	if(!m_arduinoMotorDriver)
		return false;

	if(i_column==0)
		return false;

	if(i_row==BaseActuatorItem::propertyList.indexOf("SerialRegularExpression")){
		LOG_DEBUG(i_value.toString().toStdString());
		m_arduinoMotorDriver->setSerialRegularExpresion(i_value.toString().toStdString());
		return true;
	}

	else if(i_row==BaseActuatorItem::propertyList.indexOf("ReducedSpeed")){
		LOG_DEBUG(i_value.toString().toStdString());
		m_arduinoMotorDriver->setReducedSpeed(i_value.toBool());
		return true;
	}

	else if(i_row==BaseActuatorItem::propertyList.indexOf("BufferSize"))
		return false;

	else
		return false;
}


bool BaseActuatorItem::construct(){
	return createChilderen(BaseActuatorItem::propertyList); 
}
