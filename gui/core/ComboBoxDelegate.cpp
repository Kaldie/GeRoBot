#include <macroHeader.h>
#include "ComboBoxDelegate.h"
#include <QWidget>
#include <QComboBox>
#include "BaseRobotItem.h"
#include "RobotTreeModel.h"
#include "BaseRobotItem.h"

ComboBoxDelegate::ComboBoxDelegate(QObject* i_parent)
  : QItemDelegate(i_parent) {
}


QWidget* ComboBoxDelegate::createEditor(QWidget* i_parent,
          const QStyleOptionViewItem& i_option,
          const QModelIndex& i_index) const {
  if (!useDelegate(i_index)) {
    return QItemDelegate::createEditor(i_parent, i_option, i_index);
  }
  return new QComboBox(i_parent);
}


void ComboBoxDelegate::setEditorData(QWidget *i_editor, const QModelIndex& i_index) const {
  if (!useDelegate(i_index)) {
    return QItemDelegate::setEditorData(i_editor, i_index);
  }
  LOG_DEBUG("Getting elements of column: " << i_index.column() <<
      "and row" << i_index.row());
  QComboBox* comboBox = static_cast<QComboBox*>(i_editor);
  for (const auto itemName : getItem(i_index)->getComboBoxElements(i_index.row())) {
    comboBox->addItem(QString(itemName.c_str()));
  }
}


void ComboBoxDelegate::setModelData(QWidget* i_editor,
            QAbstractItemModel *i_model,
            const QModelIndex& i_index) const {
  if (!useDelegate(i_index)) {
    return QItemDelegate::setModelData(i_editor, i_model, i_index);
  }
  getItem(i_index)->setElement(i_index.row(), static_cast<QComboBox*>(i_editor)->currentIndex());
  emit i_model->dataChanged(i_index,i_index);
}


void ComboBoxDelegate::updateEditorGeometry(QWidget* i_editor,
              const QStyleOptionViewItem& i_option,
              const QModelIndex& i_index) const {
  if (!useDelegate(i_index)) {
    QItemDelegate::updateEditorGeometry(i_editor, i_option, i_index);
  }
  i_editor->setGeometry(i_option.rect);
}



BaseRobotItem* ComboBoxDelegate::getItem(const QModelIndex& i_index) const {
return static_cast<BaseRobotItem*>
  (static_cast<const RobotTreeModel*>(i_index.model())->getItem(i_index)->parent());
}


bool ComboBoxDelegate::useDelegate(const QModelIndex& i_index) const {
  BaseRobotItem* item = getItem(i_index);
  if (item) {
  return getItem(i_index)->useComboBoxDelegate(i_index.row());
  } else {
    false;
  }
}
