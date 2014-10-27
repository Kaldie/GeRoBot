 FORMS       = MainWindow.ui

 HEADERS     = BaseRobotItem.h          \
               BaseMotorItem.h          \
               PinStateItem.h           \
               BaseJointItem.h          \
               JointControllerItem.h    \
               RobotItem.h              \
               RobotTreeModel.h         \
               MainWindow.h

 RESOURCES   = RobotTreeResources.qrc

 SOURCES     = BaseRobotItem.cpp        \
               BaseMotorItem.cpp        \
               PinStateItem.cpp         \
               BaseJointItem.cpp        \
               JointControllerItem.cpp  \
               RobotItem.cpp            \
               RobotTreeModel.cpp       \
               MainWindow.cpp           \
               main.cpp

 CONFIG  += qt
 CONFIG += c++11

 QMAKE_CXXFLAGS += -Wfatal-errors

 QT += widgets

 INCLUDEPATH += /home/ruud/Dropbox/Projects/GeRoArm/include/

 LIBS += ../bin/lib/libArduinoInterface.so
 LIBS += ../bin/lib/libMathInterface.so
 LIBS += ../bin/lib/libMotorInterface.so
 LIBS += ../bin/lib/libPugiXML.so*
 LIBS += ../bin/lib/libRobotInterface.so*



 # install
 target.path = /home/ruud/Dropbox/Projects/GeRoArm/GUI/output
 sources.files = $$FORMS $$HEADERS $$RESOURCES $$SOURCES *.pro *.txt
 sources.path = /home/ruud/Dropbox/Projects/GeRoArm/GUI
 INSTALLS += target sources

 symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
 maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)
