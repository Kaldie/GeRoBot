#include <macroHeader.h>
#include <QPainter>
#include <QPainterPathStroker>
#include <QPointF>
#include <QVariant>
#include <RotationTrace.h>
#include "./TraceGraphItem.h"


TraceGraphItem::TraceGraphItem(Trace::TracePointer i_trace /*= 0*/)
   : m_isSelected(true), m_trace(i_trace) {
   setFlag(QGraphicsItem::ItemIsSelectable);
   setFlag(QGraphicsItem::ItemIsMovable);
   LOG_DEBUG("Constructing iteme");
   if (!i_trace)
      LOG_DEBUG("Trace is not valid!");
   //   setFlag(QGraphicsItem::ItemSendsGeometryChanges);
   // todo setPos at the start point
}


void TraceGraphItem::setTrace(Trace::TracePointer i_trace){
   m_trace = i_trace;
}
//void TraceGraphItem::addEditPoint(Edge* i_editPoint) {
//   m_editPoints.addItem(i_editPoint);
//}


void TraceGraphItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
   Q_UNUSED(option);
   Q_UNUSED(widget);
   LOG_DEBUG("Painting a item.");
   Trace::TracePointer trace(m_trace.lock());
   if (!trace) {
      LOG_DEBUG("Current weak pointer does not point to anything");
      return;
   }

   QPen pen(Qt::black,1);
   pen.setCapStyle(Qt::RoundCap);
   if (m_isSelected) {
      LOG_DEBUG("Make magic!!");
      pen.setColor(Qt::red);
      painter->setPen(pen);
      //      for (auto x: m_editPoints) {
      // x.setHide(false);
      // x.update();
      //      }
   } else {
      pen.setWidth(1);
      painter->setPen(pen);
      //      for (auto x: m_editPoints) {
         //         x.setHide(true);
      //      }
   }
   //   painter->drawEllipse(0,0,10,10);
   painter->drawPath(shape());
   //   painter->drawRect(boundingRect());
}


QRectF TraceGraphItem::boundingRect() const {
   Trace::TracePointer trace = m_trace.lock();
   if (!trace) {
      return QRectF();
   }
   QPointF adjust(1,1);
   QPointF start(0,0);
   QPointF end(trace->getEndPoint().x - trace->getStartPoint().x,
               -1 * (trace->getEndPoint().y - trace->getStartPoint().y));
   return QRectF(start-adjust,end+adjust);
}


QPainterPath TraceGraphItem::shape() const {
   Trace::TracePointer trace(m_trace.lock());
   if (!trace) {
      return QPainterPath();
   }

   QPainterPath path;
   if (trace->getTraceType() == Trace::Curve) {
      LOG_DEBUG("Paint rotation trace");
      RotationTrace::RotationTracePointer
         rotationTrace(std::dynamic_pointer_cast<RotationTrace>(trace));
      assert(rotationTrace);
      Point2D centerPoint(*rotationTrace->getPointPointers()[2]);

      int startAngle = (trace->getStartPoint() - centerPoint).getAlpha() *
         double(180) / PI;
      int endAngle = (trace->getEndPoint() - centerPoint).getAlpha() *
         double(180) / PI;
      if (startAngle < endAngle) {
         path.moveTo(QPointF(trace->getStartPoint().x,
                             -1 * trace->getStartPoint().y));
      } else {
         path.moveTo(QPointF(trace->getEndPoint().x,
                             -1 * trace->getEndPoint().y));
      }
      int spanAngle = endAngle - startAngle;

      double diff = std::abs(Magnitude(centerPoint - trace->getStartPoint()));

      LOG_DEBUG("Start angle: " << startAngle / double(16));
      LOG_DEBUG("End angle: " << endAngle / double(16));
      LOG_DEBUG("Span angle: " << spanAngle / double(16));
      LOG_DEBUG("Radius is: " << diff);
      LOG_DEBUG("lala1: " << centerPoint.x - diff);
      LOG_DEBUG("lala2: " << -1*(centerPoint.y - diff));
      LOG_DEBUG("lala1: " << centerPoint.x + diff);
      LOG_DEBUG("lala2: " << -1 * (centerPoint.y + diff));

      QRectF rect(QPointF(centerPoint.x + diff,
                          -1 * (centerPoint.y - diff)),
                  QPointF(centerPoint.x - diff,
                          -1 * (centerPoint.y + diff)));

      path.arcTo(rect, startAngle, spanAngle);
   } else if (trace->getTraceType() == Trace::Line) {
      LOG_DEBUG("Paint trace line!");
      QPointF start(0, 0);
      QPointF end(trace->getEndPoint().x - trace->getStartPoint().x,
                  -1 * (trace->getEndPoint().y - trace->getStartPoint().y));
      path.moveTo(start);
      path.lineTo(end);
   } else {
      LOG_ERROR("Evil trace type found!");
   }
   return path;
}


/*
QVariant TraceGraphItem::itemChange(GraphicsItemChange change, const QVariant &value) {
   LOG_DEBUG("ItemChange is called!");
   Trace::TracePointer trace(m_trace.lock());
   if(change != QGraphicsItem::ItemPositionHasChanged || !trace) {
      return QGraphicsItem::itemChange(change, value);
   }
   LOG_DEBUG("Update start and stop position of the trace!");
   QPointF point = value.toPointF();
   LOG_DEBUG("New pos: " << point.x() << ", " << point.y());
   Point2D newStartPoint(point.x(), -1 * point.y());
   Point2D endPoint = trace->getEndPoint() +
      (newStartPoint - trace->getStartPoint());
   LOG_DEBUG("New end pos: " << endPoint.x << ", " << endPoint.y);
   trace->setEndPoint(endPoint);
   trace->setStartPoint(newStartPoint);
   update();
   return QGraphicsItem::itemChange(change, value);
}
*/
