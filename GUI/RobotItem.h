#ifndef RobotItem_H
#define RobotItem_H

#include "BaseRobotItem.h"
#include <Robot.h>

enum class RobotProperty{ElementName,Speed,CurrentPositionX,CurrentPositionY};

class RobotItem:public BaseRobotItem
{

	GET(RobotPointer,m_robotPointer,RobotPointer);

 private:
	RobotItem();
	bool addJointControllerItem();
	
 public:
	RobotItem(BaseRobotItem* i_parent,
						RobotPointer i_robotPointer);
	
	QVariant data(int i_row,int i_column) const;
	bool setData(int i_row,int i_column,const QVariant& i_data);
	
	void setRobotPointer(RobotPointer i_robotPointer);

	virtual bool construct();
	
};


#endif // RobotItem
