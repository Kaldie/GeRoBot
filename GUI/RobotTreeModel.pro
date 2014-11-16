 FORMS       = MainWindow.ui            \
               RobotMovementWidget.ui

 HEADERS     = BaseRobotItem.h          \
               BaseMotorItem.h          \
               PinStateItem.h           \
               BaseJointItem.h          \
               JointControllerItem.h    \
               BaseActuatorItem.h       \
               RobotItem.h              \
               RobotTreeModel.h         \
               BasePropertyItem.h       \
               RobotMovementWidget.h    \
               RobotPositionWidget.h    \
               MainWindow.h

 RESOURCES   = RobotTreeModel.qrc

 SOURCES     = BaseRobotItem.cpp        \
               BasePropertyItem.cpp     \
               RobotItem.cpp            \
               JointControllerItem.cpp  \
               BaseActuatorItem.cpp     \
               BaseMotorItem.cpp        \
               PinStateItem.cpp         \
               BaseJointItem.cpp        \
               RobotTreeModel.cpp       \
               RobotMovementWidget.cpp  \
               RobotPositionWidget.cpp  \
               MainWindow.cpp           \
               main.cpp

 CONFIG  += qt
 CONFIG += c++11

 QMAKE_CXXFLAGS += -Wfatal-errors

 QT += widgets

 INCLUDEPATH += /home/ruud/Dropbox/Projects/GeRoArm/include/

 LIBS += /home/ruud/Projects/GeRoArm/bin/lib/libArduinoInterface.so
 LIBS += /home/ruud/Projects/GeRoArm/bin/lib/libMathInterface.so
 LIBS += /home/ruud/Projects/GeRoArm/bin/lib/libMotorInterface.so
 LIBS += /home/ruud/Projects/GeRoArm/bin/lib/libPugiXML.so
 LIBS += /home/ruud/Projects/GeRoArm/bin/lib/libRobotInterface.so

 QMAKE_RPATHDIR += /home/ruud/Projects/GeRoArm/bin/lib/

 # install
 target.path = /home/ruud/Dropbox/Projects/GeRoArm/GUI/output
 sources.files = $$FORMS $$HEADERS $$RESOURCES $$SOURCES *.pro *.txt
 sources.path = /home/ruud/Dropbox/Projects/GeRoArm/GUI
 INSTALLS += target sources

 symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
 maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)
