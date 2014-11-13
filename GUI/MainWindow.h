#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "ui_MainWindow.h"

#include <macroHeader.h>

class QAction;
class QTreeView;
class QWidget;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

	GETSET(RobotTreeModelPointer,m_modelPointer,ModelPointer);
	
 public:
    MainWindow(const RobotPointer&,QWidget *parent = 0);

 public slots:
	 bool saveRobot();

 private slots:

};

#endif
