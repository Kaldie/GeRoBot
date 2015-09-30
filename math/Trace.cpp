//Copyright [2014] Ruud Cools

#include <macroHeader.h>
#include <Point2D.h>
#include <Line2D.h>
#include "./Trace.h"

Trace::Trace()
    : Trace(Point2D(0, 0), Point2D(0, 0.01)) {
}


Trace::Trace(const Point2D& i_startPoint,
             const Point2D& i_endPoint):
    m_traceType(Line),
    m_startPoint(i_startPoint),
    m_endPoint(i_endPoint) {
}


Trace::Trace(const Point2D& i_startPoint,
             const Point2D& i_endPoint,
             const Trace::TraceType& i_traceType):
    m_traceType(i_traceType),
    m_startPoint(i_startPoint),
    m_endPoint(i_endPoint) {
}


Line2D Trace::getTraceLine() const {
  if (m_traceType == Line) {
    LOG_INFO("Start position is: " << m_startPoint.x << "," << m_startPoint.y);
    LOG_INFO("End position is: " << m_endPoint.x << "," << m_endPoint.y);
    return Line2D(m_startPoint, m_endPoint);
  } else {
    return Line2D(Point2D(0, 0), Point2D(0, 0));
  }
}


std::string Trace::
  getRotationDirectionToEndPoint(Point2D const &i_point2D) const {
#ifdef DEBUG
  traceType pointAngle = i_point2D.getAlpha()*180/PI;
  traceType endPointAngle = m_endPoint.getAlpha()*180/PI;

  LOG_INFO("robot angle: " << pointAngle
           << ", endpoint angle: " << endPointAngle);
#endif
  return getRotationDirection(i_point2D,
                              m_endPoint);
}

std::string Trace::getRotationDirection(Point2D const &i_currentPoint,
                                        Point2D const &i_desiredPoint) const {
  traceType currentPointAngle = i_currentPoint.getAlpha()*180./PI;
  traceType endPointAngle = m_endPoint.getAlpha()*180./PI;
  LOG_INFO(currentPointAngle << ", " << endPointAngle);

  if (currentPointAngle > endPointAngle)
    return "CW";
  else if (currentPointAngle < endPointAngle)
    return "CCW";
  else
    return "STABLE";
}

std::string Trace::
  getTranslationDirectionToEndPoint(Point2D const &i_point2D) const {
#ifdef DEBUG
  traceType currentPointMagnitude = Magnitude(i_point2D);
  traceType endPointMagnitude = Magnitude(m_endPoint);
  LOG_INFO("Current magnitude:"  << currentPointMagnitude);
  LOG_INFO("desired magnitude: " << endPointMagnitude);
#endif
  return getTranslationDirection(i_point2D, m_endPoint);
}


std::string Trace::
  getTranslationDirection(Point2D const &i_currentPoint,
                          Point2D const &i_desiredPoint) const {
  traceType currentPointMagnitude = Magnitude(i_currentPoint);
  traceType desiredPointMagnitude = Magnitude(i_desiredPoint);

  if (currentPointMagnitude < desiredPointMagnitude)
    return "OUT";
  else if (currentPointMagnitude > desiredPointMagnitude)
    return "IN";
  else {
    LOG_DEBUG("Current magnitude: " << currentPointMagnitude);
    LOG_DEBUG("Desired point magnitude: " << desiredPointMagnitude);
    return "STABLE";
  }
}


Point2D Trace::intersectingPoint(const Point2D& i_currentPoint) const {
  return getTraceLine().getIntersectingPoint(i_currentPoint);
}

std::vector<Point2D*> Trace::getPointPointers() {
  std::vector<Point2D*> points;
  points.push_back(&m_startPoint);
  points.push_back(&m_endPoint);
  return points;
}
