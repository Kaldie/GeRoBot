#ifndef BaseRobotItem_H
#define BaseRobotItem_H

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
class BaseRobotItem {
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
  virtual int columnCount(){return 2;}

  BaseRobotItem* parent();
  bool setName(const QString&);

  //Create childeren from a QList of QStrings
  bool createChilderen(const QList<QString>& i_propertyList);

  //responsible for setting and getting the element name.
  virtual QVariant data(int row,int column) const;
  virtual bool setData(int row,int column, const QVariant &value);

  /**
   *  brief: Preforming all the stuff needed to show a valid/ full item.
   *  Including making child elements
   */
  virtual bool construct() = 0;
  virtual QVariant getPropertyData(int row,int column) const =0;
  virtual bool setPropertyData(int row,int column, const QVariant &value) =0;

  /**
   * It is possible to use a costom delegate, in our case this is a combo box
   * If an item can use it as a setter
   * make the next method return true
   */
  virtual bool useComboBoxDelegate(const int& i_row) const {return false;};

  /**
   * Returns string which are called in order to populate the combo box
   * The order is important. The set element method will return an index for one of the given items
   */
  virtual std::vector<std::string> getComboBoxElements(const int& i_row) const {return std::vector<std::string>();};

  /**
  * Will be called when the custom delegate is done editing
  * i_selectedItem is the index selected in the combo box
  * This method should make sure the info from the combo is set in the element
  */
  virtual void setElement(const int& i_row, const int& i_selectedItem) {};
};

#endif
