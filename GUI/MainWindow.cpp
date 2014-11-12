#include <QtGui>


#include "MainWindow.h"
#include "RobotTreeModel.h"
#include "RobotItem.h"
#include "BasePropertyItem.h"
#include <Robot.h>
#include <PinState.h>
#include <QFileDialog>
#include <RobotBuilder.h>


MainWindow::MainWindow(const RobotPointer& i_robot,QWidget* parent/*=0*/)
	: QMainWindow(parent)
{
	setupUi(this);
	
#ifdef Q_WS_MAEMO_5
	// Alternating row colors look bad on Maemo
	view->setAlternatingRowColors(false);
#endif
	
 	m_modelPointer.reset(new RobotTreeModel(i_robot));
	
	view->setModel(m_modelPointer.get());
	
	for (int column = 0; column < m_modelPointer->columnCount(); ++column)
		view->resizeColumnToContents(column);
	
	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(saveAction,SIGNAL(triggered()),this,SLOT(saveRobot()));
}


bool MainWindow::saveRobot(){

	QString fileName=QFileDialog::getSaveFileName(this,
																								tr("Save Robot to XML"));
	
	RobotBuilder robotBuilder("defaultRobot.xml");
	RobotPointer m_modelPointer->getRobotPointer()
	robotBuilder.update(m_modelPointer->getRobotPointer());
	robotBuilder.store(fileName.toUtf8().constData());
	return true;
}
