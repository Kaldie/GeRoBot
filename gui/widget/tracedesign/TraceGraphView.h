#ifndef GUI_WIDGET_TRACEWIDGET_TRACEGRAPHWIDGET_H_
#define GUI_WIDGET_TRACEWIDGET_TRACEGRAPHWIDGET_H_

#include <QGraphicsView>
#include <QList>

class TraceGraphItem;
class QGraphicsScene;

class TraceGraphView : public QGraphicsView {
  Q_OBJECT

 public:
  TraceGraphWidget(QWidget *parent = 0);
  addTraceItem(Trace::WeakTracePointer i_trace);
  removeTraceItem(Trace::WeakTracePointer i_trace);
  setSelected(Trace::WeakTracePointer i_trace);

 public slots:
  void zoomIn();
  void zoomOut();
  void updateTraceGraphItem(TraceGraphItem*);

 signals:
  updateSelectedTrace(Trace::WeakPointer i_newCurrentTrace);

 protected:
  void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
#ifndef QT_NO_WHEELEVENT
  void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif
  void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;
  void scaleView(qreal scaleFactor);

 private:
  QGraphicsScene* m_scene;
  QList<TraceGraphItem*> m_traceGraphItems;
};

#endif // GUI_WIDGET_TRACEWIDGET_TRACEGRAPHWIDGET_H_
