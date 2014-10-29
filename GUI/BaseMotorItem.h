#ifndef BaseMotorItem_H
#define BaseMotorItem_H

#include <BaseMotor.h>
#include "BaseRobotItem.h"

class BaseMotorItem:
public BaseRobotItem
{
	GETSETPOINTER(BaseMotor,m_baseMotor,BaseMotor);

 private:
	//No default constructor!
	BaseMotorItem();
	bool addPinStateItem();
	static const QList<QString> propertyList;

 public:
	BaseMotorItem(BaseRobotItem* i_parrentItem,
								BaseMotor* motorPointer);
	
	virtual QVariant getPropertyData(int row,int column) const;
	virtual bool setPropertyData(int row,int column, const QVariant& value);	
	
	virtual bool construct();
};



#endif // BaseMotorItem
