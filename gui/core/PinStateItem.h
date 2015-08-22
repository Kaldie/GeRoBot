#ifndef PinStateItem_H
#define PinStateItem_H

#include "BaseRobotItem.h"

class PinState;

class PinStateItem:public BaseRobotItem
{
	GETSETPOINTER(PinState,m_pinState,PinState);
	
 private:
	//No default contructor
	PinStateItem();
	bool createPinChildItems();
	static const QList<QString> propertyList;

 public:
	PinStateItem(BaseRobotItem* i_parent,
							 PinState* i_pinState);
	
	virtual bool construct();
  virtual QVariant getPropertyData(int row,int column) const;
  virtual bool setPropertyData(int row,int column, const QVariant &value);
};


#endif // PinStateItem
