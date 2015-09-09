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
  Q_UNUSED(option);
  Q_UNUSED(widget);
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
   rect.adjust(-2,-2,2,2);
   return rect;
}


void TraceGraphPoint::mousePressEvent(QGraphicsSceneMouseEvent* i_event) {
  if (i_event->button() == Qt::LeftButton) {
    // record the start position of the "Move"
    m_startPointAtMouseDown = static_cast<TraceGraphItem*>(parentItem())->
      getTracePointer().lock()->getStartPoint();
  }
  return QGraphicsItem::mousePressEvent(i_event);
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
      LOG_DEBUG("Update end point on scene");
      setPos(Point2D(i_trace->getEndPoint() - i_trace->getStartPoint()));
    }
    break;
  }
  case TraceGraphPoint::CenterPoint : {
    if (RotationTrace::RotationTracePointer rotationTrace =
        std::dynamic_pointer_cast<RotationTrace>(i_trace)) {
      if (rotationTrace->getCentrePoint() != scenePos())
      LOG_DEBUG("Update Center point on scene");
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
  LOG_DEBUG("Current Start point: " << i_trace->getStartPoint().x << " , " << i_trace->getStartPoint().y);
  LOG_DEBUG("Current End point: " << i_trace->getEndPoint().x << " , " << i_trace->getEndPoint().y);

  switch (m_positionOnTrace) {
  case TraceGraphPoint::StartPoint : {
    LOG_DEBUG("Start Point At MouseDown" << m_startPointAtMouseDown.x()
              << " , " << m_startPointAtMouseDown.y());
    Point2D newPoint = Point2D(i_newPosition) + m_startPointAtMouseDown;
    correctTracePosition(i_trace, &newPoint);
    i_trace->setStartPoint(newPoint);
    parentItem()->setPos(newPoint);
    i_newPosition = QPointF(0, 0);
    break;
  }
  case TraceGraphPoint::EndPoint : {
    Point2D point = i_trace->getStartPoint() + Point2D(i_newPosition);
    LOG_DEBUG("New end pos() : " << point.x << " , " << point.y);
    if (correctTracePosition(i_trace, &point)) {
      LOG_DEBUG("Corrected end pos() : " << point.x  << " , " << point.y);
    }
    i_trace->setEndPoint(point);
    i_newPosition = Point2D(i_trace->getEndPoint() - i_trace->getStartPoint());
    break;
  }
  case TraceGraphPoint::CenterPoint : {
    if (RotationTrace::RotationTracePointer rotationTrace =
        std::dynamic_pointer_cast<RotationTrace>(i_trace)) {
      LOG_DEBUG("Center point: " << rotationTrace->getCentrePoint().x << " , " << rotationTrace->getCentrePoint().y);
      Point2D possibleStartPoint = i_trace->getStartPoint() + Point2D(i_newPosition);
      if (correctTracePosition(i_trace, &possibleStartPoint)) {
        rotationTrace->setCentrePoint(possibleStartPoint);
      }
      i_newPosition = Point2D(rotationTrace->getCentrePoint() - i_trace->getStartPoint());
    } else {
      LOG_ERROR("Could not convert the trace to a rotation" <<
		" trace while the point was a center point!");
    }
    break;
  }
  default : {
    LOG_ERROR("unknown PointPosition");
    break;
  }}  // end switch

  parentItem()->update();
}


bool TraceGraphPoint::correctTracePosition(Trace::TracePointer trace,
                                           Point2D* i_newPoint) {
  LOG_DEBUG("correctTracePosition");
  // if it is a line, it is always ok
  if (trace->getTraceType() == Trace::Line) {
    LOG_DEBUG("No need to correct it, its a line");
    return false;
  }
  // if it is a curve
  RotationTrace::RotationTracePointer rotationTrace;
  rotationTrace = std::dynamic_pointer_cast<RotationTrace>(trace);
  if (!curveNeedsCorrection(rotationTrace, *i_newPoint)) {
    return false;
  }

  LOG_DEBUG("Needing to fix it!");
  // correcting it here
  switch (m_positionOnTrace) {
  case TraceGraphPoint::CenterPoint : {
    // calculate the vector between the start and stop position
    Vector2D betweenPoints = trace->getEndPoint() - trace->getStartPoint();
    // calculate the vector perpendicular to it
    Vector2D perpendicular(-betweenPoints.y, betweenPoints.x);
    // calculate the point between the center point between the start and stop position
    Point2D pointBetweenPoints = rotationTrace->getPointBetweenStartAndStopPosition();
    *i_newPoint = pointBetweenPoints +
      Vector2D::dotProduct(perpendicular.normalize(), *i_newPoint) * perpendicular;
    break;
  }
  case TraceGraphPoint::StartPoint : {
    // start point is handled the same as the end point
  }
  case TraceGraphPoint::EndPoint : {
    LOG_DEBUG("Fixing the end or start point");
    LOG_DEBUG("Point: " << i_newPoint->x << " , " << i_newPoint->y);
    *i_newPoint = rotationTrace->intersectingPoint(*i_newPoint);
    LOG_DEBUG("New point: " << i_newPoint->x << " , " << i_newPoint->y);
    break;
  }
  default : {
    LOG_ERROR("unknown PointPosition");
    break;
  }}
  return true;
}


bool TraceGraphPoint::curveNeedsCorrection(
  const RotationTrace::RotationTracePointer& i_rotationTrace,
  const Point2D& i_newPoint) const {
  switch (m_positionOnTrace) {
  case TraceGraphPoint::CenterPoint : {
  try {
    Circle2D(i_rotationTrace->getStartPoint(),
             i_rotationTrace->getEndPoint(),
             i_newPoint);
    return false;
  } catch (std::runtime_error) {}
  return true;
  break;
  }
  case TraceGraphPoint::StartPoint : {
  try {
    Circle2D(i_newPoint,
             i_rotationTrace->getEndPoint(),
             i_rotationTrace->getCentrePoint());
    return false;
  } catch (std::runtime_error) {}
  return true;
  break;
  }
  case TraceGraphPoint::EndPoint : {
  try {
    Circle2D(i_rotationTrace->getStartPoint(),
             i_newPoint,
             i_rotationTrace->getCentrePoint());
    return false;
  } catch (std::runtime_error) {}
  return true;
  break;
  }}
  return true;
}
