// Copyright [2014] Ruud Cools
#ifndef GUI_MAINWINDOW_H_
#define GUI_MAINWINDOW_H_

#include <QMainWindow>
#include <QModelIndex>
#include <macroHeader.h>
#include "./core/RobotTreeModel.h"
#include "./core/ComboBoxDelegate.h"
#include "./ui_MainWindow.h"
#include "./widget/RobotMovementWidget.h"

class QAction;
class QTreeView;

class MainWindow: public QMainWindow, private Ui::mainWindow {
  Q_OBJECT
  GETSET(std::shared_ptr<Robot>, m_robotPointer, RobotPointer);
  GETSET(RobotTreeModel::RobotTreeModelPointer, m_modelPointer, ModelPointer);
 private:
  bool initialise();
  ComboBoxDelegate m_comboBoxDelegate;

 public:
  MainWindow(const Robot::RobotPointer&, QWidget *parent = 0);
 public slots:
  bool saveRobot();
  void resizeColumnsToContents(const QModelIndex& /*modelIndex*/);
  bool saveTraceDesign();
  bool loadTraceDesign();
  bool clearTraceDesign();
  bool calculateTraces();
  bool actuateRobot();
};

#endif  // GUI_MAINWINDOW_H_
