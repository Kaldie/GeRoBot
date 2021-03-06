// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <QtGui>
#include <QFile>
#include <RobotIO.h>
#include "./MainWindow.h"

int main(int argc, char *argv[]) {
  Q_INIT_RESOURCE(RobotTreeModel);
  QFile xmlFile(":/defaultRobot.xml");
  RobotIO robotBuilder(&xmlFile);
  robotBuilder.build();
  LOG_DEBUG("Robot build finished!");
  QApplication app(argc, argv);
  MainWindow window(robotBuilder.getRobotPointer());

#if defined(Q_OS_SYMBIAN)
  window.showMaximized();
#else
  window.show();
#endif
  return app.exec();
}
