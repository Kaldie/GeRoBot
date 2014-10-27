#ifndef JointItem_H
#define JointItem_H

#include <memory>
#include "BaseRobotItem.h"

class BaseJoint;
typedef std::shared_ptr<BaseJoint> JointPointer;

enum class BaseJointProperty{ElementName,Position,MovementPerStep,LowerRange,UpperRange,MovementType};

class BaseJointItem:public BaseRobotItem{

	GETSET(JointPointer,m_jointPointer,jointPointer);
	
 private:
	bool addBaseMotor();
	BaseJointItem();
 public:
	BaseJointItem(BaseRobotItem* i_parent,
								JointPointer i_joint);

	QVariant data(int row,int column) const;
	bool setData(int row,int column,const QVariant& i_data);
	
	virtual bool construct();
	
};

#endif // JointItem
