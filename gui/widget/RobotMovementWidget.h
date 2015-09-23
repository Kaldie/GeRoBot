 // Copyright [2014] Ruud Cools
#ifndef GUI_ROBOTMOVEMENTWIDGET_H_
#define GUI_ROBOTMOVEMENTWIDGET_H_
#include <macroHeader.h>
#include <QWidget>
#include <Robot.h>
#include "./ui_RobotMovementWidget.h"

class Point2D;
class QAction;
class QTreeView;
class QWidget;

class RobotMovementWidget: public QWidget, private Ui::RobotMovementWidget {
    Q_OBJECT
    GET(Robot::RobotPointer, m_robotPointer, RobotPointer)
    GETSET(GUIMovementMode, m_mode, MovementMode)
 public:
    // Constructor
    explicit RobotMovementWidget(QWidget* parent = 0);

    RobotMovementWidget(Robot::RobotPointer i_robotPointer,
                        QWidget *parent = 0);

    void setRobotPointer(Robot::RobotPointer i_robotPointer);

 private:
    void initialise();
    bool hasValidRobot();

  public slots:
      void updateSimulateRadioButtons();
      void updateMovementType(bool i_isSet);
      void updatePositionWidget();
      void movementUp();
      void movementExtent();
      void movementDown();
      void movementRetract();
      void movementLeft();
      void movementCounterClockWise();
      void movementRight();
      void movementClockWise();

 signals:
     void hasNewPosition();
};

#endif  // GUI_ROBOTMOVEMENTWIDGET_H_

