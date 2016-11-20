// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "SpeedControllerItem.h"
#include "BasePropertyItem.h"

const QList<QString> SpeedControllerItem::propertyList{"Name", "RobotSpeed"};

SpeedControllerItem::SpeedControllerItem(BaseRobotItem* i_parent,
                                         const SpeedController::SpeedControllerPointer& i_speedController)
        : BaseRobotItem("SpeedController",i_parent),
          m_speedController(i_speedController) {
  LOG_DEBUG(getElementName().toStdString());
  setNumberOfProperties(2);
}

bool SpeedControllerItem::setPropertyData(int i_row,
                                          int i_column,
                                          const QVariant& i_data){
  if(i_column!=1)
    return false;

  bool isConverted=bool(false);
  bool* hasConverted = &isConverted;

  if(i_row==SpeedControllerItem::propertyList.indexOf("Name")) {
    m_speedController->setName(i_data.toString().toStdString());
  } else if (i_row==SpeedControllerItem::propertyList.indexOf("Name")) {
    m_speedController->setRobotSpeed(i_data.toDouble(hasConverted));
  }
  return isConverted;
}


QVariant SpeedControllerItem::getPropertyData(int i_row,
                                              int i_column)const {
  if(i_row==SpeedControllerItem::propertyList.indexOf("Name")) {
    if(i_column==0) {
      return QVariant("Name");
    } else {
      return QVariant(QString::fromStdString(m_speedController->getName()));
    }
  } else if(i_row==SpeedControllerItem::propertyList.indexOf("RobotSpeed")){
    if(i_column==0)
      return QVariant("Robot speed in mm/sec");
    else{
      return QVariant(m_speedController->getRobotSpeed());
    }
  }
  return QVariant();
}


bool SpeedControllerItem::construct(){
  return createChilderen(SpeedControllerItem::propertyList);
}