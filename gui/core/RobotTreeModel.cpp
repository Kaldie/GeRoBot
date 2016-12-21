// Copyright [2014] Ruud Cools
#include <QtGui>
#include <macroHeader.h>
#include "./BaseRobotItem.h"
#include "./BasePropertyItem.h"
#include "./RobotItem.h"
#include "./RobotTreeModel.h"

RobotTreeModel::RobotTreeModel(const Robot::RobotPointer& i_robotPointer,
                               QObject *parent /*=0*/)
    : QAbstractItemModel(parent) {
  m_headerData << "Property name" << "Value";
  rootItem = new BasePropertyItem("root", 0);
  RobotItem* robotItem = new RobotItem(rootItem, i_robotPointer);
  robotItem->construct();
  rootItem->insertChild(0, robotItem);
  // robotItem->construct();
  LOG_DEBUG(rootItem->parent());
}


RobotTreeModel::~RobotTreeModel() {
  delete rootItem;
}


int RobotTreeModel::columnCount(const QModelIndex & /* parent */) const {
  return rootItem->columnCount();
}


QVariant RobotTreeModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole && role != Qt::EditRole)
    return QVariant();

  BaseRobotItem *item = getItem(index);

  return item->data(index.row(), index.column());
}


Qt::ItemFlags RobotTreeModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return 0;

  if(index.column() == 0)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

  if(index.column() == 1)
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;

  return 0;
}


BaseRobotItem* RobotTreeModel::getItem(const QModelIndex &index) const {
  if (index.isValid()) {
    BaseRobotItem *item = static_cast<BaseRobotItem*>(index.internalPointer());
    if (item) return item;
  }
  return rootItem;
}


QVariant RobotTreeModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return m_headerData[section];

  return QVariant();
}


QModelIndex RobotTreeModel::index(int row, int column, const QModelIndex &parent) const {
  if (parent.isValid() && parent.column() != 0)
    return QModelIndex();

  BaseRobotItem *parentItem = getItem(parent);

  BaseRobotItem *childItem = parentItem->child(row);

  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}


bool RobotTreeModel::insertColumns(int, int, const QModelIndex&) {
  /*	bool success;

  beginInsertColumns(parent, position, position + columns - 1);
  success = rootItem->insertColumns(position, columns);
  endInsertColumns();
  */
  return false;
}


bool RobotTreeModel::insertRows(int, int,
                                const QModelIndex&) {
  return false;
  /*
    BaseRobotItem *parentItem = getItem(parent);
    QString childName=parentItem->getName() + "_child";
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows,childName);
    endInsertRows();

    return success;
  */
}


QModelIndex RobotTreeModel::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return QModelIndex();

  BaseRobotItem *childItem = getItem(index);
  BaseRobotItem *parentItem = childItem->parent();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->childNumber(), 0, parentItem);
}


bool RobotTreeModel::removeColumns(int, int, const QModelIndex&) {
  /*
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
    removeRows(0, rowCount());
  */
  return false;
}


bool RobotTreeModel::removeRows(int, int, const QModelIndex&) {
  return false;
  /*
    BaseRobotItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
  */
}


int RobotTreeModel::rowCount(const QModelIndex &parent) const {
  BaseRobotItem *parentItem = getItem(parent);

  return parentItem->rowCount();
}


bool RobotTreeModel::setData(const QModelIndex &index, const QVariant &value,
                             int role) {
  if (role != Qt::EditRole)
    return false;

  BaseRobotItem *item = getItem(index);
  bool result = item->setData(index.row(), index.column(), value);

  if (result) {
    LOG_DEBUG("Emit data changed");
    emit dataChanged(index, index);
  }
  return result;
}

bool RobotTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                                   const QVariant &value, int role) {
  if (role != Qt::EditRole || orientation != Qt::Horizontal)
    return false;

  bool result(false);

  if (section > 1 && m_headerData.size()-1 > section) {
    m_headerData[section]=value.toString();
    result = true;
  }

  if (result)
    emit headerDataChanged(orientation, section, section);

  return result;
}


Robot::RobotPointer RobotTreeModel::getRobotPointer() {
  return static_cast<RobotItem*>(
      rootItem->getChildItems()[0])->getRobotPointer();
}
