#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "RobotItem.h"

#include <QMainWindow>
#include <QModelIndex>
#include "ui_MainWindow.h"

class QAction;
class QTreeView;
class QWidget;



class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
	
 public:
	MainWindow(RobotPointer,QWidget *parent = 0);
		 
 public slots:
	 void updateActions();
		
 private slots:
	 void insertChild();
	 bool insertColumn(const QModelIndex &parent = QModelIndex());
	 void insertRow();
	 bool removeColumn(const QModelIndex &parent = QModelIndex());
	 void removeRow();
};

#endif
