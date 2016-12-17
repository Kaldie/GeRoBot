#ifndef BaseMotorItem_H
#define BaseMotorItem_H

#include "BaseRobotItem.h"
class BaseMotor;

class BaseMotorItem:
public BaseRobotItem {
  GETSET(std::shared_ptr<BaseMotor>, m_baseMotor, BaseMotor)
 public:
  BaseMotorItem(BaseRobotItem* i_parrentItem,
		const std::shared_ptr<BaseMotor>& motorPointer);
  virtual QVariant getPropertyData(int row,int column) const;
  virtual bool setPropertyData(int row,int column, const QVariant& value);
  virtual bool construct();

 private:
	//No default constructor!
  BaseMotorItem();
  bool addPinStateItem();
  static const QList<QString> propertyList;
};



#endif // BaseMotorItem
