// Copyright [2014] Ruud Cools
#ifndef GUI_ROBOTPOSITIONWIDGET_H_
#define GUI_ROBOTPOSITIONWIDGET_H_

#include <macroHeader.h>
#include <QWidget>
#include <Robot.h>

class RobotPositionWidget : public QWidget {
  Q_OBJECT
 private:
  typedef std::weak_ptr<Robot> WeakRobotPointer;
  GETSET(WeakRobotPointer, m_robot, RobotPointer);

 public:
  explicit RobotPositionWidget(QWidget *parent = 0);

  RobotPositionWidget(const Robot::RobotPointer& i_robot,
                      QWidget *parent = 0);

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

 protected:
  void paintEvent(QPaintEvent *event);
};

#endif  // GUI_ROBOTPOSITIONWIDGET_H_

