// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "SpeedControllerItem.h"
#include "BasePropertyItem.h"
#include "RobotItem.h"
#include <ConstantSpeedController.h>
#include <RelativeSpeedController.h>
#include <PrescribedSpeedController.h>

const QList<QString> SpeedControllerItem::propertyList{"Type", "RobotSpeed"};

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
  LOG_DEBUG("Set property data!");
  if(i_column!=1)
    return false;

  bool isConverted=bool(false);

  if(i_row==SpeedControllerItem::propertyList.indexOf("Type")) {
    return true;
  } else if (i_row==SpeedControllerItem::propertyList.indexOf("RobotSpeed")) {
    m_speedController->setRobotSpeed(i_data.toDouble(&isConverted));
  }
  return isConverted;
}


QVariant SpeedControllerItem::getPropertyData(int i_row,
                                              int i_column)const {
  if(i_row==SpeedControllerItem::propertyList.indexOf("Type")) {
    if(i_column==0) {
      return QVariant("Type");
    } else {
        switch (m_speedController->getType()) {
        case SpeedController::Type::Constant:
            return QVariant("ConstantSpeedController");
        case SpeedController::Type::Relative:
            return QVariant("RelativeSpeedController");
        case SpeedController::Type::Prescribed:
            return QVariant("PrescribedSpeedController");
        default:
            return QVariant("UnknownSpeedController");
        }
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


bool SpeedControllerItem::useComboBoxDelegate(const int& i_row) const {
  if (i_row != 0) {
    return false;
  }
  return true;
}

std::vector<std::string> SpeedControllerItem::getComboBoxElements(const int& i_row) const {
  if (i_row != 0) {
    return std::vector<std::string>();
  }
  return std::vector<std::string>{"None", "Constant", "Prescribed", "Relative"};
}


void SpeedControllerItem::setElement(const int& i_row, const int& i_selectedItem) {
  if (i_row != 0) {
    return;
  }

  SpeedController* raw_pointer;
  switch (i_selectedItem) {
  case 0:
    LOG_DEBUG("Cannot select None as a speed controller!");
    return;
  case 1:
    raw_pointer = new ConstantSpeedController(m_speedController->getRobotSpeed(),
                m_speedController->getCurrentSequenceVectorPosition());
    break;
  case 2:
    raw_pointer = new PrescribedSpeedController(m_speedController->getRobotSpeed(),
            m_speedController->getCurrentSequenceVectorPosition());
    break;
  case 3:
    raw_pointer = new RelativeSpeedController(m_speedController->getRobotSpeed(),
                m_speedController->getCurrentSequenceVectorPosition());
  }

  if (!raw_pointer) {
    return;
  }

  std::shared_ptr<SpeedController> shrd_ptr;
  shrd_ptr.reset(raw_pointer);
  LOG_DEBUG("Pre use count: " << m_speedController.use_count());
  static_cast<RobotItem*>(parent())->getRobotPointer()->setSpeedController(shrd_ptr);
  LOG_DEBUG("Post use count: " << m_speedController.use_count());
  m_speedController = static_cast<RobotItem*>(parent())->getRobotPointer()->getSpeedController();
}

