// Copyright [2015] Ruud Cools

#include <macroHeader.h>
#include <Point2D.h>
#include <Arc2D.h>
#include "Circle2D.h"


Circle2D::Circle2D(const Point2D& i_firstPoint,
                   const Point2D& i_secondPoint,
                   const Point2D& i_centrePoint):
    m_firstPoint(i_firstPoint),
    m_secondPoint(i_secondPoint),
    m_centrePoint(i_centrePoint) {
  if (!isPointOnCircle(m_firstPoint) ||
      !isPointOnCircle(m_secondPoint))
    LOG_ERROR("Radius defined by first and second point are not equal!");
}


Circle2D::Circle2D(const Point2D& i_firstPoint,
                   const Point2D& i_secondPoint,
                   const traceType& i_radius,
                   const bool& i_isClockwise /*=true*/):
    m_firstPoint(i_firstPoint),
    m_secondPoint(i_secondPoint) {
  m_centrePoint= this->getCentrePoint(i_firstPoint,
                                      i_secondPoint,
                                      i_radius,
                                      i_isClockwise);
}


Point2D Circle2D::getCentrePoint(const Point2D& i_firstPoint,
                                 const Point2D& i_secondPoint,
                                 const traceType& i_radius,
                                 const bool& i_isClockwise) {
  /**
   * Centre point is calculated using stuff on:
   * http://cs.stackexchange.com/questions/19970/find-the-centre-of-a-circle-given-two-points-lying-on-it-and-its-radius
   * or
   * http://math.stackexchange.com/questions/27535/how-to-find-center-of-an-arc-given-start-point-end-point-radius-and-arc-direc
   */
  Vector2D V = i_secondPoint - i_firstPoint;
  traceType h = sqrt( (i_radius * i_radius) - (Magnitude(V) * Magnitude(V)) / 4);
  Vector2D unitH = 1 / Magnitude(V) * Vector2D(V.y,-1*V.x);

  Point2D centrePoint=i_firstPoint + 0.5 * V + h * unitH;

  LOG_DEBUG("Start point x,y "<<i_firstPoint.x<<", "<<i_firstPoint.y);
  LOG_DEBUG("End point x,y "<<i_secondPoint.x<<", "<<i_secondPoint.y);
  LOG_DEBUG("Vector V x,y "<<V.x<<", "<<V.y);

  if(i_isClockwise==Arc2D(m_firstPoint,m_secondPoint,centrePoint).isClockwise())
    return centrePoint;
	
  centrePoint=i_firstPoint + 0.5 * V - h * unitH;
			
  if(i_isClockwise==Arc2D(m_firstPoint,m_secondPoint,centrePoint).isClockwise())
    return centrePoint;

  if(i_isClockwise)
    LOG_ERROR("Could not find a centre point which define a arc were the direction is clockwise");
  else
    LOG_ERROR("Could not find a centre point which define a arc were the direction is counter-clockwise");

  return Point2D();
}


traceType Circle2D::radius()const {
  return Magnitude(m_firstPoint-m_centrePoint);
}


bool Circle2D::isPointOnCircle(const Point2D& i_point) const {
  traceType pointRadius = Magnitude(i_point-m_centrePoint);
  traceType circleRadius = radius();
  if ((pointRadius+0.001 > circleRadius) &&
     pointRadius-0.001 < circleRadius) {
    return true;
  } else {
    LOG_DEBUG("Point radius: " << pointRadius);
    LOG_DEBUG("Circle radius: " << circleRadius);
    LOG_DEBUG("radius from start point and end point are not equal!");
							
    return false;
  }
}

