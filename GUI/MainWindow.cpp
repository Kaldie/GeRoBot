// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <QtGui>
#include <Robot.h>
#include <PinState.h>
#include <QFileDialog>
#include <RobotBuilder.h>
#include "./RobotMovementWidget.h"
#include "./MainWindow.h"
#include "./RobotTreeModel.h"
#include "./RobotItem.h"
#include "./BasePropertyItem.h"


MainWindow::MainWindow(const RobotPointer& i_robot,
                         QWidget* parent/*=0*/):
    QMainWindow(parent) {
  setupUi(this);

#ifdef Q_WS_MAEMO_5
  // Alternating row colors look bad on Maemo
  configurationView->setAlternatingRowColors(false);
#endif

  m_modelPointer.reset(new RobotTreeModel(i_robot));

  configurationView->setModel(m_modelPointer.get());

  initialise();
}


void MainWindow::resizeColumnsToContents(const QModelIndex& /*modelIndex*/) {
  for (int column = 0; column < m_modelPointer->columnCount(); ++column)
    configurationView->resizeColumnToContents(column);
}


bool MainWindow::initialise() {
  // exit file action
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
  // Save robot action
  connect(saveAction, SIGNAL(triggered()), this, SLOT(saveRobot()));

  // On expand addapt size of the columns
  connect(configurationView,
          SIGNAL(expanded(const QModelIndex& /*modelIndex*/)),
          this,
          SLOT(resizeColumnsToContents(const QModelIndex& /*modelIndex*/)));

  RobotPointer robotPointer =  m_modelPointer->getRobotPointer();
  robotMovementTab->layout()->addWidget(
      new RobotMovementWidget(robotPointer, this));

  return true;
}


bool MainWindow::saveRobot() {
  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Save Robot to XML"));
  RobotBuilder robotBuilder("defaultRobot.xml");
  //  m_modelPointer->getRobotPointer()
  robotBuilder.update(m_modelPointer->getRobotPointer());
  robotBuilder.store(fileName.toUtf8().constData());
  return true;
}
