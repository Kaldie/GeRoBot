// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "BasePropertyItem.h"

BasePropertyItem::BasePropertyItem(const QString& i_propertyName,
																	 BaseRobotItem* i_parent):
	BaseRobotItem( i_propertyName,i_parent)
{
	LOG_DEBUG(getElementName().toStdString());
	setNumberOfProperties(0);
}
