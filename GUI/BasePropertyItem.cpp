
#include "BasePropertyItem.h"

BasePropertyItem::BasePropertyItem(const QString i_propertyName,
																	 BaseRobotItem* i_parent):
	BaseRobotItem(i_propertyName,i_parent)
{
	LOG_DEBUG(getElementName().toStdString());
	setNumberOfProperties(0);
}


QVariant BasePropertyItem::data(int i_row,
																int i_column) const {
	if(childCount()==0){
			LOG_DEBUG("Child count is 0");
			return getParentItem()->data(i_row,i_column);
	}
	else{
		LOG_DEBUG("number of childs: "<<childCount());
		return getChildItems()[i_row]->data(0,0);

	}
}

bool BasePropertyItem::setData(int i_row,
															 int i_column,
															 const QVariant& i_value){
	if(childCount()==0){
			LOG_DEBUG("Child count is 0");
			return getParentItem()->setData(i_row,i_column,i_value);
	}
	else{
		return child(i_row)->setData(0,0,i_value);
	}
}
