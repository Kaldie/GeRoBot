#include <QtGui>

#include "MainWindow.h"
#include "RobotTreeModel.h"
#include "RobotItem.h"
#include "BasePropertyItem.h"
#include <Robot.h>

MainWindow::MainWindow(RobotPointer robot,QWidget* parent/*=0*/)
	: QMainWindow(parent)
 {
     setupUi(this);

 #ifdef Q_WS_MAEMO_5
     // Alternating row colors look bad on Maemo
     view->setAlternatingRowColors(false);
 #endif

		 BasePropertyItem* rootItem= new BasePropertyItem("root",0);
		 RobotItem* robotItem = new RobotItem(rootItem,robot);
		 RobotItem* robotItem2 = new RobotItem(rootItem,robot);
		 robotItem->construct();
		 robotItem2->construct();
		 rootItem->insertChild(0,robotItem);
		 rootItem->insertChild(0,robotItem2);
		 //		 robotItem->construct();
		 LOG_DEBUG(rootItem->parent());
     RobotTreeModel *model = new RobotTreeModel(rootItem);
		 
		 view->setModel(model);

		 /*     for (int column = 0; column < model->columnCount(); ++column)
         view->resizeColumnToContents(column);

     connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

     connect(view->selectionModel(),
             SIGNAL(selectionChanged(const QItemSelection &,
                                     const QItemSelection &)),
             this, SLOT(updateActions()));

     connect(actionsMenu, SIGNAL(aboutToShow()), this, SLOT(updateActions()));
     connect(insertRowAction, SIGNAL(triggered()), this, SLOT(insertRow()));
     connect(insertColumnAction, SIGNAL(triggered()), this, SLOT(insertColumn()));
     connect(removeRowAction, SIGNAL(triggered()), this, SLOT(removeRow()));
     connect(removeColumnAction, SIGNAL(triggered()), this, SLOT(removeColumn()));
     connect(insertChildAction, SIGNAL(triggered()), this, SLOT(insertChild()));

     updateActions();*/
 }

 void MainWindow::insertChild()
 {
	 /*     QModelIndex index = view->selectionModel()->currentIndex();
     QAbstractItemModel *model = view->model();

     if (model->columnCount(index) == 0) {
         if (!model->insertColumn(0, index))
             return;
     }

     if (!model->insertRow(0, index))
         return;

     for (int column = 0; column < model->columnCount(index); ++column) {
         QModelIndex child = model->index(0, column, index);
         model->setData(child, QVariant("[No data]"), Qt::EditRole);
         if (!model->headerData(column, Qt::Horizontal).isValid())
             model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"),
                                  Qt::EditRole);
     }

     view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                             QItemSelectionModel::ClearAndSelect);
																						 updateActions();
	 */
 }

 bool MainWindow::insertColumn(const QModelIndex &parent)
 {
   /*  QAbstractItemModel *model = view->model();
     int column = view->selectionModel()->currentIndex().column();

     // Insert a column in the parent item.
     bool changed = model->insertColumn(column + 1, parent);
     if (changed)
         model->setHeaderData(column + 1, Qt::Horizontal, QVariant("[No header]"),
                              Qt::EditRole);

     updateActions();

     return changed;
	 */
	 return false;
 }

 void MainWindow::insertRow()
 {
	 /*
		 QModelIndex index = view->selectionModel()->currentIndex();
     QAbstractItemModel *model = view->model();

     if (!model->insertRow(index.row()+1, index.parent()))
         return;

     updateActions();

     for (int column = 0; column < model->columnCount(index.parent()); ++column) {
         QModelIndex child = model->index(index.row()+1, column, index.parent());
         model->setData(child, QVariant("[No data]"), Qt::EditRole);
     }
	 */
 }

 bool MainWindow::removeColumn(const QModelIndex &parent)
 {
	 /*
     QAbstractItemModel *model = view->model();
     int column = view->selectionModel()->currentIndex().column();

     // Insert columns in each child of the parent item.
     bool changed = model->removeColumn(column, parent);

     if (!parent.isValid() && changed)
         updateActions();

     return changed;
	 */
     return false;
 }

 void MainWindow::removeRow()
 {
	 /*
     QModelIndex index = view->selectionModel()->currentIndex();
     QAbstractItemModel *model = view->model();
     if (model->removeRow(index.row(), index.parent()))
         updateActions();
	 */
 }

 void MainWindow::updateActions()
 {
	 /*
     bool hasSelection = !view->selectionModel()->selection().isEmpty();
     removeRowAction->setEnabled(hasSelection);
     removeColumnAction->setEnabled(hasSelection);

     bool hasCurrent = view->selectionModel()->currentIndex().isValid();
     insertRowAction->setEnabled(hasCurrent);
     insertColumnAction->setEnabled(hasCurrent);

     if (hasCurrent) {
         view->closePersistentEditor(view->selectionModel()->currentIndex());

         int row = view->selectionModel()->currentIndex().row();
         int column = view->selectionModel()->currentIndex().column();
         if (view->selectionModel()->currentIndex().parent().isValid())
             statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
         else
             statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
     }
	 */
 }