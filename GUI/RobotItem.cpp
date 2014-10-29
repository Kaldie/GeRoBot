#include "RobotItem.h"
#include <Point2D.h>
#include "JointControllerItem.h"

RobotItem::RobotItem(BaseRobotItem* i_parent,
										 RobotPointer i_robotPointer):
	BaseRobotItem("Robot",i_parent)
{
	m_robotPointer = i_robotPointer;
	setNumberOfProperties(3);
}

QVariant RobotItem::getPropertyData(int i_row,int i_column) const { 

  if(i_row==RobotItem::propertyList.indexOf("Speed")){
		if(i_column==1)
			return QVariant(m_robotPointer->getSpeed());
		else
			return QVariant(QString("Speed"));
	}
	
	else 	if(i_row==RobotItem::propertyList.indexOf("CurrentPositionX")){
		if(i_column==1)
			return QVariant(m_robotPointer->getPosition().x);
		else
			return QVariant(QString("Current x position"));
	}
	
	else 	if(i_row==RobotItem::propertyList.indexOf("CurrentPositionY")){
		if(i_column==1)
			return QVariant(m_robotPointer->getPosition().y);
		else
			return QVariant(QString("Current y position"));
	}
		
	return QVariant();
}


bool RobotItem::setPropertyData(int i_row,int i_column,const QVariant& i_data){

	if(i_column!=1)
		return false;

	if(i_row==RobotItem::propertyList.indexOf("Speed")){
		m_robotPointer->setSpeed(i_data.toFloat());
		return true;
	}
	
	else 	if(i_row==RobotItem::propertyList.indexOf("CurrentPositionX")){
		Point2D point=m_robotPointer->getPosition();
		point.x=i_data.toDouble();
		m_robotPointer->setPosition(point);
		return true;
	}	
	else 	if(i_row===RobotItem::propertyList.indexOf("CurrentPositionY")){
	Point2D point=m_robotPointer->getPosition();
		point.y=i_data.toDouble();
		m_robotPointer->setPosition(point);
		return true;
	}
	
	return false;
}


void RobotItem::setRobotPointer(RobotPointer i_robotPointer){
	m_robotPointer = i_robotPointer;
}


bool RobotItem::construct(){
  //add property childern here!
	LOG_DEBUG("Adding new Joint controller!");
	return addJointControllerItem();
}

bool RobotItem::addJointControllerItem(){
	JointControllerItem* child = new JointControllerItem(this,&m_robotPointer->getJointController());
	child->construct();
	return insertChild(0,child);
}
