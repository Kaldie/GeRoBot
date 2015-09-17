// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <QtGui>
#include <RobotIO.h>
#include "./MainWindow.h"

int main(int argc, char *argv[]) {
  Q_INIT_RESOURCE(RobotTreeModel);

  RobotIO robotBuilder("/home/ruud/project/gerobot/gui/defaultRobot.xml");
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
