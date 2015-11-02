// Copyright [2015] Ruud Cools

#include <macroHeader.h>
#include <Arc2D.h>
#include <Circle2D.h>

Arc2D::Arc2D(const Point2D& i_startPoint,
             const Point2D& i_endPoint,
             const Point2D& i_centrePoint)
  : Circle2D(i_startPoint, i_centrePoint), m_secondPoint(i_endPoint) {
  traceType startAngle = (m_firstPoint-m_centrePoint).getAlpha();
  LOG_DEBUG("m_firstPoint: " << m_firstPoint.x << " , " << m_firstPoint.y);
  LOG_DEBUG("Start angle: " << startAngle);
  traceType stopAngle = (m_secondPoint-m_centrePoint).getAlpha();
  LOG_DEBUG("Stop angle: " << stopAngle);
  if (stopAngle >= startAngle) {
    m_isClockwise = false;
  } else {
    m_isClockwise = true;
  }
}


Arc2D::Arc2D(const Point2D& i_startPoint,
             const Point2D& i_endPoint,
             const Point2D& i_centrePoint,
             const bool& i_isClockwise)
  : Circle2D(i_startPoint, i_centrePoint),
    m_secondPoint(i_endPoint),
    m_isClockwise(i_isClockwise)
{}


Arc2D::Arc2D(const Point2D& i_startPoint,
             const Point2D& i_endPoint,
             const traceType& i_radius,
             const bool& i_isClockwise)
  : Circle2D(i_startPoint, i_endPoint, i_radius, i_isClockwise),
    m_secondPoint(i_endPoint),
    m_isClockwise(i_isClockwise)
{}


traceType Arc2D::spanAngle() const {
  if (m_firstPoint == m_secondPoint) {
    return 2 * PI;
  }
  traceType startAngle=(m_firstPoint-m_centrePoint).getAlpha();
  traceType stopAngle=(m_secondPoint-m_centrePoint).getAlpha();
  LOG_DEBUG("Is clockwise: " << m_isClockwise);
  if (m_isClockwise) {
    if (startAngle < stopAngle)
      return 2 * PI - (stopAngle - startAngle);
    return startAngle-stopAngle;
  } else {
    if (startAngle > stopAngle) {
      LOG_DEBUG("taking the long route!");
      return (2 * PI) - (startAngle - stopAngle);
    }
    LOG_DEBUG("taking the short route!");
    return stopAngle-startAngle;
  }
}

traceType Arc2D::arcLength() const {
  validate();
  LOG_DEBUG("Startpoint x,y: " << m_firstPoint.x << ", " << m_firstPoint.y);
  LOG_DEBUG("Endpoint x,y: " << m_secondPoint.x << ", " << m_secondPoint.y);
  LOG_DEBUG("is clockwise : " << m_isClockwise);
  LOG_DEBUG("Magnitude is: " << radius());

  traceType angle = spanAngle();
  return angle * radius();
}
