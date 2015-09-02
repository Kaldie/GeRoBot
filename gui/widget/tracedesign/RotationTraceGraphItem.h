#ifndef GUI_WIDGET_TRACE_ROTATIONTRACEGRAPHITEM_H_
#define GUI_WIDGET_TRACE_ROTATIONTRACEGRAPHITEM_H_

#include <RotationTrace.h>
#include "./TraceGraphItem.h"

class TraceGraphEditPoint;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class RotationTraceGraphItem : public TraceGraphItem {
 Q_OBJECT
 public:
   RotationTraceGraphItem(Trace::TracePointer = nullptr);

   /**
    * Add edit points to the trace.
    * These will be visible if the trace is selected
    * The will enable the user to added the trace from the graph
    */
   //void addEditPoint(Edge* i_editPoint);

   enum { Type = UserType + 2};
   virtual int type() const Q_DECL_OVERRIDE { return Type; }

   virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
   virtual QPainterPath shape() const Q_DECL_OVERRIDE;
   //virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

 protected:
   virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

 private:
   void startAndStopAngle(double* i_startAngle,
                          double* i_stopAngle) const;
   RotationTrace::RotationTracePointer getPointer() const;
   //   QList<Edge *> edgeList;
   //   QPointF newPos;
   //   GraphWidget *graph;
};

#endif // GUI_WIDGET_TRACE_ROTATIONTRACEGRAPHITEM_H_
