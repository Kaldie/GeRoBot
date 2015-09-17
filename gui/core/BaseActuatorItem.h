#ifndef BaseActuatorItem_H
#define BaseActuatorItem_H

#include <ArduinoMotorDriver.h>
#include "BaseRobotItem.h"

class BaseActuatorItem: public BaseRobotItem {
  GETSETPOINTER(ArduinoMotorDriver,m_arduinoMotorDriver,ArduinoMotorDriver);
 public:
  BaseActuatorItem(BaseRobotItem* i_parrentItem,
		   ArduinoMotorDriver* motorPointer);
  virtual QVariant getPropertyData(int row, int column) const;
  virtual bool setPropertyData(int row,int column, const QVariant& value);
  virtual bool construct();

 private:
	//No default constructor!
	BaseActuatorItem();
	static const QList<QString> propertyList;
};



#endif // BaseActuatorItem
