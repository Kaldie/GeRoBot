/*
	SinglePropertyItem.cpp
	
	A container for items of data supplied by the simple tree model.
	A container for items of robot parts which is derived from TreeItem
*/

#include <QStringList>
#include "SinglePropertyItem.h"

SinglePropertyItem::SinglePropertyItem(const QString& i_elementName, SinglePropertyItem *parent):
	parentItem(parent),
	m_isProperty(false),
	m_elementName(i_elementName),
	m_propertyPointer(NULL)
{}


SinglePropertyItem::~SinglePropertyItem(){
	qDeleteAll(childItems);
}


SinglePropertyItem *SinglePropertyItem::child(int number){
	return childItems.value(number);
}


int SinglePropertyItem::childCount() const{
	return childItems.count();
}


int SinglePropertyItem::childNumber() const{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<SinglePropertyItem*>(this));
	
	return 0;
}


QVariant SinglePropertyItem::data(int column) const{
	/*
		data block always consists of 2 items, Name and a property.
		The propery is "" if the element is not an propery
	*/
	
	if(0<column ||column<2){
		if(0==column)
			return m_elementName;
		
		else if (column==1 && childCount()==0)
			return getChildData(this);
		else
			return QVariant();
	}
}


bool SinglePropertyItem::insertChildren(int position, int count, QString i_defaultChildName){
	if (position < 0 || position > childItems.size())
		return false;
	
	for (int row = 0; row < count; ++row) {
		SinglePropertyItem *item = new SinglePropertyItem(i_defaultChildName, this);
		childItems.insert(position, item);
	}
	
	return true;
}


SinglePropertyItem *SinglePropertyItem::parent(){
	return parentItem;
}

 
bool SinglePropertyItem::removeChildren(int position, int count){
	if (position < 0 || position + count > childItems.size())
		return false;
	
	for (int row = 0; row < count; ++row)
		delete childItems.takeAt(position);
	
	return true;
}


bool SinglePropertyItem::setData(int column, const QVariant &value){
	if (column < 0 || column >= 2)
		return false;
	
	if(column == 0){
		m_elementName=value.toString();
		return true;
	}

	else if(column == 1 childCount()==0){
		setChildData(this,value);
		return true;
	}
	else
		return false;
}
