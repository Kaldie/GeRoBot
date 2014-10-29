#ifndef JointItem_H
#define JointItem_H

#include <memory>
#include "BaseRobotItem.h"

class BaseJoint;
typedef std::shared_ptr<BaseJoint> JointPointer;

class BaseJointItem:public BaseRobotItem{

	GETSET(JointPointer,m_jointPointer,jointPointer);
	
 private:
	BaseJointItem();
	bool addBaseMotor();
	const static QList<QString> propertyList;

 public:
	BaseJointItem(BaseRobotItem* i_parent,
								JointPointer i_joint);
	
	QVariant getPropertyData(int row,int column) const;
	bool setPropertyData(int row,int column,const QVariant& i_data);
	
	virtual bool construct();
	
};

#endif // JointItem
