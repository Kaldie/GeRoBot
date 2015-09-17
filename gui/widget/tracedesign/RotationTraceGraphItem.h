#ifndef GUI_WIDGET_TRACE_ROTATIONTRACEGRAPHITEM_H_
#define GUI_WIDGET_TRACE_ROTATIONTRACEGRAPHITEM_H_


#include "./TraceGraphItem.h"
#include <RotationTrace.h>

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

   /**
    * Get the extreme points on the curve
    * Which are points at 90, 180, 270 and 360 which lay still on the curve
    * @param[in] i_angle The angle to start the calculations with.
    * this parameter is used to cover the corner situations
    * These can be solved easily be recurvion with angle + 360
    * To start the calculation use nullptr for this parameter
    * @param[in] i_trace RotationTracePointer for which this has to be calculated
    * @param[in] i_list list of extreme points
    */
   void getExtremePoints(const double* i_angle,
			 const RotationTrace::RotationTracePointer& i_trace,
			 QList<QPointF>* i_list) const;
   //   QList<Edge *> edgeList;
   //   QPointF newPos;
   //   GraphWidget *graph;
};

#endif // GUI_WIDGET_TRACE_ROTATIONTRACEGRAPHITEM_H_
