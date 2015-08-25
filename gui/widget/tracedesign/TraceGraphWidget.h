#ifndef GUI_WIDGET_TRACEWIDGET_TRACEGRAPHVIEW_H_
#define GUI_WIDGET_TRACEWIDGET_TRACEGRAPHVIEW_H_

#include <QGraphicsView>
#include <QList>

class TraceGraphItem;

class TraceGraphView : public QGraphicsView {
  Q_OBJECT

 public:
  TraceGraphView(QWidget *parent = 0);

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

#endif // GUI_WIDGET_TRACEWIDGET_TRACEGRAPHVIEW_H_
