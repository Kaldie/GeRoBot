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


Line2D Trace::getTraceLine() const {
  if (m_traceType == Line) {
    LOG_INFO("Start position is: " << m_startPoint.x << "," << m_startPoint.y);
    LOG_INFO("End position is: " << m_endPoint.x << "," << m_endPoint.y);
    return Line2D(m_startPoint, m_endPoint);
  } else {
    return Line2D(Point2D(0, 0), Point2D(0, 0));
  }
}


bool Trace::operator==(const Trace& rhs) const{
  if (m_startPoint != rhs.m_startPoint)
    return false;
  if (m_endPoint != rhs.m_endPoint)
    return false;
  return true;
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
  traceType currentPointMagnitude = magnitude(i_point2D);
  traceType endPointMagnitude = magnitude(m_endPoint);
  LOG_INFO("Current magnitude:"  << currentPointMagnitude);
  LOG_INFO("desired magnitude: " << endPointMagnitude);
#endif
  return getTranslationDirection(i_point2D, m_endPoint);
}


std::string Trace::
  getTranslationDirection(Point2D const &i_currentPoint,
                          Point2D const &i_desiredPoint) const {
  traceType currentPointMagnitude = magnitude(i_currentPoint);
  traceType desiredPointMagnitude = magnitude(i_desiredPoint);

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


std::vector<Point2D> Trace::estimateTrace
(const int& i_numberOfPoints) const {
  LOG_DEBUG("Trace::estimateTrace()");
  // a vector in the direction of the trace with length with
  // which will be increasing the point with each step
  Vector2D normalisedLine =
    (m_endPoint - m_startPoint).normalize() *
    getTraceLine().getLength() / (i_numberOfPoints -1);
  Point2D currentPoint = m_startPoint;
  std::vector<Point2D> points;
  for (int i = 0;
       i < i_numberOfPoints;
       ++i) {
    points.push_back(currentPoint);
    currentPoint += normalisedLine;
  }
  return points;
}


void Trace::reverse() {
  Point2D newEndPoint = m_startPoint;
  m_startPoint = m_endPoint;
  m_endPoint = newEndPoint;
}


bool Trace::isAbutting(const Trace& i_trace) const {
  // if the trace is equal check if the start and end are at the same spot
  // then they are abutting otherwise nope
  if (*this == i_trace) {
    if (m_startPoint == m_endPoint) {
      return true;
    } else {
      return false;
    }
  }
  // if they are not the same, check if the end or start points are the same
  if (m_startPoint == i_trace.m_startPoint) {
    return true;
  } else  if (m_startPoint == i_trace.m_endPoint) {
    return true;
  } else if (m_endPoint == i_trace.m_startPoint) {
    return true;
  } else if (m_endPoint == i_trace.m_endPoint) {
    return true;
  } else {
    return false;
  }
}


traceType Trace::getPerpendicularDistance(const Point2D& i_position) const {
  /// Determine gradient of the line
  Vector2D gradient(i_position.y, -1 * i_position.x);
  Line2D gradientLine(i_position, i_position + gradient);
  return magnitude(i_position - getTraceLine().getIntersectingPoint(gradientLine));
}
