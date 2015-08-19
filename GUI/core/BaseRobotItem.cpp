/*
	BaseRobotItem.cpp
	
	A container for items of data supplied by the simple tree model.
	A container for items of robot parts which is derived from TreeItem
*/

#include <QStringList>
#include "BaseRobotItem.h"
#include "BasePropertyItem.h"

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
	return childCount();
}

QVariant BaseRobotItem::data(int row, int column) const{
  if(!m_parentItem){
    //if I'm root, I don't hold any data
    return QVariant();  
  }
  
  if(childCount() == 0){
    //I'm a property of my parent, route this call to the parent!
    return m_parentItem->getPropertyData(row,column);
  }
  
  else{
   //I'm a parent node, get my name if column is 0
    if(column == 0){
      return getElementName();
    }
  }
	return QVariant();
}


bool BaseRobotItem::setData(int row,int column,const QVariant& i_data){
	
  if(!m_parentItem){
    //if I'm root, I don't hold any data
    return false;  
  }
  
  else if(childCount()==0){
    //I'm a property of my parent, route this call to the parent!
    return m_parentItem->setPropertyData(row,column,i_data); 
  }
  
  else{
   //I'm a parent node myself however not the root, set my name if column is 0 or other parent magic
    if(column==0){
      setElementName(i_data.toString());
      return true;
    }
  }
	return false;
}


bool BaseRobotItem::createChilderen(const QList<QString>& i_propertyList){
	bool hasSucceeded(true);

	foreach (const QString& propertyName,i_propertyList){
		BasePropertyItem* property = new BasePropertyItem(propertyName,this);
		hasSucceeded&=insertChild(0,property);
	}
	return hasSucceeded;
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
