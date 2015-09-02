#include <macroHeader.h>
#include <QPainter>
#include <QPointF>
#include <QVariant>
#include <QtWidgets>
#include "./RotationTraceGraphItem.h"


RotationTraceGraphItem::RotationTraceGraphItem(Trace::TracePointer i_trace /*= 0*/)
  : TraceGraphItem(i_trace) {
   // todo setPos at the start point
}


QRectF RotationTraceGraphItem::boundingRect() const {
   RotationTrace::RotationTracePointer rotationTrace(getPointer());
   if (!rotationTrace) {
      return QRectF();
   }


   QPointF start(0,0);
   QPointF end(rotationTrace->getEndPoint().x - rotationTrace->getStartPoint().x,
               -1 * (rotationTrace->getEndPoint().y - rotationTrace->getStartPoint().y));
   QPointF minPoint, maxPoint;
   QList<QPointF> points;
   double startAngle, stopAngle, radius;
   Point2D centerPoint = static_cast<Point2D>(rotationTrace->getCentrePoint() -
                          rotationTrace->getStartPoint());
   rotationTrace->getStartStopAngle(&startAngle,
                                    &stopAngle);
   radius = rotationTrace->getArc().radius();
   points.append(start);
   points.append(end);
   if ((startAngle < 90 && 90 < stopAngle) ||
       (startAngle > 90 && 90 > stopAngle)) {
      LOG_DEBUG("Add 90 degree point " << startAngle << " " << stopAngle);
      points.append(QPointF(centerPoint.x, -centerPoint.y - radius));
   }

   if (((startAngle < 270) && (270 < stopAngle)) ||
       ((startAngle > 270) && (270 > stopAngle)) ) {
      LOG_DEBUG("Add 270 point!" << startAngle << " " << stopAngle);
      points.append(QPointF(centerPoint.x, -centerPoint.y + radius));
   }
   if ((360 > startAngle && startAngle > 180 && stopAngle < 180) ||
       (360 > stopAngle && stopAngle > 180 && startAngle < 180)) {
      LOG_DEBUG("Add 180 point!" << startAngle << " " << stopAngle);
      points.append(QPointF(centerPoint.x - radius,
                            -centerPoint.y));
   }
   if ((startAngle > 180) && (180 > stopAngle) && false) {
      LOG_DEBUG("Add 180 point!" << startAngle << " " << stopAngle);
      points.append(QPointF(centerPoint.x - radius,
                            -centerPoint.y));
   }
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

   QPointF adjust(2,2);
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
   double spanAngle = stopAngle - startAngle;
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
   LOG_DEBUG("get a pointer");
   RotationTrace::RotationTracePointer rotationTrace(getPointer());
   LOG_DEBUG("Pointer is gotten");
   if (!rotationTrace) {
      return QGraphicsItem::itemChange(change, value);
   }
   // distance the item is moved
   QPointF point = value.toPointF();
   Point2D newStartPoint(point.x(), -1 * point.y());
   Point2D moved = newStartPoint - rotationTrace->getStartPoint();
   // call the base itemChange
   TraceGraphItem::itemChange(change, value);
   Point2D centralPoint = rotationTrace->getCentrePoint() + moved;
   LOG_DEBUG("New centre pos: " << centralPoint.x << ", " << centralPoint.y);
   rotationTrace->setCentrePoint(centralPoint);
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
