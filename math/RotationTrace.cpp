// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <Point2D.h>
#include <Circle2D.h>
#include <Arc2D.h>
#include "./RotationTrace.h"

RotationTrace::RotationTrace()
  : Trace(Point2D(-1, 0), Point2D(1,0)) {
  m_traceType = Trace::Curve;
    Arc2D arc(m_startPoint, m_endPoint, Point2D(0,0));
  m_centrePoint = arc.getCentrePoint();
  m_isClockwise = arc.getIsClockwise();
}


RotationTrace::RotationTrace(const Point2D& i_startPoint,
                             const Point2D& i_endPoint,
                             const Point2D& i_centrePoint)
  : Trace(i_startPoint, i_endPoint), m_centrePoint(i_centrePoint) {
  m_traceType = Trace::Curve;
  Arc2D arc(i_startPoint, i_endPoint, i_centrePoint);
  m_isClockwise = arc.getIsClockwise();
}


RotationTrace::RotationTrace(const Point2D& i_startPoint,
                             const Point2D& i_endPoint,
                             const traceType& i_radius,
                             const bool& i_isClockwise /*=true*/)
  : Trace(i_startPoint, i_endPoint), m_isClockwise(i_isClockwise) {
  m_traceType = Trace::Curve;
}


RotationTrace::RotationTrace(const Arc2D& i_arc)
    : Trace(i_arc.getFirstPoint(), i_arc.getSecondPoint()) {
  m_traceType = Trace::Curve;
  m_centrePoint = i_arc.getCentrePoint();
  m_isClockwise = i_arc.getIsClockwise();
}


Arc2D RotationTrace::getArc() const {
  return Arc2D(m_startPoint,
               m_endPoint,
               m_centrePoint,
               m_isClockwise);
}


Point2D RotationTrace::intersectingPoint(
    const Point2D& i_currentPosition) const {
  /* calculating the intersecting points leads to solving a quardatic equition, vector style
     where a,b and c are results based on vector shit
     check out:
     http://stackoverflow.com/questions/1073336/circle-line-collision-detection
  */

  Vector2D f = (-1 * m_centrePoint);
  Point2D d = Vector2D(i_currentPosition.x,
                       i_currentPosition.y);

  traceType a = Vector2D::dotProduct(d, d);
  traceType b = 2*(Vector2D::dotProduct(f, d));

  traceType radius = getArc().radius();
  traceType c =
      Vector2D::dotProduct(f, f) - radius * radius;

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
    LOG_DEBUG("T1 : " << t1 <<  ", T2: " << t2);

    if (std::abs(1-t1) < std::abs(1-t2))
      return t1*i_currentPosition;
    else
      return t2*i_currentPosition;
  }
}


std::vector<RotationTrace> RotationTrace::getNecessaryTraces() const {
  Point2D firstExtreme, secondExtreme;
  getPointAtExtremeAngle(&firstExtreme, &secondExtreme);
  traceType firstExtremeAngle=(firstExtreme - m_centrePoint).getAlpha();
  traceType secondExtremeAngle=(secondExtreme - m_centrePoint).getAlpha();
  traceType startAngle=(m_startPoint - m_centrePoint).getAlpha();
  traceType endAngle=(m_endPoint - m_centrePoint).getAlpha();

  Point2D startPoint = m_startPoint;

  std::vector<RotationTrace> itermediateTraces;
  if (shouldAddExtremePoint(startAngle, endAngle, firstExtremeAngle)) {
    itermediateTraces.push_back(
        RotationTrace(startPoint,
                      firstExtreme,
                      m_centrePoint));

    startPoint = firstExtreme;
  }
  if (shouldAddExtremePoint(startAngle, endAngle, secondExtremeAngle)) {
    itermediateTraces.push_back(
        RotationTrace(startPoint,
                      secondExtreme,
                      m_centrePoint));
    startPoint = secondExtreme;
  }

  itermediateTraces.push_back(
      RotationTrace(startPoint,
                    m_endPoint,
                    m_centrePoint));
  return itermediateTraces;
}


bool RotationTrace::shouldAddExtremePoint(const traceType& i_startAngle,
                                          const traceType& i_stopAngle,
                                          const traceType& i_extremeAngle) const {
  bool shouldAddPoint = false;
  traceType span = getArc().spanAngle();
  traceType angle;
  if (m_isClockwise) {
    angle = i_startAngle;
  } else {
    angle = i_stopAngle;
  }

  if (angle > i_extremeAngle && span > (angle - i_extremeAngle)) {
    shouldAddPoint = true;
  }

  if (angle < span) {
    // this means it goes through 0, the angle needs to be adjusted such that it will work
    shouldAddPoint |= shouldAddExtremePoint(i_startAngle + 2 * PI,
                                            i_stopAngle + 2 * PI,
                                            i_extremeAngle);
  }
  return shouldAddPoint;
}


std::vector<Point2D*> RotationTrace::getPointPointers() {
  std::vector<Point2D*> pointers = Trace::getPointPointers();
  pointers.push_back(&m_centrePoint);
  return pointers;
}

Point2D RotationTrace::getPointBetweenStartAndStopPosition() const {
  return RotationTrace::suggestCentralPoint(m_startPoint,
                                            m_endPoint);
}

Point2D RotationTrace::suggestCentralPoint(const Point2D& i_startPoint,
                                           const Point2D& i_endPoint) {
  LOG_DEBUG("start point: " << i_startPoint.x << ", " << i_startPoint.y);
  LOG_DEBUG("end point: " << i_endPoint.x << ", " << i_endPoint.y);
  Point2D xnew = i_endPoint + (i_startPoint - i_endPoint) * 0.5;
  LOG_DEBUG("new point: " << xnew.x << ", " << xnew.y);
  return xnew;
}


void RotationTrace::getStartStopAngle(double* i_startAngle,
                                      double* i_stopAngle) const {
  *i_startAngle = (m_startPoint - m_centrePoint).getAlpha() *
    180. / PI;
  *i_stopAngle = (m_endPoint - m_centrePoint).getAlpha() *
    180. / PI;
}


void RotationTrace::getExtremePoints(std::vector<Point2D>* i_list,
                                     const double* i_angle /* = nullptr*/) const {
   double startAngle, stopAngle, angle;
   if (!i_angle) {
      getStartStopAngle(&startAngle, &stopAngle);
      if (getIsClockwise()) {
         angle = startAngle;
      } else {
         angle = stopAngle;
      }
   } else {
      angle = *i_angle;
   }
   double span = getArc().spanAngle() * 180.0 / PI;
   double radius = getArc().radius();
   if (angle < span) {
      LOG_DEBUG("add 360 deg point");
      i_list->push_back(m_centrePoint + Point2D(radius, 0));
      // this means it goes through 0, the angle needs to be adjusted such that it will work
      // check an angle with +360 degree
      double alterAngle(angle + 360.0);
      getExtremePoints(i_list, &alterAngle);

   }
   if (angle > 90 && span > (angle - 90)) {
      LOG_DEBUG("add 90 deg point");
      i_list->push_back(m_centrePoint + Point2D(0, radius));
   }
   if ( angle > 180 && span > angle - 180) {
      LOG_DEBUG("add 180 deg point");
      i_list->push_back(m_centrePoint + Point2D(-radius, 0));
   }
   if ( angle > 270 && span > angle - 270) {
      LOG_DEBUG("add 270 deg point");
      i_list->push_back(m_centrePoint + Point2D(0, -radius));
   }
}


void RotationTrace::getPointAtExtremeAngle
(Point2D* i_firstPoint,
 Point2D* i_secondPoint) const {
  /**
   * Given a circle
   * gives the two points for which the angle to the origin is
   * minimum or maximum
   */
  traceType centreMagnitude = Magnitude(m_centrePoint);
  traceType radius = getArc().radius();
  if (centreMagnitude > radius) {
    traceType rotationAngle = asin(radius/centreMagnitude);
    LOG_DEBUG("Rotation angle: " << rotationAngle*180/PI);

    *i_firstPoint = m_centrePoint*cos(rotationAngle);
    *i_secondPoint = m_centrePoint*cos(rotationAngle);

    i_secondPoint->rotate(rotationAngle);
    i_firstPoint->rotate(-rotationAngle);
  } else {
    // er ligt ergens een kladje waar dit op staat uitgelegd
    *i_secondPoint = (m_startPoint - m_centrePoint).rotate(PI) + m_centrePoint;
    *i_firstPoint = (m_endPoint - m_centrePoint).rotate(PI) + m_centrePoint;
    if (*i_firstPoint == m_startPoint) {
      // zelfde geld hier
      LOG_DEBUG("Same points, monster!");
      *i_secondPoint = (m_startPoint - m_centrePoint).rotate(PI/2.0) + m_centrePoint;
      *i_firstPoint = (m_endPoint - m_centrePoint).rotate(PI/2.0) + m_centrePoint;
    }
  }
}


std::vector<Point2D> RotationTrace::estimateTrace(const int& i_numberOfPoints) const {
  // if the trace is clockwise reverse it
  if (m_isClockwise) {
    RotationTrace reverseTrace = *this;
    reverseTrace.setIsClockwise(!m_isClockwise);
    reverseTrace.setStartPoint(m_endPoint);
    reverseTrace.setEndPoint(m_startPoint);
    std::vector<Point2D> points(reverseTrace.estimateTrace(i_numberOfPoints));
    std::reverse(points.begin(), points.end());
    return points;
  }

  traceType angle = getArc().spanAngle();
  std::vector<Point2D> points;
  for (traceType currentAngle = 0;
       currentAngle <= angle;
       currentAngle += angle / (i_numberOfPoints - 1)) {
    Point2D normalisedStartPoint = m_startPoint - m_centrePoint;
    points.push_back
      (normalisedStartPoint.rotate(currentAngle) + m_centrePoint);
  }
  return points;
}


void RotationTrace::reverse() {
  Trace::reverse();
  m_isClockwise = !m_isClockwise;
}
