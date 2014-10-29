#ifndef BaseProperty_H
#define BaseProperty_H

#include "BaseRobotItem.h"

class BasePropertyItem:public BaseRobotItem
{
	
 public:
	BasePropertyItem(const QString i_propertyName,
									 BaseRobotItem* i_parent=0);
	
	virtual QVariant data(int row,int column) const;
	virtual bool setData(int row,int column, const QVariant &value);
	
	bool construct(){return true;};

};

#endif // BaseProperty

