#ifndef RobotItem_H
#define RobotItem_H

#include "BaseRobotItem.h"
#include <Robot.h>

class RobotItem:public BaseRobotItem
{

	GET(RobotPointer,m_robotPointer,RobotPointer);

 private:
  const static QList<QString> propertyList({"Speed","CurrentPositionX","CurrentPositionY"});
  
	RobotItem();
	bool addJointControllerItem();
  
	
 public:
	RobotItem(BaseRobotItem* i_parent,
						RobotPointer i_robotPointer);
	
	virtual QVariant getPropertyData(int i_row,int i_column) const;
	virtual bool setPropertyData(int i_row,int i_column,const QVariant& i_data);
	
	void setRobotPointer(RobotPointer i_robotPointer);

	virtual bool construct();
	
};


#endif // RobotItem
