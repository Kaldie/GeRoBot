// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <Point2D.h>
#include <Circle2D.h>
#include <Arc2D.h>
#include "./RotationTrace.h"

RotationTrace::RotationTrace()
    : Trace(Point2D(0, 0), Point2D(0, 0.1), Curve),
      m_arc(Arc2D(Point2D(-1, 0), Point2D(0, 1), Point2D(0, 0)))
{}

RotationTrace::RotationTrace(const Point2D& i_startPoint,
                             const Point2D& i_endPoint,
                             const Point2D& i_centrePoint)
    : Trace(i_startPoint, i_endPoint, Curve),
      m_arc(Arc2D(i_startPoint, i_endPoint, i_centrePoint))
{}


RotationTrace::RotationTrace(const Point2D& i_startPoint,
                             const Point2D& i_endPoint,
                             const traceType& i_radius,
                             const bool& i_isClockwise /*=true*/)
    : Trace(i_startPoint, i_endPoint, Curve),
      m_arc(Arc2D(i_startPoint, i_endPoint, i_radius, i_isClockwise))
{}


RotationTrace::RotationTrace(const Arc2D& i_arc)
    : Trace(i_arc.getFirstPoint(), i_arc.getSecondPoint(), Curve),
      m_arc(i_arc)
{}


Point2D RotationTrace::intersectingPoint(
    const Point2D& i_currentPosition) const {
  /* calculating the intersecting points leads to solving a quardatic equition, vector style
     where a,b and c are results based on vector shit
     check out:
     http://stackoverflow.com/questions/1073336/circle-line-collision-detection
  */
  Vector2D f = (-1*this->m_arc.getCentrePoint());
  Point2D d = i_currentPosition;

  traceType a = Dot(d, d);
  traceType b = 2*(Dot(f, d));
  traceType c =
      Dot(f, f) - m_arc.radius() * m_arc.radius();

  traceType discriminant = b * b - 4 * a * c;  // squared

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

  discriminant = sqrt(discriminant);
  if (discriminant == 0) {
    // only 1 solution
    traceType t = (-b - discriminant)/(2*a);
    return t*i_currentPosition;
  } else {
    // We will have 2 so
    traceType t1 = (-b - discriminant)/(2*a);
    traceType t2 = (-b + discriminant)/(2*a);
    LOG_DEBUG("T1 : " << t1 << ", T2: " << t2);

    if (std::abs(1-t1) < std::abs(1-t2))
      return t1*i_currentPosition;
    else
      return t2*i_currentPosition;
  }
}


void RotationTrace::getExtremePoints(Point2D& i_firstPoint,
                                     Point2D& i_secondPoint) const {
  /**
   * Given a circle 
   * gives the two points for which the angle to the origin is
   * minimum or maximum
   */

  Point2D centrePoint = m_arc.getCentrePoint();
  traceType centreMagnitude = Magnitude(centrePoint);
  traceType radius = m_arc.radius();
  if (centreMagnitude != 0) {
    traceType rotationAngle = asin(radius/centreMagnitude);
    LOG_DEBUG("Rotation angle: " << rotationAngle*180/PI);

    i_firstPoint = centrePoint*cos(rotationAngle);
    i_secondPoint = centrePoint*cos(rotationAngle);

    i_secondPoint.rotate(rotationAngle);
    i_firstPoint.rotate(-rotationAngle);

    // i_secondPoint*=cos(rotationAngle);
    // i_firstPoint*=cos(rotationAngle);
  } else {
    i_firstPoint = centrePoint+Point2D(radius, 0);
    i_secondPoint = centrePoint-Point2D(radius, 0);
  }
}


std::vector<RotationTrace> RotationTrace::getNecessaryTraces() const {
  Point2D firstExtreme, secondExtreme;
  getExtremePoints(firstExtreme, secondExtreme);
  Point2D centrePoint = m_arc.getCentrePoint();

  traceType firstExtremeAngle=(firstExtreme-centrePoint).getAlpha();
  traceType secondExtremeAngle=(secondExtreme-centrePoint).getAlpha();
  traceType startAngle=(getStartPoint()-centrePoint).getAlpha();
  traceType endPoint=(getEndPoint()-centrePoint).getAlpha();

  Point2D startPoint = getStartPoint();

  std::vector<RotationTrace> itermediateTraces;
  if (shouldAddExtremePoint(startAngle, endPoint, firstExtremeAngle)) {
    itermediateTraces.push_back(
        RotationTrace(startPoint,
                      firstExtreme,
                      m_arc.getCentrePoint()));

    startPoint = firstExtreme;
  }
  if (shouldAddExtremePoint(startAngle, endPoint, secondExtremeAngle)) {
    itermediateTraces.push_back(
        RotationTrace(startPoint,
                      secondExtreme,
                      m_arc.getCentrePoint()));
    startPoint = secondExtreme;
  }

  itermediateTraces.push_back(
      RotationTrace(startPoint,
                    getEndPoint(),
                    m_arc.getCentrePoint()));
  return itermediateTraces;
}


bool RotationTrace::shouldAddExtremePoint(traceType& i_startAngle,
                                          traceType& i_stopAngle,
                                          traceType& i_extremeAngle) const {

  LOG_DEBUG("Start angle: " << i_startAngle*180/PI);
  LOG_DEBUG("Stop angle: " << i_stopAngle*180/PI);
  LOG_DEBUG("Extreme angle: " << i_extremeAngle*180/PI);

  i_stopAngle-=i_startAngle;
  if (i_stopAngle < 0)
    i_stopAngle+=2*PI;

  i_extremeAngle-=i_startAngle;
  if (i_extremeAngle < 0)
    i_extremeAngle+=2*PI;

  if (m_arc.isClockwise()) {
    if (i_extremeAngle > i_stopAngle)
      return true;
    else
      return false;
  } else {
    if (i_extremeAngle < i_stopAngle)
      return true;
    else
      return false;
  }
}


std::vector<Point2D*> RotationTrace::getPointPointers() {
  std::vector<Point2D*> pointers = Trace::getPointPointers();
  pointers.push_back(m_arc.getPointPointers()[2]);
  return pointers;
}


Point2D RotationTrace::suggestCentralPoint(const Point2D& i_startPoint,
                                           const Point2D& i_endPoint) {
  LOG_DEBUG("start point: " << i_startPoint.x << ", " << i_startPoint.y);
  LOG_DEBUG("end point: " << i_endPoint.x << ", " << i_endPoint.y);
  Point2D xnew = i_endPoint + (i_startPoint - i_endPoint) * 0.5;
  LOG_DEBUG("new point: " << xnew.x << ", " << xnew.y);
  return xnew;
}
