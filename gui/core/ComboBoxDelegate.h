#include <QItemDelegate>

class QModelIndex;
class BaseRobotItem;
class ComboBoxDelegate : public QItemDelegate {
 public:
  ComboBoxDelegate(QObject* i_parent);

  virtual QWidget* createEditor(QWidget* i_parent,
				const QStyleOptionViewItem& i_option,
				const QModelIndex& i_index) const;

  virtual void setEditorData(QWidget *i_editor,
			     const QModelIndex& i_index) const;

  virtual void setModelData(QWidget* i_editor,
		    QAbstractItemModel *i_model,
		    const QModelIndex& i_index) const;

  virtual void updateEditorGeometry(QWidget* i_editor,
			    const QStyleOptionViewItem& i_option,
				    const QModelIndex& i_index) const;

 private:
  BaseRobotItem* getItem(const QModelIndex& i_index) const;
  bool useDelegate(const QModelIndex& i_index) const;
};
