#ifndef RobotTreeItem_H
#define RobotTreeItem_H

#include <QList>
#include <QVariant>
#include <QString>

/* 
	 Robot Tree Item
	 This item will be derived to represent all parts and relating properties of a robot.
	 A parent is in charge of setting and getting the property of a part.
	 A child wil visualize a property of a part.
     
*/
	 
class SinglePropertyItem : public RobotParentItem 
{
 public:
	SinglePropertyItem(const QString& i_itemName, RobotParentItem* parent = 0);
  virtual ~SinglePropertyItem();

	// No inserting (and thus no need for) deleting childern
	virtual bool insertChildren(int position, int count, QString i_childName) {return false;};
	virtual bool removeChildren(int position, int count){return false;};
	/*
		Short list of methods inherented from RobotParentItem
				
		//	int columnCount() const {return 2;};
		//	bool insertColumns(int, int){return false;};
		//	bool removeColumns(int, int){return false;};
		//	bool setName(const QString&);
		//	const QString& getName(){return m_elementName;};
		//	virtual QVariant data(int index) const;
		//  virtual bool setData(int index, const QVariant &value);
		// 	RobotParentItem* parent();
  */
	virutal int childCount() const {return 0};
	virtual int childNumber() const {return 0};

 protected:
	// Derived classes should only implement the protected methods
	// return not succeded or empty variable
	virtual const QVariant getChildData(const SinglePropertyItem* child) const {return QVariant();};
	virtual bool setChildData(const SinglePropertyItem* child,QVariant& o_data) {return false;};
};

#endif
