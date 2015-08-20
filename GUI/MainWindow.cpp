// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <QtGui>
#include <Robot.h>
#include <PinState.h>
#include <QFileDialog>
#include <RobotIO.h>
#include "./MainWindow.h"
#include "./core/RobotTreeModel.h"
#include "./core/RobotItem.h"
#include "./core/BasePropertyItem.h"
#include "./widget/RobotMovementWidget.h"
#include "./widget/Point2DWidget.h"
#include "./widget/tracedesign/TraceDesignWidget.h"
#include "./widget/tracedesign/TraceInfoWidget.h"


MainWindow::MainWindow(const Robot::RobotPointer& i_robot,
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

  Robot::RobotPointer robotPointer =  m_modelPointer->getRobotPointer();
  robotMovementTab->layout()->addWidget(
    new RobotMovementWidget(robotPointer, this));
  traceDesignTab->layout()->addWidget(new TraceDesignWidget(this));
  /* REMOVE, altough it give a nice thingy to look at!
  TraceInfoWidget* info = traceDesignTab->findChild<TraceInfoWidget*>();
  if (!info) {
    LOG_ERROR("fond no info");}
  Trace::TracePointer x(std::make_shared<Trace>());
  //LOG_DEBUG("Trace type: " << x->getTraceType());
  info->updateTrace(x);
  // END OF REMOVE!!! */
  return true;
}


bool MainWindow::saveRobot() {
  QString fileName = QFileDialog::getSaveFileName(this,
                                                  tr("Save Robot to XML"));
  RobotIO robotBuilder("defaultRobot.xml");
  //  m_modelPointer->getRobotPointer()
  robotBuilder.update(m_modelPointer->getRobotPointer());
  robotBuilder.store(fileName.toUtf8().constData());
  return true;
}
