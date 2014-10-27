/*
	BaseRobotItem.cpp
	
	A container for items of data supplied by the simple tree model.
	A container for items of robot parts which is derived from TreeItem
*/

#include <QStringList>
#include "BaseRobotItem.h"

BaseRobotItem::BaseRobotItem(const QString& i_elementName, BaseRobotItem *parent):
	m_parentItem(parent),
	m_elementName(i_elementName)
{}


BaseRobotItem::~BaseRobotItem(){
	qDeleteAll(getChildItems());
}


BaseRobotItem *BaseRobotItem::child(int number){
	return m_childItems.value(number);
}


int BaseRobotItem::childCount() const{
	return m_childItems.count();
}


int BaseRobotItem::childNumber() const{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<BaseRobotItem*>(this));
	return 0;
}

int BaseRobotItem::rowCount(){
	LOG_DEBUG("Number of rows: "<<m_numberOfPropeties+ childCount());
	return m_numberOfPropeties+ childCount();
}

QVariant BaseRobotItem::data(int row, int column) const{
	if(row==0 and column==0)
		return QVariant(QString(getElementName()));
	else
		return QVariant();
}


bool BaseRobotItem::setData(int row,int column,const QVariant& i_data){
	
	if(row==0 and column==0){
		setElementName(i_data.toString());
		return true;
	}
	else
		return false;
}


bool BaseRobotItem::insertChild(int position, BaseRobotItem* i_child){
	if (position < 0 || position > m_childItems.size()){
		return false;
		LOG_DEBUG("Could not insert child due to stuff");
	}
	m_childItems.insert(position, i_child);
	
	LOG_DEBUG("Insert child");
	return true;
}


BaseRobotItem *BaseRobotItem::parent(){
    return m_parentItem;
}

 
bool BaseRobotItem::removeChildren(int position, int count){
	if (position < 0 || position + count > m_childItems.size())
		return false;
	
	for (int row = 0; row < count; ++row)
		delete m_childItems.takeAt(position);
	
	return true;
}
