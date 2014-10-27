#ifndef PinStateItem_H
#define PinStateItem_H

#include "BaseRobotItem.h"

class PinState;
enum class PinStateProperty{ElementName,NumericValue};

class PinStateItem:
public BaseRobotItem
{

	GETSETPOINTER(PinState,m_pinState,PinState);
	
 private:
	//No default contructor
	PinStateItem();
	
 public:
	PinStateItem(BaseRobotItem* i_parent,
							 PinState* i_pinState);

	QVariant data(int row,int column);
	bool setData(int row,int column ,const QVariant& i_data);
	
	virtual bool construct();
};


#endif // PinStateItem
