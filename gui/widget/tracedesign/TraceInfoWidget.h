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
  Point2DWidget* startPoint;
  Point2DWidget* endPoint;
  Point2DWidget* centralPoint;

  QComboBox* traceTypeComboBox;
  QLabel* traceTypeLabel;

  void initialise();

 public:
  /// Constructor
  explicit TraceInfoWidget(QWidget* parent = nullptr,
                           Trace::TracePointer = nullptr);
  /**
   * Sets a new trace pointer as source and update() the widget
   */
  void setNewTracePointer(const Trace::TracePointer& i_ptr);
  void setNewTracePointer(const Trace::WeakTracePointer& i_ptr);

 signals:
  void requestTrace(Trace::TraceType);
  void tracePositionChanged();

 public slots:
  virtual void update();

 private slots:
  void shouldRequestTraceUpdate();
  void shouldSignalHasNewPosition();

};
#endif  // GUI_TRACEINFOWIDGET_H_
