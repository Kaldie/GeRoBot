// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <QtGui>
#include <Robot.h>
#include <PinState.h>
#include <QFileDialog>
#include <RobotIO.h>
#include <TraceListIO.h>
#include <QtWidgets/QTreeView>
#include <QAbstractItemModel>
#include "./MainWindow.h"
#include "./core/RobotTreeModel.h"
#include "./core/RobotItem.h"
#include "./core/BasePropertyItem.h"
#include "./widget/ManualMovementWidget.h"
#include "./widget/RobotMovementWidget.h"
#include "./widget/Point2DWidget.h"
#include "./widget/tracedesign/TraceDesignWidget.h"
#include "./widget/tracedesign/TraceInfoWidget.h"
#include "./widget/calibrations/JointCalibrationWidget.h"


MainWindow::MainWindow(const Robot::RobotPointer& i_robot,
                        QWidget* parent/*=0*/)
  : QMainWindow(parent),
    m_robotPointer(i_robot),
    m_comboBoxDelegate(parent) {
  setupUi(this);

#ifdef Q_WS_MAEMO_5
  // Alternating row colors look bad on Maemo
  configurationView->setAlternatingRowColors(false);
#endif

  m_modelPointer.reset(new RobotTreeModel(m_robotPointer));
  configurationView->setModel(m_modelPointer.get());
  configurationView->setItemDelegateForColumn(1, &m_comboBoxDelegate);
  initialise();
}


void MainWindow::resizeColumnsToContents(const QModelIndex& /*modelIndex*/) {
  for (int column = 0; column < m_modelPointer->columnCount(); ++column)
    configurationView->resizeColumnToContents(column);
}


bool MainWindow::initialise() {
  // Make and set the Robot movement widget
  ManualMovementWidget* manualMovementWidget =
    new ManualMovementWidget(m_robotPointer,this);
  movementTab->layout()->addWidget(manualMovementWidget);
  // get the robot movement widget to connect the model::dataChanged with updateFromConfig
  RobotMovementWidget* robotMovementWidget =
      manualMovementWidget->findChild<RobotMovementWidget*>();
  if (robotMovementWidget == nullptr) {
    LOG_ERROR("Bad");
  }
  connect(m_modelPointer.get(), &QAbstractItemModel::dataChanged,
          robotMovementWidget, &RobotMovementWidget::updateFromConfiguration);


  // Create and add the joint calibration widget
  JointCalibrationWidget* jointCalibrationWidget =
    new JointCalibrationWidget(m_robotPointer,this);
  calibrationTab->layout()->addWidget(jointCalibrationWidget);

  // Make and set the trace design widget
  TraceDesignWidget* traceDesignWidget = new TraceDesignWidget(m_robotPointer, this);
  traceDesignTab->layout()->addWidget(traceDesignWidget);

  // exit file action
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
  // Save robot action
  connect(saveAction, SIGNAL(triggered()), this, SLOT(saveRobot()));
  // Save trace design action
  connect(saveTraceAction, SIGNAL(triggered()), this, SLOT(saveTraceDesign()));
  // Save trace design action
  connect(loadTraceAction, SIGNAL(triggered()), this, SLOT(loadTraceDesign()));
  // Clear trace design action
  connect(clearTraceAction, SIGNAL(triggered()), this , SLOT(clearTraceDesign()));
  // Calculate traces
  connect(calculateTraceAction, SIGNAL(triggered()), traceDesignWidget, SLOT(calculateTraces()));
  // Make the calculated shizzle shizzle
  connect(actuateAction, SIGNAL(triggered()), this, SLOT(actuateRobot()));
  // On expand addapt size of the columns
  connect(configurationView,
          SIGNAL(expanded(const QModelIndex& /*modelIndex*/)),
          this,
          SLOT(resizeColumnsToContents(const QModelIndex& /*modelIndex*/)));

  // connect the has message from the calibration widget to the update status bar
  connect(jointCalibrationWidget, SIGNAL(hasMessage(const QString&)), this, SLOT(UpdateStatusBar(const QString&)));
  return true;
}


void MainWindow::UpdateStatusBar(const QString& i_message) {
  statusbar->showMessage(i_message);
}


bool MainWindow::saveRobot() {
  QString fileName =
    QFileDialog::getSaveFileName(this,
                                 tr("Save Robot to XML"));
  QFile qFile(":/defaultRobot.xml");
  RobotIO robotBuilder(&qFile);
  //  m_modelPointer->getRobotPointer()
  robotBuilder.update(m_modelPointer->getRobotPointer());
  robotBuilder.store(fileName.toUtf8().constData());
  return true;
}


bool MainWindow::saveTraceDesign() {
 QString fileName =
   QFileDialog::getSaveFileName(this,
                                tr("Save Trace Design to XML"));
  QFile qFile(":/defaultTraceDesign.xml");
  TraceListIO traceListIO(&qFile);
  LOG_DEBUG("Storing trace design at: " << fileName.toStdString());
  if (TraceDesignWidget* traceWidget = findChild<TraceDesignWidget*>()) {
    LOG_DEBUG("Updating and saving!");
    Trace::TracePointerVector vector = traceWidget->getVector();
    traceListIO.update(vector);
    traceListIO.store(fileName.toStdString());
  } else {
    LOG_DEBUG(traceWidget);
    return false;
  }
  return true;
}


bool MainWindow::loadTraceDesign() {
 QString fileName =
   QFileDialog::getOpenFileName(this,
                                tr("Load Trace Design to XML"),
                                "",
                                "XML files (*.xml)");
 if (fileName.isNull()) {
   return false;
 }
 LOG_DEBUG("Selected file to load: " << fileName.toStdString());
 TraceListIO traceListIO(fileName.toStdString());
  traceListIO.build();
  if (TraceDesignWidget* traceWidget = findChild<TraceDesignWidget*>()) {
    traceWidget->resetWidget(traceListIO.getVector());
  } else {
    LOG_DEBUG(traceWidget);
    return false;
  }
  return true;
}


bool MainWindow::clearTraceDesign() {
 if (TraceDesignWidget* traceWidget = findChild<TraceDesignWidget*>()) {
   traceWidget->clearWidget();
   return true;
  } else {
   LOG_DEBUG(traceWidget);
   return false;
  }
}


bool MainWindow::actuateRobot() {
  m_robotPointer->actuate();
}
