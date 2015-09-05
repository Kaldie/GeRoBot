// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <QPainter>
#include <QPointF>
#include <QVariant>
#include <QtWidgets>
#include <Trace.h>
#include <RotationTrace.h>
#include "./TraceGraphItem.h"
#include "./TraceGraphPoint.h"

const int TraceGraphPoint::size = 5;

TraceGraphPoint::TraceGraphPoint(TraceGraphItem* i_parent,
				 TraceGraphPoint::PointPosition i_position)
  :QGraphicsItem(i_parent), m_positionOnTrace(i_position) {
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  setCacheMode(QGraphicsItem::DeviceCoordinateCache);
  setCursor(Qt::CrossCursor);
  LOG_DEBUG("Constructing a TraceGraphPoint");
  updatePositionOnScene(i_parent->getTracePointer().lock());
}


void TraceGraphPoint::paint(QPainter *painter,
			    const QStyleOptionGraphicsItem *option,
			    QWidget *widget) {
  LOG_DEBUG("Paint an Point");
  QPen pen = painter->pen();
  pen.setCosmetic(true);
  pen.setWidth(1);
  pen.setColor(QColor(Qt::red));
  painter->setPen(pen);
  QBrush brush = painter->brush();
  brush.setStyle(Qt::SolidPattern);
  painter->setBrush(brush);
  painter->drawEllipse(-TraceGraphPoint::size * 0.5,
                       -TraceGraphPoint::size * 0.5,
                       TraceGraphPoint::size,
                       TraceGraphPoint::size);
}


QRectF TraceGraphPoint::boundingRect() const {
  LOG_DEBUG("Create bouding box!");
   QRectF rect(-TraceGraphPoint::size * 0.5,
                -TraceGraphPoint::size * 0.5,
                TraceGraphPoint::size,
               TraceGraphPoint::size);
   rect.adjust(-1,-1,1,1);
   return rect;
}


QVariant TraceGraphPoint::itemChange(GraphicsItemChange change,
		    const QVariant &value) {
  if(change != QGraphicsItem::ItemPositionChange) {
    return QGraphicsItem::itemChange(change, value);
  }
  Trace::TracePointer trace = static_cast<TraceGraphItem*>(parentItem())->getTracePointer().lock();
  QPointF point = value.toPointF();
  updateTracePosition(trace, point);
  return QGraphicsItem::itemChange(change, point);
}


void TraceGraphPoint::mousePressEvent(QGraphicsSceneMouseEvent* i_event) {
  if (i_event->button() == Qt::LeftButton) {
    m_startPointAtMouseDown = static_cast<TraceGraphItem*>(parentItem())->
      getTracePointer().lock()->getStartPoint();
  }
  return QGraphicsItem::mousePressEvent(i_event);
}

void TraceGraphPoint::updatePositionOnScene() {
  TraceGraphItem* parent = static_cast<TraceGraphItem*>(parentItem());
  updatePositionOnScene(parent->getTracePointer().lock());
}


void TraceGraphPoint::updatePositionOnScene(const Trace::TracePointer& i_trace) {
  LOG_DEBUG("Updating the position on the scene of this Point");
  switch (m_positionOnTrace) {
  case TraceGraphPoint::StartPoint : {
    // No need to change the position, 0,0 is ALWAYS the start point
    break;
  }
  case TraceGraphPoint::EndPoint : {
    if (i_trace->getEndPoint() != scenePos()) {
      LOG_DEBUG("Update end point");
      setPos(Point2D(i_trace->getEndPoint() - i_trace->getStartPoint()));
    }
    break;
  }
  case TraceGraphPoint::CenterPoint : {
    if (RotationTrace::RotationTracePointer rotationTrace =
        std::dynamic_pointer_cast<RotationTrace>(i_trace)) {
      if (rotationTrace->getCentrePoint() != scenePos())
      LOG_DEBUG("Update Center point");
      setPos(Point2D(rotationTrace->getCentrePoint() - i_trace->getStartPoint()));
    } else {
      LOG_ERROR("Could not convert the trace to a rotation" <<
		" trace while the point was a center point!");
    }
    break;
  }
  default : {
    LOG_ERROR("unknown PointPosition");
    break;
  }}
}


void TraceGraphPoint::updateTracePosition(Trace::TracePointer& i_trace,
                                          QPointF& i_newPosition) {
  LOG_DEBUG("Update trace position based on the current position of the point in the scene");
  switch (m_positionOnTrace) {
  case TraceGraphPoint::StartPoint : {
    LOG_DEBUG("Start Point At MouseDown" << m_startPointAtMouseDown.x()
              << " , " << m_startPointAtMouseDown.y());
    QPointF scenePoint = i_newPosition + m_startPointAtMouseDown;
    i_trace->setStartPoint(scenePoint);
    parentItem()->setPos(i_trace->getStartPoint());
    parentItem()->update();
    i_newPosition = QPointF(0, 0);
    break;
  }
  case TraceGraphPoint::EndPoint : {
    LOG_DEBUG("Previouse end pos() : " << pos().x() << " , " << pos().y());
    LOG_DEBUG("New end pos() : " << i_newPosition.x() << " , " << i_newPosition.y());
    i_trace->setEndPoint(i_trace->getStartPoint() + Point2D(i_newPosition));
    parentItem()->update();
    break;
  }
  case TraceGraphPoint::CenterPoint : {
    if (RotationTrace::RotationTracePointer rotationTrace =
	std::dynamic_pointer_cast<RotationTrace>(i_trace)) {
      rotationTrace->setCentrePoint(mapToScene(pos()));
    } else {
      LOG_ERROR("Could not convert the trace to a rotation" <<
		" trace while the point was a center point!");
    }
    break;
  }
  default : {
    LOG_ERROR("unknown PointPosition");
    break;
  }}
}
