// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <Point2D.h>
#include <Circle2D.h>
#include <Arc2D.h>
#include "./RotationTrace.h"
#include <Quadrant2D.h>

RotationTrace::RotationTrace()
  : Trace(Point2D(-1, 0), Point2D(1,0)) {
  m_traceType = Trace::Curve;
  m_centrePoint = Point2D(0,0);
  m_isClockwise = true;
}


RotationTrace::RotationTrace(const Point2D& i_startPoint,
                             const Point2D& i_endPoint,
                             const Point2D& i_centrePoint)
  : Trace(i_startPoint, i_endPoint), m_centrePoint(i_centrePoint) {
  m_traceType = Trace::Curve;
  m_isClockwise = true;
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
  return getArc().
    nearestIntersection(Line2D(Point2D(0, 0), i_currentPosition));
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
  Point2D centralPoint = i_endPoint + (i_startPoint - i_endPoint) * 0.5;
  return centralPoint;
}


void RotationTrace::getStartStopAngle(double* i_startAngle,
                                      double* i_stopAngle) const {
  *i_startAngle = (m_startPoint - m_centrePoint).getAlpha() *
    180. / PI;
  *i_stopAngle = (m_endPoint - m_centrePoint).getAlpha() *
    180. / PI;
}


void RotationTrace::getExtremePoints(std::vector<Point2D>* i_list) const {
  Quadrant2D quadrant(m_startPoint - m_centrePoint);
  Quadrant2D stopQuadrant(m_endPoint - m_centrePoint);
  Arc2D arc(getArc());
  bool spansQuadrants(arc.spanAngle() >= PI);
  double radius = arc.radius();
  Point2D quardrantPoint;
  /* 
     As long as the current quadrant is not equal to the stop quadrant we can add extreme points
     If the quadrant is equal to stop quadrant, we can stop if and only if the spaned angled is bigger or equal then 270 degree
     thus span angle has to be bigger then 90 = PI
     which saves computation time
  */
  while (quadrant != stopQuadrant || spansQuadrants) {
    // stuff we need to do when the start and stop are in the same quadrant
    quardrantPoint = quadrant.getBorderPoint(m_isClockwise) * radius;
    quardrantPoint += m_centrePoint;
    LOG_DEBUG("Adding extreme point: " << quardrantPoint);
    i_list->push_back(quardrantPoint);
    quadrant.increment(m_isClockwise);
    spansQuadrants = false;
  } 
}


void RotationTrace::getPointAtExtremeAngle(Point2D* i_firstPoint,
                                           Point2D* i_secondPoint) const {
  /**
   * Given a circle
   * gives the two points for which the angle to the origin is
   * minimum or maximum
   */
  traceType centreMagnitude = magnitude(m_centrePoint);
  traceType radius = getArc().radius();
  if (centreMagnitude > radius) {
    // determin the angle between the origin and the extreme pointers
    traceType rotationAngle = asin(radius/centreMagnitude);
    // Update the length of the extreme points
    // (they are still on the line of the centre of the cirlce)
    *i_firstPoint = m_centrePoint * cos(rotationAngle);
    *i_secondPoint = m_centrePoint * cos(rotationAngle);
    // Rotate the point to the extreme points
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
  LOG_DEBUG("RotationTrace::estimateTrace()");
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
  LOG_DEBUG("Span angle: " << angle);
  std::vector<Point2D> points;
  for (traceType currentAngle = 0;
       currentAngle <= angle;
       currentAngle += angle / (i_numberOfPoints - 1)) {
    Point2D normalisedStartPoint = m_startPoint - m_centrePoint;
    points.push_back
      (normalisedStartPoint.rotate(currentAngle) + m_centrePoint);
    normalisedStartPoint += m_centrePoint;
  }
  return points;
}


void RotationTrace::reverse() {
  Trace::reverse();
  m_isClockwise = !m_isClockwise;
}


traceType RotationTrace::getPerpendicularDistance
(const Point2D& i_position) const {
  return abs(magnitude(i_position - m_centrePoint) - getArc().radius());
}
