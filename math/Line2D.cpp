#include <macroHeader.h>
#include "Point2D.h"
#include "Line2D.h"

Line2D::Line2D() {
  m_startPoint = Point2D(0, 0);
  m_endPoint = Point2D(0, 0);
};

Line2D::Line2D(Point2D i_startPoint, Point2D i_endPoint) {
  m_startPoint = i_startPoint;
  m_endPoint = i_endPoint;
};


Point2D Line2D::getIntersectingPoint(const Line2D &i_line2D) const {
  Point2D startPoint = i_line2D.getStartPoint();
  Point2D endPoint = i_line2D.getEndPoint();
  traceType xTeller;
  traceType yTeller;
  traceType denominator;
  denominator=((m_startPoint.x-m_endPoint.x) *
               (startPoint.y-endPoint.y) -
               (m_startPoint.y-m_endPoint.y) *
               (startPoint.x-endPoint.x));
  if(denominator == 0) {
    LOG_ERROR("No intersection!!!");
  }
  xTeller=(
      (m_startPoint.x * m_endPoint.y - m_startPoint.y * m_endPoint.x) *
      (startPoint.x - endPoint.x) -
      (startPoint.x * endPoint.y - startPoint.y * endPoint.x) *
      (m_startPoint.x - m_endPoint.x));
  yTeller=(
      (m_startPoint.x * m_endPoint.y - m_startPoint.y * m_endPoint.x) *
      (startPoint.y - endPoint.y) -
      (m_startPoint.y - m_endPoint.y) *
      (startPoint.x * endPoint.y - startPoint.y*endPoint.x));

  Point2D intersectingPoint;
  intersectingPoint.x = xTeller / denominator;
  intersectingPoint.y = yTeller / denominator;
  return intersectingPoint;
}


Point2D Line2D::getIntersectingPoint(const Point2D &i_point) const {
  Line2D line(Point2D(0, 0), i_point);
  return getIntersectingPoint(line);
}

traceType Line2D::getLength() {
  return Magnitude(m_endPoint-m_startPoint);
}


bool Line2D::operator==(const Line2D& i_rhs) const {
  if (m_startPoint != i_rhs.m_startPoint)
    return false;
  if (m_endPoint != i_rhs.m_endPoint)
    return false;
  return true;
}


bool Line2D::intersects(const Line2D& i_line) const {
  Point2D intersectingPoint;
  try {
    intersectingPoint = getIntersectingPoint(i_line);
  } catch (std::runtime_error) {
    LOG_DEBUG("They run in parrelal");
    // try go paralel
    return false;
  }
  bool liesOnFirst = false;
  if (std::min(m_startPoint.x, m_endPoint.x) - TOLERANCE <= intersectingPoint.x &&
      std::max(m_startPoint.x, m_endPoint.x) + TOLERANCE >= intersectingPoint.x &&
      std::min(m_startPoint.y, m_endPoint.y) - TOLERANCE <= intersectingPoint.y &&
      std::max(m_startPoint.y, m_endPoint.y) + TOLERANCE >= intersectingPoint.y) {
    liesOnFirst = true;
  }
  // if it doesnt lie on the first
  Point2D startPoint = i_line.getStartPoint();
  Point2D endPoint = i_line.getEndPoint();
  bool liesOnSecond = false;
  if (std::min(startPoint.x, endPoint.x) - TOLERANCE <= intersectingPoint.x &&
      std::max(startPoint.x, endPoint.x) + TOLERANCE >= intersectingPoint.x &&
      std::min(startPoint.y, endPoint.y) - TOLERANCE <= intersectingPoint.y &&
      std::max(startPoint.y, endPoint.y) + TOLERANCE >= intersectingPoint.y) {
    liesOnSecond = true;
  }

  return liesOnFirst && liesOnSecond;
}
