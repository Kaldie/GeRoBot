#include <macroHeader.h>
#include <EndStop.h>
#include "./EndStopItem.h"

const QList<QString> EndStopItem::propertyList({"Position", "PinNumber", "ActivationState", "ActivationDirection"});

EndStopItem::EndStopItem(BaseRobotItem* i_parent,
			 std::shared_ptr<EndStop> i_endStop)
  : BaseRobotItem("EndStop",i_parent),
    m_endStop(i_endStop) {
  LOG_DEBUG(getElementName().toStdString());
  setNumberOfProperties(4);
}

QVariant EndStopItem::getPropertyData(int i_row,
				      int i_column) const {
  if(i_column>1) {
    return false;
  }

  if(i_row == propertyList.indexOf("Position")) {
    if (i_column == 0) {
      return QVariant("Position");
    } else {
      return QVariant(m_endStop->getPosition());
    }
  }

  if(i_row == propertyList.indexOf("PinNumber")) {
    if (i_column == 0) {
      return QVariant("PinNumber");
    } else {
      return QVariant(m_endStop->getPinNumber());
    }
  }

  if(i_row == propertyList.indexOf("ActivationState")) {
    if (i_column == 0) {
      return QVariant("State of trigger stop");
    } else {
      return QVariant(m_endStop->getActivationState());
    }
  }

  if(i_row == EndStopItem::propertyList.indexOf("ActivationDirection")) {
    if (i_column == 0) {
      return QVariant("Direction in which the stop is triggered");
    } else {
      return QVariant(QString(m_endStop->getActivationDirection().c_str()));
    }
  }
  LOG_DEBUG("Could not find a property of row: " << i_row);
  return QVariant();
}

bool EndStopItem::setPropertyData(int i_row,
				    int i_column,
				    const QVariant& i_value){
  if(!m_endStop)
    return false;

  if(i_column!=1)
    return false;

  if(i_row==EndStopItem::propertyList.indexOf("Position")){
    m_endStop->setPosition(i_value.toDouble());
    return true;
  }
  
  if(i_row==EndStopItem::propertyList.indexOf("PinNumber")){
    m_endStop->setPinNumber(i_value.toInt());
    return true;
  }
  
  if(i_row==EndStopItem::propertyList.indexOf("ActivationState")){
    m_endStop->setActivationState(i_value.toBool());
    return true;
  }
  if(i_row==EndStopItem::propertyList.indexOf("ActivationDirection")){
    m_endStop->setActivationDirection(i_value.toString().toStdString());
    return true;
  }
  return false;
}


bool EndStopItem::construct(){
  return createChilderen(propertyList);
}
