#include "PinStateItem.h"
#include <PinState.h>

PinStateItem::PinStateItem(BaseRobotItem* i_parent,
													 PinState* i_pinState):
	BaseRobotItem("PinState",i_parent),
	m_pinState(i_pinState)
{
	setNumberOfProperties(2);
}

bool PinStateItem::setData(int i_row,
													 int i_column,
													 const QVariant& i_data)
{
	if(!m_pinState)
		return false;
	
	if(i_row==static_cast<int>(PinStateProperty::ElementName))
		return BaseRobotItem::setData(i_row,i_column,i_data);
	
	//This property cannot be set!
	else if(i_row==static_cast<int>(PinStateProperty::NumericValue))
		return false;
	
	else if(i_row>(getNumberOfProperties()-1)){
		if(i_column==0)
			//this model does not allow to update the pin numbers. Currently this is not supported by the PinState class
			return false;
		else{
			//Updating the state of the pin
			int pinNumberEntry=i_row-(getNumberOfProperties()-1);
			int pinNumber=m_pinState->getPinVector()[pinNumberEntry];
			m_pinState->update(pinNumber,i_data.toInt());
			return true;
		}
	}

	return false;
}


QVariant PinStateItem::data(int i_row,
														int i_column)
{
	if(i_row==static_cast<int>(PinStateProperty::ElementName))
		return BaseRobotItem::data(i_row,i_column);
	
	//This property cannot be set!
	else if(i_row==static_cast<int>(PinStateProperty::NumericValue))
		return QVariant(m_pinState->getNumericValue());
	
	else if(i_row>(getNumberOfProperties()-1)){
		int pinNumberEntry=i_row-(getNumberOfProperties()-1);
		if(i_column==0)
			return QVariant(m_pinState->getPinVector()[pinNumberEntry]);
		else{
			//Updating the state of the pin
			int pinNumber=m_pinState->getPinVector()[pinNumberEntry];
			return QVariant(m_pinState->getPinState(pinNumber));
		}
	}
	return false;
}
	
bool PinStateItem::construct(){
	return true;
}
