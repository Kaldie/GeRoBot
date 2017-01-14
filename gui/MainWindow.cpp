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
#include "./widget/RobotMovementWidget.h"
#include "./widget/Point2DWidget.h"
#include "./widget/tracedesign/TraceDesignWidget.h"
#include "./widget/tracedesign/TraceInfoWidget.h"


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
  RobotMovementWidget* robotMovementWidget =
    new RobotMovementWidget(m_robotPointer, this);
  robotMovementTab->layout()->addWidget(robotMovementWidget);

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
  // on switch the tabes, update the positition widget
  connect(m_modelPointer.get(), &QAbstractItemModel::dataChanged,
          robotMovementWidget, &RobotMovementWidget::updateFromConfiguration);
  connect(tabWidget, SIGNAL(currentChanged(int)),
    robotMovementWidget, SLOT(updatePositionWidget()));
  return true;
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
