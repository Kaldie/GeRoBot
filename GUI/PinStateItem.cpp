#include "PinStateItem.h"
#include <PinState.h>
#include "BasePropertyItem.h"

const QList<QString> PinStateItem::propertyList{"NumericValue"};

PinStateItem::PinStateItem(BaseRobotItem* i_parent,
													 PinState* i_pinState):
	BaseRobotItem("PinState",i_parent),
	m_pinState(i_pinState)
{
	LOG_DEBUG(getElementName().toStdString());
	setNumberOfProperties(1);
}

bool PinStateItem::setPropertyData(int i_row,
																	 int i_column,
																	 const QVariant& i_data){
	if(!m_pinState)
		return false;
	
	//This property cannot be set!
	else if(i_row==PinStateItem::propertyList.indexOf("NumericValue"))
		return false;
	
	else if(i_row>(getNumberOfProperties()-1)){
		if(i_column==0)
			//this model does not allow to update the pin numbers. Currently this is not supported by the PinState class
			return false;
		else{
			//Updating the state of the pin
			int pinNumberEntry=i_row-getNumberOfProperties();
			int pinNumber=m_pinState->getPinVector()[pinNumberEntry];
			LOG_DEBUG("Pin number entry: "<<pinNumberEntry<<" corrispond to: "<<pinNumber);
			m_pinState->update(pinNumber,i_data.toInt());
			return true;
		}
	}
	return false;
}


QVariant PinStateItem::getPropertyData(int i_row,
																			 int i_column)const
{
	
	if(i_row==PinStateItem::propertyList.indexOf("NumericValue"))
		if(i_column==0)
			return QVariant("Numerical Value");
		else
			return QVariant(m_pinState->getNumericValue());
	
	else if(i_row>(getNumberOfProperties()-1)){
		int pinNumberEntry=i_row-getNumberOfProperties();
		if(i_column==0)
			return QVariant(QString("State pin: ") + QString::number(m_pinState->getPinVector()[pinNumberEntry]));
		else{
			//Updating the state of the pin
			int pinNumber=m_pinState->getPinVector()[pinNumberEntry];
			LOG_DEBUG("Pin number entry: "<<pinNumberEntry<<" corrispond to: "<<pinNumber);
			return QVariant(m_pinState->getPinState(pinNumber));
		}
	}
	return false;
}
	
bool PinStateItem::createPinChildItems(){
	bool hasSucceeded(true);
	
	foreach (const int& pinNumber,m_pinState->getPinVector()){
		LOG_DEBUG(pinNumber);
		BasePropertyItem* pinItem = new BasePropertyItem(QString::number(pinNumber),this);
		hasSucceeded&=insertChild(0,pinItem);
	}
	return hasSucceeded;
}

bool PinStateItem::construct(){
	
	return createChilderen(PinStateItem::propertyList) && createPinChildItems();
}
