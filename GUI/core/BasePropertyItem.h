#ifndef BaseProperty_H
#define BaseProperty_H

#include "BaseRobotItem.h"

class BasePropertyItem:public BaseRobotItem
{
 private:
	BasePropertyItem();
 public:
	BasePropertyItem(const QString& i_propertyName,
									 BaseRobotItem* i_parent=0);
	
	virtual bool construct(){return true;};
	virtual QVariant getPropertyData(int,int) const {return QVariant();};
	virtual bool setPropertyData(int,int, const QVariant&){return false;};
};

#endif // BaseProperty

