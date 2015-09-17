#include <macroHeader.h>
#include <QPainter>
#include <QPointF>
#include <QVariant>
#include <QtWidgets>
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
   QPointF minPoint, maxPoint;
   QList<QPointF> points;
   QPointF start(0,0);
   QPointF end(Point2D(rotationTrace->getEndPoint() -
                       rotationTrace->getStartPoint()));
   Point2D centerPoint = Point2D(rotationTrace->getCentrePoint() -
                                 rotationTrace->getStartPoint());
   points.append(start);
   points.append(end);
   points.append(centerPoint);

   // get the extreme points, points which lay on the curve at 90, 180, 270 and 0
   getExtremePoints(nullptr, rotationTrace, &points);

   minPoint = points[0];
   maxPoint = points[1];
   double x(0.0);
   double y(0.0);
   for (const auto& point : points) {
      x = point.x();
      y = point.y();
      LOG_DEBUG("x : " << x << " y: " << y);
      if (minPoint.x() > x) {
         minPoint.setX(x);
      }
      if (maxPoint.x() < x) {
         maxPoint.setX(x);
      }
      if (minPoint.y() > y) {
         minPoint.setY(y);
      }
      if (maxPoint.y() < y) {
         maxPoint.setY(y);
      }
   }

   QPointF adjust(3,3);
   return QRectF(minPoint - adjust,
                 maxPoint + adjust);
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


void RotationTraceGraphItem::getExtremePoints(const double* i_angle,
                                              const RotationTrace::RotationTracePointer& i_trace,
                                              QList<QPointF>* i_list) const {
   double startAngle, stopAngle, angle;
   if (!i_angle) {
      i_trace->getStartStopAngle(&startAngle, &stopAngle);
      if (i_trace->getIsClockwise()) {
         angle = startAngle;
      } else {
         angle = stopAngle;
      }
   } else {
      angle = *i_angle;
   }

   double span = i_trace->getArc().spanAngle() * 180.0 / PI;
   double radius = i_trace->getArc().radius();
   Point2D centerPoint = Point2D(i_trace->getCentrePoint() -
                                 i_trace->getStartPoint());
   LOG_DEBUG("Angle: " << angle <<" , span: " << span);
   if (angle < span) {
      LOG_DEBUG("add 360 deg point");
      // this means it goes through 0, the angle needs to be adjusted such that it will work
      double alterAngle(angle + 360.0);
      getExtremePoints(&alterAngle, i_trace, i_list);
      i_list->append(QPointF(centerPoint.x + radius, -centerPoint.y));
   }
   // check if 90 is in the mix
   if (angle > 90 && span > (angle - 90)) {
      LOG_DEBUG("add 90 deg point");
      i_list->append(QPointF(centerPoint.x, -centerPoint.y - radius));
   }
   if ( angle > 180 && span > angle - 180) {
      LOG_DEBUG("add 180 deg point");
      i_list->append(QPointF(centerPoint.x - radius,  -centerPoint.y));
   }
   if ( angle > 270 && span > angle - 270) {
      LOG_DEBUG("add 270 deg point");
      i_list->append(QPointF(centerPoint.x, -centerPoint.y + radius));
   }

}
