#include <macroHeader.h>
#include <QPainter>
#include <QPointF>
#include <QVariant>
#include <QtWidgets>
#include <Quadrant2D.h>
#include <Point2D.h>
#include "./RotationTraceGraphItem.h"
#include "./TraceGraphPoint.h"

RotationTraceGraphItem::RotationTraceGraphItem(Trace::TracePointer i_trace /*= 0*/)
  : TraceGraphItem(i_trace) {
   LOG_DEBUG("Constructing a Rotation trace!");   // todo setPos at the start point
   new TraceGraphPoint(this,TraceGraphPoint::CenterPoint);
}


QRectF RotationTraceGraphItem::boundingRect() const {
   RotationTrace::RotationTracePointer rotationTrace(getPointer());
   if (!rotationTrace) {
      return QRectF();
   }
   std::vector<Point2D> points;
   points.push_back(rotationTrace->getStartPoint());
   points.push_back(rotationTrace->getEndPoint());
   points.push_back(rotationTrace->getCentrePoint());

   // get the extreme points, points which lay on the curve at 90, 180, 270 and 0
   rotationTrace->getExtremePoints(&points);
   double x(0.0);
   double y(0.0);
   QPointF topLeft(0,0), bottemRight(0,0);
   //   minPoint = points[0];
   //   maxPoint = points[0];
   for (const Point2D& point : points) {
      QPointF qPoint = Point2D(point - rotationTrace->getStartPoint());
      x = qPoint.x();
      y = qPoint.y();
      LOG_DEBUG("x : " << x << " y: " << y);
      if (topLeft.x() > x) {
         topLeft.setX(x);
      }
      if (bottemRight.x() < x) {
         bottemRight.setX(x);
      }
      if (topLeft.y() > y) {
         topLeft.setY(y);
      }
      if (bottemRight.y() < y) {
         bottemRight.setY(y);
      }
   }

   QPointF adjust(3,3);
   topLeft -= adjust;
   bottemRight += adjust;
   LOG_DEBUG("topLeft: " << topLeft.x() << ", " << topLeft.y() << " bottemRight: " << bottemRight.x() << ", " << bottemRight.y());
   return QRectF(topLeft, bottemRight);
}


QPainterPath RotationTraceGraphItem::shape() const {
   RotationTrace::RotationTracePointer rotationTrace(getPointer());
   if (!rotationTrace) {
      return QPainterPath();
   }
   QPainterPath path;
   LOG_DEBUG("Paint rotation trace");
   double startAngle, stopAngle;
   Point2D centerPoint(rotationTrace->getCentrePoint());
   rotationTrace->getStartStopAngle(&startAngle,
                                    &stopAngle);
   centerPoint -= rotationTrace->getStartPoint();
   int sign;
   if (rotationTrace->getIsClockwise())
      sign = -1;
   else
      sign = 1;
   double spanAngle = rotationTrace->getArc().spanAngle() * 180.0/PI * sign;
   LOG_DEBUG("start angle: " << startAngle);
   LOG_DEBUG("Span angle: " << spanAngle);
   double diff = rotationTrace->getArc().radius();
   QRectF rect(centerPoint.x - diff,
               -centerPoint.y - diff,
               2 * diff,
               2 * diff);
   /*
     LOG_DEBUG("Center point: " << centerPoint.x << " , " << centerPoint.y);
     LOG_DEBUG("Start angle: " << startAngle);
     LOG_DEBUG("End angle: " << stopAngle);
     LOG_DEBUG("Span angle: " << spanAngle);
     LOG_DEBUG("Radius is: " << diff);
   */
   path.moveTo(0,0);
   path.arcTo(rect, startAngle, spanAngle);

   return path;
}


QVariant RotationTraceGraphItem::itemChange(GraphicsItemChange change, const QVariant &value) {
   if(change != QGraphicsItem::ItemPositionHasChanged) {
      return QGraphicsItem::itemChange(change, value);
   }
   // distance moved
   RotationTrace::RotationTracePointer rotationTrace(getPointer());
   if (!rotationTrace) {
      return QGraphicsItem::itemChange(change, value);
   }
   // call the base itemChange
   LOG_DEBUG("change: " << Point2D(value.toPointF()).x << " , " << Point2D(value.toPointF()).y);
   Point2D centralPoint = rotationTrace->getCentrePoint() +
      Point2D(value.toPointF()) - rotationTrace->getStartPoint();
   LOG_DEBUG("New centre pos: " << centralPoint.x << ", " << centralPoint.y);
   rotationTrace->setCentrePoint(centralPoint);
   TraceGraphItem::itemChange(change, value);
   return QGraphicsItem::itemChange(change, value);
}


RotationTrace::RotationTracePointer RotationTraceGraphItem::getPointer() const{
  // get a shared_ptr
  Trace::TracePointer trace(getTracePointer().lock());
  // it should be valid!
  if (!trace)
     return nullptr;
  // dynamic pointer cast it to rotation trace
  RotationTrace::RotationTracePointer rotationTrace =
    std::dynamic_pointer_cast<RotationTrace>(trace);
  // Yes this should also work!
  if (!rotationTrace)
     return nullptr;
  return rotationTrace;
}
