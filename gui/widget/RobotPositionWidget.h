// Copyright [2014] Ruud Cools
#ifndef GUI_ROBOTPOSITIONWIDGET_H_
#define GUI_ROBOTPOSITIONWIDGET_H_

#include <macroHeader.h>
#include <QWidget>
#include <Point2D.h>

class RobotPositionWidget : public QWidget {
  Q_OBJECT

  GETSETPOINTER(Point2D, m_point, Point);

 public:
  explicit RobotPositionWidget(QWidget *parent = 0);

  RobotPositionWidget(Point2D* i_point,
                      QWidget *parent = 0);

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

 protected:
  void paintEvent(QPaintEvent *event);
};

#endif  // GUI_ROBOTPOSITIONWIDGET_H_

