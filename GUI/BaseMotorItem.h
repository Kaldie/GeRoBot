#ifndef BaseMotorItem_H
#define BaseMotorItem_H

#include <BaseMotor.h>
#include "BaseRobotItem.h"

enum class BaseMotorProperty{ElementName,DefaultDirection,HoldMotor};

class BaseMotorItem:
public BaseRobotItem
{
	GETSETPOINTER(BaseMotor,m_baseMotor,BaseMotor);

 private:
	//No default constructor!
	BaseMotorItem();
	bool addPinStateItem();
	
 public:
	BaseMotorItem(BaseRobotItem* i_parrentItem,
								BaseMotor* motorPointer);
	
	virtual QVariant data(int row,int column) const;
	virtual bool setData(int row,int column, const QVariant& value);	
	
	virtual bool construct();
};

#endif // BaseMotorItem
