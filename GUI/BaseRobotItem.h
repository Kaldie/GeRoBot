#ifndef BaseRobotItem_H
#define BaseRobotItem_H

#include <macroHeader.h>

#include <QList>
#include <QVariant>
#include <QString>

/* 
	 Robot Parent Item
	 This item will be derived to represent all parts and relating properties of a robot.
	 A parent is in charge of setting and getting the property of a part.
	 calls to data should be relayed to the specific properties of the containing part

	 If a property cannot be described in 2 columns, where the first column is the name of the property, your skrewed!     
*/
	 
class BaseRobotItem
{
    GETSETPOINTER(BaseRobotItem,m_parentItem,ParentItem)
    GETSET(QString,m_elementName,ElementName)

    GETSET(int,m_numberOfPropeties,NumberOfProperties)
    GETSET(QList<BaseRobotItem*>,m_childItems,ChildItems)
	
 public:
	BaseRobotItem(const QString& i_itemName, BaseRobotItem *parent = 0);
  virtual ~BaseRobotItem();
	
	BaseRobotItem* child(int number);

	bool insertChild(int position, BaseRobotItem* i_child);
	bool removeChildren(int position, int count);

	int childCount() const;
	int childNumber() const;
	
	//Number of rows is given by the name of the object and the properties it represents.
	virtual int rowCount();
	
	//normaly there will be only 2 columns
	virtual int columnCount(){return 2;};

	BaseRobotItem* parent();
	bool setName(const QString&);
	
	//responsible for setting and getting the element name.
    virtual QVariant data(int row,int column) const;
	virtual bool setData(int row,int column, const QVariant &value);

	virtual bool construct() = 0;
};

#endif
