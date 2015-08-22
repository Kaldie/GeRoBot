#ifndef RobotItem_H
#define RobotItem_H

#include <Robot.h>
#include "BaseRobotItem.h"

class RobotItem:public BaseRobotItem
{
 GET(Robot::RobotPointer,m_robotPointer,RobotPointer);

 private:
  const static QList<QString> propertyList;

	RobotItem();
	bool addJointControllerItem();

 public:
    RobotItem(BaseRobotItem* i_parent,
              Robot::RobotPointer i_robotPointer);

    void setRobotPointer(Robot::RobotPointer i_robotPointer);

	virtual QVariant getPropertyData(int i_row,int i_column) const;
	virtual bool setPropertyData(int i_row,int i_column,const QVariant& i_data);
	virtual bool construct();

};


#endif // RobotItem
