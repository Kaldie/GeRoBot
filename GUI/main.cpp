// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <QtGui>
#include <RobotBuilder.h>
#include "./MainWindow.h"

int main(int argc, char *argv[]) {
  Q_INIT_RESOURCE(RobotTreeModel);

  RobotBuilder robotBuilder("defaultRobot.xml");
  robotBuilder.build();

  QApplication app(argc, argv);
  MainWindow window(robotBuilder.getRobotPointer());

#if defined(Q_OS_SYMBIAN)
  window.showMaximized();
#else
  window.show();
#endif
  return app.exec();
}
