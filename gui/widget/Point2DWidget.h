// Copyright [2014] Ruud Cools
#ifndef GUI_WIDGET_POINT2DWIDGET_H_
#define GUI_WIDGET_POINT2DWIDGET_H_

#include <Point2D.h>
#include <QWidget>
#include "./ui_Point2DWidget.h"


class Point2DWidget: public QWidget, private Ui::Point2DWidget {
  Q_OBJECT
  GETSETPOINTER(Point2D, m_pointPointer, Point);
  GETSET(bool, m_isValid, Valid);

 public:
  // Constructor
  explicit Point2DWidget(QWidget* parent = 0);

  Point2DWidget(Point2D* i_pointer,
                QWidget *parent = 0);
  void setPointName(const QString& i_pointName);

 private:
  void initialise();
  static const int m_precision = 15;

 private slots:
   void updatePoint();

 public slots:
   void update();

 signals:
   void hasNewPosition();
};
#endif  // GUI_WIDGET_POINT2DWIDGET_H_
