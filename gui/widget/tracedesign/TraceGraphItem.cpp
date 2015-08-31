#include <macroHeader.h>
#include <QPainter>
#include <QPointF>
#include <QVariant>
#include <QtWidgets>
#include <RotationTrace.h>
#include "./TraceGraphItem.h"


TraceGraphItem::TraceGraphItem(Trace::TracePointer i_trace /*= 0*/)
   : m_isSelected(true), m_trace(i_trace) {
   setFlag(QGraphicsItem::ItemIsSelectable);
   setFlag(QGraphicsItem::ItemIsMovable);
   setFlag(QGraphicsItem::ItemSendsGeometryChanges);
   setCursor(Qt::OpenHandCursor);
   LOG_DEBUG("Constructing iteme");
   if (!i_trace)
      LOG_DEBUG("Trace is not valid!");
   else
      setPos(i_trace->getStartPoint());
   // todo setPos at the start point
}


void TraceGraphItem::setTrace(Trace::TracePointer i_trace){
   m_trace = i_trace;
   setPos(i_trace->getStartPoint());
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
   if (isSelected()) {
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
   painter->drawRect(boundingRect());
}


QRectF TraceGraphItem::boundingRect() const {
   Trace::TracePointer trace = m_trace.lock();
   if (!trace) {
      return QRectF();
   }

   QPointF start(0,0);
   QPointF end(trace->getEndPoint().x - trace->getStartPoint().x,
               -1 * (trace->getEndPoint().y - trace->getStartPoint().y));
   QPointF minPoint;
   QPointF maxPoint;
      minPoint.setX(std::min(start.x(), end.x()));
      minPoint.setY(std::min(start.y(), end.y()));
      maxPoint.setX(std::max(start.x(), end.x()));
      maxPoint.setY(std::max(start.y(), end.y()));
   QPointF adjust(2,2);
   return QRectF(minPoint - adjust,
                 maxPoint + adjust);
}


QPainterPath TraceGraphItem::shape() const {
   Trace::TracePointer trace(m_trace.lock());
   if (!trace) {
      return QPainterPath();
   }

   QPainterPath path;
      LOG_DEBUG("Paint trace line!");
      QPointF start(0, 0);
      QPointF end(trace->getEndPoint().x - trace->getStartPoint().x,
                  -trace->getEndPoint().y - -trace->getStartPoint().y);
      path.moveTo(start);
      path.lineTo(end);
   return path;
}



QVariant TraceGraphItem::itemChange(GraphicsItemChange change, const QVariant &value) {
   if(change != QGraphicsItem::ItemPositionHasChanged) {
      return QGraphicsItem::itemChange(change, value);
   }
   Trace::TracePointer trace(m_trace.lock());
   if (!trace) {
      return QGraphicsItem::itemChange(change, value);
   }

   LOG_DEBUG("Update start and stop position of the trace!");
   QPointF point = value.toPointF();
   LOG_DEBUG("New pos: " << point.x() << ", " << point.y());
   Point2D newStartPoint(point.x(), -1 * point.y());
   Point2D endPoint = trace->getEndPoint() +
      (newStartPoint - trace->getStartPoint());
   LOG_DEBUG("New end pos: " << endPoint.x << ", " << endPoint.y);
   prepareGeometryChange();
   trace->setEndPoint(endPoint);
   trace->setStartPoint(newStartPoint);
   return QGraphicsItem::itemChange(change, value);
}


void TraceGraphItem::updatePosition() {
   if (Trace::TracePointer trace = m_trace.lock()) {
      LOG_DEBUG("Set the item to the new position is necessary!");
      prepareGeometryChange();
      if (pos() != trace->getStartPoint()){
         setPos(trace->getStartPoint());
      }
      update();
      scene()->update();
   }
}
