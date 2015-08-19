// Copyright [2014] Ruud Cools
#ifndef GUI_TRACEINFOWIDGET_H_
#define GUI_TRACEINFOWIDGET_H_

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <TraceListIO.h>

class Point2DWidget;

class TraceInfoWidget: public QWidget {
  Q_OBJECT
  Point2DWidget* startPoint = 0;
  Point2DWidget* endPoint = 0;
  Point2DWidget* centralPoint = 0;

  QComboBox* traceTypeComboBox = 0;
  QLabel* traceTypeLabel = 0;

 private:
  TracePointer m_trace;
  void initialise();

 public:
  // Constructor
  explicit TraceInfoWidget(QWidget* parent = 0);

  void trace(TracePointer i_tracePointer);

 public slots:
  void updateTrace();

#endif  // GUI_TRACEINFOWIDGET_H_
