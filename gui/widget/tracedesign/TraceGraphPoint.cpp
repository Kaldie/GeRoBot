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

bool TraceGraphPoint::m_snapToOthers = false;
const float TraceGraphPoint::m_searchDistance = 20.0;
const int TraceGraphPoint::m_size = 5;

TraceGraphPoint::TraceGraphPoint(TraceGraphItem* i_parent,
				 TraceGraphPoint::PointPosition i_position)
  : QGraphicsItem(i_parent), 
    m_positionOnTrace(i_position),
    m_startPointAtMouseDown(QPointF(0.0,0.0)) {
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
  painter->drawEllipse(-TraceGraphPoint::m_size * 0.5,
                       -TraceGraphPoint::m_size * 0.5,
                       TraceGraphPoint::m_size,
                       TraceGraphPoint::m_size);
}


QRectF TraceGraphPoint::boundingRect() const {
   QRectF rect(-TraceGraphPoint::m_size * 0.5,
                -TraceGraphPoint::m_size * 0.5,
                TraceGraphPoint::m_size,
               TraceGraphPoint::m_size);
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
  QPointF point = value.toPointF();
  if (TraceGraphPoint::m_snapToOthers) {
    snapPointToOthers(&point);
  }
  Trace::TracePointer trace = static_cast<TraceGraphItem*>(parentItem())->getTracePointer().lock();
  updateTracePosition(trace, point);
  return point;
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
    correctTracePosition(i_trace, &point);
    i_trace->setEndPoint(point);
    i_newPosition = Point2D(i_trace->getEndPoint() - i_trace->getStartPoint());
    break;
  }
  case TraceGraphPoint::CenterPoint : {
    if (RotationTrace::RotationTracePointer rotationTrace =
        std::dynamic_pointer_cast<RotationTrace>(i_trace)) {
      LOG_DEBUG("Center point: " << rotationTrace->getCentrePoint().x << " , " << rotationTrace->getCentrePoint().y);
      Point2D possibleCenterPoint = i_trace->getStartPoint() + Point2D(i_newPosition);
      LOG_DEBUG("possibleCenterPoint: " << possibleCenterPoint.x << " , " << possibleCenterPoint.y);
      correctTracePosition(i_trace, &possibleCenterPoint);
      LOG_DEBUG("Corrected position: " << possibleCenterPoint.x << " , " << possibleCenterPoint.y);
      rotationTrace->setCentrePoint(possibleCenterPoint);
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
    Vector2D perpendicular(betweenPoints.y, -betweenPoints.x);
    // calculate the point between the center point between the start and stop position
    *i_newPoint = rotationTrace->getPointBetweenStartAndStopPosition() +
      Vector2D::dotProduct(*i_newPoint - rotationTrace->getPointBetweenStartAndStopPosition(),
                           perpendicular.normalize()) * perpendicular.normalize();
    break;
  }
  case TraceGraphPoint::StartPoint : {
    // start point is handled the same as the end point
  }
  case TraceGraphPoint::EndPoint : {
    LOG_DEBUG("Fixing the end or start point");
    LOG_DEBUG("Point: " << i_newPoint->x << " , " << i_newPoint->y);
    try {
      *i_newPoint = rotationTrace->intersectingPoint(*i_newPoint);
    } catch (std::runtime_error) {
      LOG_DEBUG("Could not find intersecting point!");
      if (m_positionOnTrace == TraceGraphPoint::EndPoint) {
       *i_newPoint = rotationTrace->getEndPoint();
      } else if (m_positionOnTrace == TraceGraphPoint::StartPoint) {
        *i_newPoint = rotationTrace->getStartPoint();
      }
    }
    break;
  }
  default : {
    LOG_ERROR("unknown PointPosition");
    break;
  }}
  return true;
}


bool TraceGraphPoint::curveNeedsCorrection
(const RotationTrace::RotationTracePointer& i_rotationTrace,
 const Point2D& i_newPoint) const {
  switch (m_positionOnTrace) {
  case TraceGraphPoint::CenterPoint : {
    try {
      Arc2D(i_rotationTrace->getStartPoint(),
               i_rotationTrace->getEndPoint(),
	    i_newPoint, true);
      return false;
    } catch (std::runtime_error) {}
    return true;
    break;
  }
  case TraceGraphPoint::StartPoint : {
    try {
      Arc2D(i_newPoint,
               i_rotationTrace->getEndPoint(),
	    i_rotationTrace->getCentrePoint(), true);
      return false;
    } catch (std::runtime_error) {}
    return true;
    break;
  }
  case TraceGraphPoint::EndPoint : {
    try {
      Arc2D(i_rotationTrace->getStartPoint(),
               i_newPoint,
	    i_rotationTrace->getCentrePoint(), true);
      return false;
    } catch (std::runtime_error) {}
    return true;
    break;
  }}
  return true;
}


bool TraceGraphPoint::snapPointToOthers(QPointF* i_newPoint) const {
  LOG_DEBUG("snapPointToOthers");
  LOG_DEBUG("new point is: " << i_newPoint->x() << ", " << i_newPoint->y());
  bool hasSnapped = false;
  QGraphicsScene* aScene = scene();
  if (!aScene) {
    return hasSnapped;
  }
  // create a bounding box rectanle at the scene location of the object
  QRectF sceneRect = boundingRect();
  sceneRect.moveTo(scenePos());
  sceneRect.adjust(-TraceGraphPoint::m_searchDistance,
		   -TraceGraphPoint::m_searchDistance,
		   TraceGraphPoint::m_searchDistance,
		   TraceGraphPoint::m_searchDistance);

  for (QGraphicsItem* item : scene()->items(sceneRect, Qt::IntersectsItemBoundingRect)) {
    if (item->type() != TraceGraphPoint::Type) {
      LOG_DEBUG("Found an item which is not a point");
      continue;
    }
    LOG_DEBUG("Found a point");
    int distance = (scenePos() - item->scenePos()).manhattanLength();
    if (distance == 0) {
      LOG_DEBUG("Found Myself!");
      continue;
    }
    if (distance <= TraceGraphPoint::m_searchDistance) {
      LOG_DEBUG("old point is: " << i_newPoint->x() << ", " << i_newPoint->y());
      *i_newPoint = item->scenePos() - m_startPointAtMouseDown;
      LOG_DEBUG("new point is: " << i_newPoint->x() << ", " << i_newPoint->y());
      hasSnapped = true;
      LOG_DEBUG("Has found a companion!!!");
      break;
    }
  }
  return hasSnapped;

}


void TraceGraphPoint::updateSnapToOthers(const bool& i_snapToOthers) {
  m_snapToOthers = i_snapToOthers;
}
