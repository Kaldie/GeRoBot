#ifndef SPEEDCONTROLLERITEM_H_
#define SPEEDCONTROLLERITEM_H_

#include "BaseRobotItem.h"
#include <SpeedController.h>

class SpeedControllerItem:public BaseRobotItem
{
        GETSET(SpeedController::SpeedControllerPointer,
               m_speedController,
               SpeedController);

private:
        //No default contructor
        SpeedControllerItem();
        static const QList<QString> propertyList;

public:
        SpeedControllerItem(BaseRobotItem* i_parent,
                            const SpeedController::SpeedControllerPointer& i_speedController);
        virtual bool construct();
        virtual QVariant getPropertyData(int row,int column) const;
        virtual bool setPropertyData(int row,int column, const QVariant &value);
	// Yes for the controller type row
	virtual bool useComboBoxDelegate(const int& i_row) const;
	virtual std::vector<std::string> getComboBoxElements(const int& i_row) const;
	virtual void setElement(const int& i_row, const int& i_selectedItem);
};

#endif // SPEEDCONTROLLERITEM_H_
