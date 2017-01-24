 FORMS = \
   widget/Point2DWidget.ui \
   widget/tracedesign/TraceDesignWidget.ui \
   MainWindow.ui \
   widget/RobotMovementWidget.ui \
   widget/calibrations/JointCalibrationWidget.ui \
   widget/calibrations/CalibrationWidget.ui \
   widget/ManualMovementWidget.ui

 RESOURCES   = RobotTreeModel.qrc

 HEADERS = \
    core/ComboBoxDelegate.h \
    core/BaseActuatorItem.h \
    core/BaseJointItem.h \
    core/BaseMotorItem.h \
    core/BasePropertyItem.h \
    core/BaseRobotItem.h \
    core/JointControllerItem.h \
    core/PinStateItem.h \
    core/RobotItem.h \
    core/SpeedControllerItem.h \
    core/EndStopItem.h \
    core/RobotTreeModel.h \
    widget/Point2DWidget.h \
    widget/RobotMovementWidget.h \
    widget/RobotPositionWidget.h \
    widget/ManualMovementWidget.h \
    widget/tracedesign/TraceGraphPoint.h \
    widget/tracedesign/TraceGraphItem.h \
    widget/tracedesign/RotationTraceGraphItem.h \
    widget/tracedesign/TraceGraphView.h \
    widget/tracedesign/TraceDesignWidget.h \
    widget/tracedesign/TraceInfoWidget.h \
    widget/tracedesign/RobotLocator.h \
    widget/calibrations/CalibrationWidgetFactory.h \
    widget/calibrations/JointCalibrationWidget.h \
    widget/calibrations/CalibrationWidget.h \
    widget/calibrations/EndStopCalibrationWidget.h \
    widget/calibrations/PointCalibrationWidget.h \
    MainWindow.h


 SOURCES = \
    core/ComboBoxDelegate.cpp \
    core/BaseActuatorItem.cpp \
    core/BaseJointItem.cpp \
    core/BaseMotorItem.cpp \
    core/BasePropertyItem.cpp \
    core/BaseRobotItem.cpp \
    core/JointControllerItem.cpp \
    core/PinStateItem.cpp \
    core/RobotItem.cpp \
    core/SpeedControllerItem.cpp \
    core/EndStopItem.cpp \
    core/RobotTreeModel.cpp \
    widget/Point2DWidget.cpp \
    widget/RobotMovementWidget.cpp \
    widget/RobotPositionWidget.cpp \
    widget/ManualMovementWidget.cpp \
    widget/tracedesign/TraceGraphPoint.cpp \
    widget/tracedesign/TraceGraphItem.cpp \
    widget/tracedesign/RotationTraceGraphItem.cpp \
    widget/tracedesign/TraceGraphView.cpp \
    widget/tracedesign/TraceInfoWidget.cpp \
    widget/tracedesign/TraceDesignWidget.cpp \
    widget/tracedesign/RobotLocator.cpp \
    widget/calibrations/CalibrationWidget.cpp \
    widget/calibrations/CalibrationWidgetFactory.cpp \
    widget/calibrations/JointCalibrationWidget.cpp \
    widget/calibrations/EndStopCalibrationWidget.cpp \
    widget/calibrations/PointCalibrationWidget.cpp \
    MainWindow.cpp \
    main.cpp

    
 CONFIG  += qt
 CONFIG += c++11

 QMAKE_CXXFLAGS += -Wfatal-errors -DQT
# QMAKE_CXXFLAGS += -H

 QT += widgets
 INCLUDEPATH += /home/ruud/projects/gerobot/include/
 LIBS += /home/ruud/projects/gerobot/bin/lib/libGeRobot.so

 # install
 target.path = /home/ruud/projects/gerobot/GUI/output
 sources.files = $$FORMS $$HEADERS $$RESOURCES $$SOURCES *.pro *.txt
 sources.path = /home/ruud/projects/gerobot/GUI
 INSTALLS += target sources

 symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
 maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)
