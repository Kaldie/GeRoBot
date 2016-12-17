#ifndef JointControllerItem_H
#define JointControllerItem_H

#include <BaseMotor.h>
#include "BaseRobotItem.h"

class JointControllerItem:public BaseRobotItem
{
  GETSET(std::shared_ptr<JointController>, m_jointController, JointController);
 private:
	//No default constructor!
	JointControllerItem();
	bool addArduinoActuator();
 public:
	JointControllerItem(BaseRobotItem* i_parrentItem,
			    const std::shared_ptr<JointController>& i_jointController);
	virtual QVariant getPropertyData(int,int) const {return QVariant();};
	virtual bool setPropertyData(int,int, const QVariant&) {return false;};
	virtual bool construct();
};


#endif // JointControllerItem
