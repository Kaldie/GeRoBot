// Copyright [2014] Ruud Cools
#ifndef GUI_MAINWINDOW_H_
#define GUI_MAINWINDOW_H_

#include <QMainWindow>
#include <QModelIndex>
#include <macroHeader.h>
#include "./ui_MainWindow.h"
#include "./RobotMovementWidget.h"

class QAction;
class QTreeView;
class QWidget;

class MainWindow : public QMainWindow, private Ui::mainWindow {
  Q_OBJECT

  GETSET(RobotTreeModelPointer, m_modelPointer, ModelPointer);

 private:
  bool initialise();

 public:
  MainWindow(const RobotPointer&, QWidget *parent = 0);
  public slots:
      bool saveRobot();
  void resizeColumnsToContents(const QModelIndex& /*modelIndex*/);
};

#endif  // GUI_MAINWINDOW_H_
