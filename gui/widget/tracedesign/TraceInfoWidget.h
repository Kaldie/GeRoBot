// Copyright [2014] Ruud Cools
#ifndef GUI_TRACEINFOWIDGET_H_
#define GUI_TRACEINFOWIDGET_H_

#include <QWidget>
#include <Trace.h>

class Point2DWidget;
class QComboBox;
class QLabel;


class TraceInfoWidget: public QWidget {
  Q_OBJECT
  GETSET(Trace::WeakTracePointer, m_trace, WeakTracePointer);
 private:
  Point2DWidget* startPoint = 0;
  Point2DWidget* endPoint = 0;
  Point2DWidget* centralPoint = 0;

  QComboBox* traceTypeComboBox = 0;
  QLabel* traceTypeLabel = 0;

  void initialise();
  void showTraceInfo();

 public:
  // Constructor
  explicit TraceInfoWidget(QWidget* parent = 0,
                           Trace::TracePointer = nullptr);
  /**
   *Based on the ComboBox, create a new trace and set it in
   */
  void setNewTracePointer(Trace::TracePointer& i_ptr);

 signals:
  void requestTrace(Trace::TraceType);

 public slots:
  virtual void update();

 private slots:
  void shouldRequestTraceUpdate();

};
#endif  // GUI_TRACEINFOWIDGET_H_
