 // Copyright [2014] Ruud Cools
#ifndef GUI_ROBOTMOVEMENTWIDGET_H_
#define GUI_ROBOTMOVEMENTWIDGET_H_

#include <QMainWindow>
#include <QWidget>
#include <Point2D.h>
#include <macroHeader.h>
#include "./ui_RobotMovementWidget.h"

class QAction;
class QTreeView;
class QWidget;

class RobotMovementWidget: public QWidget, private Ui::RobotMovementWidget {
    Q_OBJECT

    GET(RobotPointer, m_robotPointer, RobotPointer)
    GETSETPOINTER(Point2D, m_point2DPointer, Point)
    GETSET(GUIMovementMode, m_mode, MovementMode)

 public:
    // Constructor
    explicit RobotMovementWidget(QWidget* parent = 0);

    RobotMovementWidget(RobotPointer i_robotPointer,
                        QWidget *parent = 0);

    void setRobotPointer(RobotPointer i_robotPointer);

    ~RobotMovementWidget();

 private:
    void initialise();
    bool hasValidConnection();

  public slots:
      void updateSimulateRadioButtons();
      void updateMovementType(bool i_isSet);
      void updatePositionWidget();
      void movementUp();
      void movementDown();
      void movementLeft();
      void movementRight();

 signals:
     void hasNewPosition();
};

#endif  // GUI_ROBOTMOVEMENTWIDGET_H_

