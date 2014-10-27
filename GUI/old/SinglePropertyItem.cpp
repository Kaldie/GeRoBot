/*
	SinglePropertyItem.cpp
	
	A container for items of data supplied by the simple tree model.
	A container for items of robot parts which is derived from TreeItem
*/

#include <QStringList>
#include "SinglePropertyItem.h"

SinglePropertyItem::SinglePropertyItem(const QString& i_elementName, SinglePropertyItem *parent):
	parentItem(parent),
	m_elementName(i_elementName)
{}


SinglePropertyItem::~SinglePropertyItem(){
}
