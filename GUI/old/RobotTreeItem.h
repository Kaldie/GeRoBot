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
	 
class SinglePropertyObject
{
 public:
	SinglePropertyObject(const QString& i_itemName, SinglePropertyObject *parent = 0);
  virtual ~SinglePropertyObject();
	
	SinglePropertyObject* child(int number);

	bool insertChildren(int position, int count, QString i_childName);
	bool removeChildren(int position, int count);
	int childCount() const;
	int childNumber() const;

	int columnCount() const {return 2;};
	bool insertColumns(int position, int columns){return false;};
	bool removeColumns(int position, int columns){return false;};

	SinglePropertyObject* parent();
	bool setName(const QString&);

	/* 
		 Data is always stored in the parrent. 
		 Using index the correlating property of the robot data is found in the corrispondig 
		 This means that a certain type of RobotTree is always build the same order.
		 By definition we declare that first the properties of a object are defined and 
		 later the othe subparts of the object
		 
		 Therefor we can cast a void pointer to the corisponding type.
		 The relation between types and childern of a parrent is kept within ChildPointerTypeMap<SinglePropertyObject*,Type>
		 
		 The index of the child is used to relate different childeren to properties of a robot part.
	*/
  virtual QVariant data(int index) const;
	virtual bool setData(int index, const QVariant &value);
	//  virtual bool setPropertyPointer(QVariant*);
	
	const QString& getName(){return m_elementName;};

 protected:
	//Derived classes should only implement the protected methods
	const QVariant getChildDate(const SinglePropertyObject* child)=0;
	setChildData(const SinglePropertyObject* child,QVariant& o_data)=0;

 private:
	QList<SinglePropertyObject*> childItems;
	SinglePropertyObject *parentItem;
	QString m_elementName;        // Name of the element, which can be the property name or the robot part name
};

#endif
