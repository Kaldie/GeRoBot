#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <Robot.h>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class BaseRobotItem;

class RobotTreeModel : public QAbstractItemModel {

 public:
  typedef std::shared_ptr<RobotTreeModel> RobotTreeModelPointer;
  RobotTreeModel(const Robot::RobotPointer& i_robotPointer,
                 QObject *parent = 0);

  ~RobotTreeModel();

  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;

  QModelIndex parent(const QModelIndex &index) const;

  int rowCount(const QModelIndex &parent = QModelIndex()) const;

  int columnCount(const QModelIndex &parent = QModelIndex()) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;

  QVariant data(const QModelIndex &index, int role) const;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;

  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole);

  bool setHeaderData(int section, Qt::Orientation orientation,
                     const QVariant &value, int role = Qt::EditRole);

  bool insertColumns(int position, int columns,
                     const QModelIndex &parent = QModelIndex());

  bool removeColumns(int position, int columns,
                     const QModelIndex &parent = QModelIndex());

  bool insertRows(int position, int rows,
                  const QModelIndex &parent = QModelIndex());

  bool removeRows(int position, int rows,
                  const QModelIndex &parent = QModelIndex());

  BaseRobotItem* getItem(const QModelIndex &index) const;

  Robot::RobotPointer getRobotPointer();

 private:
  void setupModelData(const QStringList &lines, BaseRobotItem *parent);

  BaseRobotItem *rootItem;

  QStringList m_headerData;
};

#endif
