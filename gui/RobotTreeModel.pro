 FORMS = \
   widget/Point2DWidget.ui \
   widget/tracedesign/TraceDesignWidget.ui \
   MainWindow.ui \
   widget/RobotMovementWidget.ui \

 RESOURCES   = RobotTreeModel.qrc

 HEADERS = \
    core/BaseActuatorItem.h \
    core/BaseJointItem.h \
    core/BaseMotorItem.h \
    core/BasePropertyItem.h \
    core/BaseRobotItem.h \
    core/JointControllerItem.h \
    core/PinStateItem.h \
    core/RobotItem.h \
    core/RobotTreeModel.h \
    widget/Point2DWidget.h \
    widget/RobotMovementWidget.h \
    widget/RobotPositionWidget.h \
    widget/tracedesign/TraceGraphItem.h \
    widget/tracedesign/RotationTraceGraphItem.h \
    widget/tracedesign/TraceGraphView.h \
    widget/tracedesign/TraceDesignWidget.h \
    widget/tracedesign/TraceInfoWidget.h \
    MainWindow.h



 SOURCES = \
    core/BaseActuatorItem.cpp \
    core/BaseJointItem.cpp \
    core/BaseMotorItem.cpp \
    core/BasePropertyItem.cpp \
    core/BaseRobotItem.cpp \
    core/JointControllerItem.cpp \
    core/PinStateItem.cpp \
    core/RobotItem.cpp \
    core/RobotTreeModel.cpp \
    widget/Point2DWidget.cpp \
    widget/RobotMovementWidget.cpp \
    widget/RobotPositionWidget.cpp \
    widget/tracedesign/TraceGraphItem.cpp \
    widget/tracedesign/RotationTraceGraphItem.cpp \
    widget/tracedesign/TraceGraphView.cpp \
    widget/tracedesign/TraceInfoWidget.cpp \
    widget/tracedesign/TraceDesignWidget.cpp \
    MainWindow.cpp \
    main.cpp

 CONFIG  += qt
 CONFIG += c++11

 QMAKE_CXXFLAGS += -Wfatal-errors

 QT += widgets
 INCLUDEPATH += /home/ruud/project/gerobot/include/

 LIBS += /home/ruud/project/gerobot/bin/lib/libGeRobot.so

 QMAKE_RPATHDIR += /home/ruud/project/gerobot/bin/lib/

 # install
 target.path = /home/ruud/projects/gerobot/GUI/output
 sources.files = $$FORMS $$HEADERS $$RESOURCES $$SOURCES *.pro *.txt
 sources.path = /home/ruud/projects/gerobot/GUI
 INSTALLS += target sources

 symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
 maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)