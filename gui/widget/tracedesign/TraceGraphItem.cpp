#include <macroHeader.h>
#include <QPainter>
#include <QPointF>
#include <QVariant>
#include <QtWidgets>
#include <RotationTrace.h>
#include "./TraceGraphItem.h"
#include "./TraceGraphPoint.h"

const QString TraceGraphItem::RemoveTraceActionText("Remove Trace");
const QString TraceGraphItem::ConvertToLineActionText("Convert to Line");
const QString TraceGraphItem::ConvertToCurveActionText("Convert to Curve");
const QString TraceGraphItem::ConvertDirection("Alter direction");

TraceGraphItem::TraceGraphItem(Trace::TracePointer i_trace /*= 0*/)
  : QGraphicsObject(),
    m_trace(i_trace),
    m_isBeingMoved(false) {
   setFlag(QGraphicsItem::ItemIsSelectable);
   setFlag(QGraphicsItem::ItemIsMovable);
   setFlag(QGraphicsItem::ItemSendsGeometryChanges);
   setCursor(Qt::OpenHandCursor);
   LOG_DEBUG("Constructing iteme");
   if (!i_trace)
      LOG_DEBUG("Trace is not valid!");
   else
      setPos(i_trace->getStartPoint());
   new TraceGraphPoint(this,TraceGraphPoint::StartPoint);
   new TraceGraphPoint(this,TraceGraphPoint::EndPoint);
   // todo setPos at the start point
}


void TraceGraphItem::setTrace(const Trace::TracePointer& i_trace){
   m_trace = i_trace;
   setPos(i_trace->getStartPoint());
}


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
      pen.setWidth(2);
      painter->setPen(pen);
   } else {
      pen.setWidth(1);
      painter->setPen(pen);
   }
   painter->drawPath(shape());
}


QRectF TraceGraphItem::boundingRect() const {
   Trace::TracePointer trace = m_trace.lock();
   if (!trace) {
      return QRectF();
   }

   QPointF start(0,0);
   QPointF end(trace->getEndPoint() - trace->getStartPoint());
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
   QPointF end(trace->getEndPoint() - trace->getStartPoint());
   path.moveTo(start);
   path.lineTo(end);
   return path;
}


void TraceGraphItem::mousePressEvent(QGraphicsSceneMouseEvent* i_event) {
  if (i_event->button() == Qt::LeftButton) {
    // record the start position of the "Move"
    LOG_DEBUG("Left Mouse is pressed");
    m_isBeingMoved = true;
  }
  return QGraphicsObject::mousePressEvent(i_event);
}


void TraceGraphItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* i_event) {
  if (i_event->button() == Qt::LeftButton) {
    LOG_DEBUG("Left mouse is released");
    m_isBeingMoved = false;
  }
  return QGraphicsObject::mouseReleaseEvent(i_event);
}


QVariant TraceGraphItem::itemChange(GraphicsItemChange change, const QVariant &value) {
   if(change != QGraphicsItem::ItemPositionHasChanged) {
      return QGraphicsItem::itemChange(change, value);
   }
   // if (!m_isBeingMoved) {
   //LOG_DEBUG("Something is moving me, but not the user");
   //return QGraphicsItem::itemChange(change, value);
   //}

   Trace::TracePointer trace(m_trace.lock());
   if (!trace) {
      return QGraphicsItem::itemChange(change, value);
   }

   LOG_DEBUG("Update start and stop position of the trace!");
   Point2D newStartPoint(value.toPointF());
   LOG_DEBUG("New pos: " << newStartPoint.x << ", " << newStartPoint.y);
   Point2D endPoint = trace->getEndPoint() +
      (newStartPoint - trace->getStartPoint());
   LOG_DEBUG("New end pos: " << endPoint.x << ", " << endPoint.y);

   // this is not necessary as the item as is is moved, its rect stays the same relative to its left upper conner
   // prepareGeometryChange();
   trace->setEndPoint(endPoint);
   trace->setStartPoint(newStartPoint);
   return value.toPointF();
}



void TraceGraphItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
   QMenu *menu = new QMenu;
   // add removal of traces
   menu->addAction(TraceGraphItem::RemoveTraceActionText,
                   this, SLOT(handleTrigger()), QKeySequence::Delete);
   // add conversion of trace to the oppisite site
   if (m_trace.lock()->getTraceType() == Trace::Curve) {
      menu->addAction(TraceGraphItem::ConvertToLineActionText,
                      this, SLOT(handleTrigger()));
      menu->addAction(TraceGraphItem::ConvertDirection,
                      this, SLOT(handleTrigger()));
   } else if (m_trace.lock()->getTraceType() == Trace::Line) {
      menu->addAction(TraceGraphItem::ConvertToCurveActionText,
                      this, SLOT(handleTrigger()));
   } else {
      LOG_ERROR("Unknown trace type");
   }
   menu->popup(event->screenPos());
}


void TraceGraphItem::updatePosition() {
   if (Trace::TracePointer trace = m_trace.lock()) {
      LOG_DEBUG("Set the item to the new position is necessary!");
      prepareGeometryChange();
      if (pos() != trace->getStartPoint()){
         setPos(trace->getStartPoint());
      }
      for (auto& x : childItems()) {
         TraceGraphPoint* point = dynamic_cast<TraceGraphPoint*>(x);
         if (point) {
            LOG_DEBUG("updateing the point from the item");
            point->updatePositionOnScene();
         }
      }
      update();
   }
}


void TraceGraphItem::handleTrigger() {
   QAction* action = dynamic_cast<QAction*>(sender());
   LOG_DEBUG("menu says hi");
   if (!action) {
      LOG_ERROR("Action is not resolved. Do not call this function directly.");
   }
   if (action->text() == TraceGraphItem::RemoveTraceActionText) {
      LOG_DEBUG("Emit signal to make this trace be removed");
      //      scene()->setSelected(m_trace.lock());
      emit removeThisTrace(m_trace.lock());
   } else if (action->text() == TraceGraphItem::ConvertToLineActionText) {
      LOG_DEBUG("Emit signal to make convert this trace");
      emit convertThisTrace(m_trace.lock(), Trace::Line);
   } else if (action->text() == TraceGraphItem::ConvertToCurveActionText) {
      LOG_DEBUG("Emit signal to make convert this trace");
      emit convertThisTrace(m_trace.lock(), Trace::Curve);
   } else if (action->text() == TraceGraphItem::ConvertDirection) {
      LOG_DEBUG("Emit signal to change direction");
      emit reverse(m_trace.lock());
   } else {
      LOG_ERROR("Action: '" << action->text().toStdString() << "' is not resolved!");
   }
}


void TraceGraphItem::updatePointPositions(TraceGraphPoint* i_point /* = nullptr*/) const {
  Trace::TracePointer trace = m_trace.lock();
  if (!trace) {
    LOG_ERROR("Unknown trace!");
  }
  for (QGraphicsItem* child : childItems()) {
    TraceGraphPoint* point = static_cast<TraceGraphPoint*>(child);
    if (i_point == point) {
      continue;
    }
    switch (point->getPositionOnTrace()) {
    case TraceGraphPoint::StartPoint: {
      point->setPos(QPointF(0.0,0.0));
      break;
    }
    case TraceGraphPoint::EndPoint: {
      point->setPos(trace->getEndPoint() - trace->getStartPoint());
      break;
    }
    case TraceGraphPoint::CenterPoint: {
      if (trace->getTraceType() == Trace::Curve) {
  std::shared_ptr<RotationTrace> rotationTrace = std::static_pointer_cast<RotationTrace>(trace);
  point->setPos(rotationTrace->getCentrePoint() - trace->getStartPoint());
  break;
      }
    }
    default: {
      LOG_ERROR("Unknown TraceGraphPoint type!");
    }
    }
  }
}
