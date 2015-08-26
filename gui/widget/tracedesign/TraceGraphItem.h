#ifndef GUI_WIDGET_TRACE_TRACEGRAPHITEM
#define GUI_WIDGET_TRACE_TRACEGRAPHITEM

#include <QGraphicsItem>
#include <QList>
#include <Trace.h>
class TraceGraphEditPoint;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class TraceGraphItem : public QGraphicsItem {
   //   GETSET(QList<TraceGraphEditPoint*>, m_editPoints, EditPoints);
   GETSET(bool, m_isSelected, isSelected);
   GETSET(Trace::WeakTracePointer, m_trace, TracePointer);

 public:
   TraceGraphItem(Trace::TracePointer = nullptr);

   /**
    * Add edit points to the trace.
    * These will be visible if the trace is selected
    * The will enable the user to added the trace from the graph
    */
   //virtual void addEditPoint(Edge* i_editPoint);

   /**
    * Link this graph to a trace in the master widget
    */
   void setTrace(Trace::TracePointer);

   enum { Type = UserType + 1 };
   virtual int type() const Q_DECL_OVERRIDE { return Type; }

   virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
   virtual QPainterPath shape() const Q_DECL_OVERRIDE;
   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

 protected:
   virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

 private:
   void startAndStopAngle(double* i_startAngle,
                          double* i_stopAngle) const;
   //   QList<Edge *> edgeList;
   //   QPointF newPos;
   //   GraphWidget *graph;
};

#endif // GUI_WIDGET_TRACE_TRACEGRAPHITEM
