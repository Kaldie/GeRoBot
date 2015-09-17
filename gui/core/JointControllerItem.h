#ifndef JointControllerItem_H
#define JointControllerItem_H

#include <BaseMotor.h>
#include "BaseRobotItem.h"

class JointControllerItem:public BaseRobotItem
{
	GETSET(JointController::JointControllerPointer, m_jointController, JointController);
 private:
	//No default constructor!
	JointControllerItem();
	bool addArduinoActuator();
 public:
	JointControllerItem(BaseRobotItem* i_parrentItem,
			    const JointController::JointControllerPointer& motorPointer);
	virtual QVariant getPropertyData(int,int) const {return QVariant();};
	virtual bool setPropertyData(int,int, const QVariant&) {return false;};
	virtual bool construct();
};


#endif // JointControllerItem
