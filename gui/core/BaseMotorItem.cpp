#include <macroHeader.h>
#include <BaseMotor.h>
#include "BaseMotorItem.h"
#include "PinStateItem.h"

const QList<QString> BaseMotorItem::propertyList({"DefaultDirection","HoldMotor"});

BaseMotorItem::BaseMotorItem(BaseRobotItem* i_parent,
			     const BaseMotor::MotorPointer& i_baseMotor)
  : BaseRobotItem("BaseMotor",i_parent),
    m_baseMotor(i_baseMotor) {
      LOG_DEBUG(getElementName().toStdString());
      setNumberOfProperties(2);
}

QVariant BaseMotorItem::getPropertyData(int i_row,
					int i_column) const {
  if(i_column>1) {
    return false;
  } else if(i_row == BaseMotorItem::
	    propertyList.indexOf("DefaultDirection")) {
    if (i_column == 0) {
      return QVariant("Default Direction");
    } else {
      return QVariant(QString(m_baseMotor->getDefaultDirection().c_str()));
    }
  } else if(i_row == BaseMotorItem::
	    propertyList.indexOf("HoldMotor")) {
    if(i_column == 0) {
      return QVariant("Hold Motor");
    } else {
      return m_baseMotor->getHoldMotor();
    }
  } else {
    return QVariant();
  }
}

bool BaseMotorItem::setPropertyData(int i_row,
				    int i_column,
				    const QVariant& i_value){
  if(!m_baseMotor)
    return false;

  if(i_column==0)
    return false;

  if(i_row==BaseMotorItem::propertyList.indexOf("DefaultDirection")){
    LOG_DEBUG(i_value.toString().toStdString());
    m_baseMotor->setDefaultDirection(i_value.toString().toStdString());
    return true;
  }
  else if(i_row==BaseMotorItem::propertyList.indexOf("HoldMotor")){
    m_baseMotor->setHoldMotor(i_value.toBool());
    return true;
  }
  else
    return false;
}


bool BaseMotorItem::addPinStateItem(){
  if(!m_baseMotor) {
    return false;
  }
  LOG_DEBUG("Create pin state and add to child!");
  //Create a pin state item with this as the parent
  PinStateItem* pinStateItemPointer = new PinStateItem(this,m_baseMotor->getPinStatePointer());
  pinStateItemPointer->construct();
  insertChild(0,pinStateItemPointer);
	return true;
}

bool BaseMotorItem::construct(){
        return addPinStateItem() && createChilderen(BaseMotorItem::propertyList);
}
