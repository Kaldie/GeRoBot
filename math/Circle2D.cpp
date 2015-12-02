// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <Point2D.h>
#include <Line2D.h>
#include <Arc2D.h>
#include "Circle2D.h"


Circle2D::Circle2D(const Point2D& i_firstPoint,
                   const Point2D& i_centrePoint)
  : m_firstPoint(i_firstPoint), m_centrePoint(i_centrePoint) {
  validate();
}


Circle2D::Circle2D(const Point2D& i_firstPoint,
                   const Point2D& i_secondPoint,
                   const traceType& i_radius,
                   const bool& i_isClockwise /*=true*/)
  : m_firstPoint(i_firstPoint) {
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
  traceType startAngle=(i_firstPoint-centrePoint).getAlpha();
  traceType stopAngle=(i_secondPoint-centrePoint).getAlpha();

  LOG_DEBUG("Start point x,y "<<i_firstPoint.x<<", "<<i_firstPoint.y);
  LOG_DEBUG("End point x,y "<<i_secondPoint.x<<", "<<i_secondPoint.y);
  LOG_DEBUG("Vector V x,y "<<V.x<<", "<<V.y);

  if(i_isClockwise == (stopAngle <= startAngle)) {
    return centrePoint;
  } else {
    return i_firstPoint + 0.5 * V - h * unitH;
  }
  if(i_isClockwise)
    LOG_ERROR("Could not find a centre point which define a arc were the direction is clockwise");
  else
    LOG_ERROR("Could not find a centre point which define a arc were the direction is counter-clockwise");

  return Point2D();
}


traceType Circle2D::radius() const {
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

std::vector<Point2D*> Circle2D::getPointPointers() {
  std::vector<Point2D*> points;
  points.push_back(&m_firstPoint);
  points.push_back(&m_centrePoint);
  return points;
}


void Circle2D::validate() const {
  if (!isPointOnCircle(m_firstPoint))
    LOG_ERROR("Radius defined by first and second point are not equal!");
}


Point2D Circle2D::nearestIntersection(const Line2D& i_line) const {
  /* calculating the nearest
     intersecting points leads to solving a quardatic equition, vector style
     where a,b and c are results based on vector shit
     check out:
     http://stackoverflow.com/questions/1073336/circle-line-collision-detection
  */
  Vector2D d = i_line.getEndPoint() - i_line.getStartPoint();
  Vector2D f = i_line.getStartPoint() - m_centrePoint;
  traceType thisRadius = radius();

  traceType a = Vector2D::dotProduct(d, d);
  traceType b = 2 * Vector2D::dotProduct(f, d);
  traceType c = Vector2D::dotProduct(f, f) - thisRadius * thisRadius;
  traceType discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    LOG_ERROR("No intersection, discriminant is 0");
  /*
    Line=startPoint+t*(endPoint-(0,0))
    if t > 1 there is an insection only it lies a bit furthur
    if 0<=t<=t it lies within the line piece
    if t<0 it lies backward on the of the line piece
    We simply want the point nearest to the robot position
  */
  }
  if (discriminant == 0) {
    // only 1 solution
    traceType t = (-b - discriminant)/(2*a);
    return t * i_line.getStartPoint();
  } else {
    discriminant = sqrt(discriminant);
    // We will have 2 so
    traceType t1 = (-b - discriminant)/(2*a);
    traceType t2 = (-b + discriminant)/(2*a);
    LOG_DEBUG("T1 : " << t1 <<  ", T2: " << t2);
    if (std::abs(1 - t1) < std::abs(1 - t2)) {
      LOG_DEBUG("Adjustment vector: " << static_cast<Point2D>(t1 * d));
      return t1 * d + i_line.getStartPoint();
    } else {
      LOG_DEBUG("Adjustment vector: " << static_cast<Point2D>(t2 * d));
      return t2 * d + i_line.getStartPoint();
    }
  }

}
