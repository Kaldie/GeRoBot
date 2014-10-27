#ifndef JointControllerItem_H
#define JointControllerItem_H

#include <BaseMotor.h>
#include "BaseRobotItem.h"

enum class JointControllerProperty{ElementName};

class JointControllerItem:
public BaseRobotItem
{
	GETSETPOINTER(JointController,m_jointController,JointController);

 private:
	//No default constructor!
	JointControllerItem();
	bool addPinStateItem();
	
 public:
	JointControllerItem(BaseRobotItem* i_parrentItem,
											JointController* motorPointer);
	
	virtual QVariant data(int row,int column) const;
	virtual bool setData(int row,int column, const QVariant& value);	
	
	virtual bool construct();
};


#endif // JointControllerItem
