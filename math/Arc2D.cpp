// Copyright [2015] Ruud Cools

#include <macroHeader.h>
#include <Arc2D.h>
#include <Circle2D.h>

Arc2D::Arc2D(const Point2D& i_startPoint,
             const Point2D& i_endPoint,
             const Point2D& i_centrePoint):
    Circle2D(i_startPoint,
             i_endPoint,
             i_centrePoint)
{}


Arc2D::Arc2D(const Point2D& i_startPoint,
             const Point2D& i_endPoint,
             const traceType& i_radius,
             const bool& i_isClockwise):
    Circle2D(i_startPoint,
             i_endPoint,
             i_radius,
             i_isClockwise)
{}


bool Arc2D::isClockwise() const {
  /*
    In doubt => yes it is clockwise!
  */

  validate();

  traceType startAngle=(m_firstPoint-m_centrePoint).getAlpha();
  traceType stopAngle=(m_secondPoint-m_centrePoint).getAlpha();
  LOG_DEBUG("Start and stop angle is:  " <<
            startAngle << ", " << stopAngle << " .");
  LOG_DEBUG("startAngle <= PI: " << (startAngle <= (PI+0.000001)));
  LOG_DEBUG("stopAngle>startAngle && stopAngle<(PI+startAngle): "
            << (stopAngle>startAngle && stopAngle < (PI+startAngle)));

  stopAngle-=startAngle;
  if (stopAngle < 0)
    stopAngle+=2*PI;

  if (stopAngle < PI-0.000001)
    return false;
  else
    return true;
}


traceType Arc2D::arcLength(const Point2D& i_startPoint,
                        const Point2D& i_endPoint) const  {
  validate();
  isPointOnCircle(i_startPoint);
  isPointOnCircle(i_endPoint);

  traceType startAngle=(m_firstPoint-m_centrePoint).getAlpha();
  traceType stopAngle=(m_secondPoint-m_centrePoint).getAlpha();

  LOG_DEBUG("Startpoint x,y: " << i_startPoint.x << ", " << i_startPoint.y);
  LOG_DEBUG("Endpoint x,y: " << i_endPoint.x << ", " << i_endPoint.y);
  traceType angle = startAngle - stopAngle;
  LOG_DEBUG("Anglular difference is: " << angle*(180/PI));
  LOG_DEBUG("is clockwise : " << isClockwise());
  LOG_DEBUG("Magnitude is: " << radius());

  if (isClockwise()) {
    if (startAngle < stopAngle)
      startAngle+=2*PI;
    return (startAngle-stopAngle) * radius();
  } else {
    if (startAngle > stopAngle)
      stopAngle+=2*PI;
    return (stopAngle-startAngle) * radius();
  }
}


traceType Arc2D::arcLength() const {
  return arcLength(m_firstPoint, m_secondPoint);
}
