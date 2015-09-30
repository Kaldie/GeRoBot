// Copyright [2015] Ruud Cools

#ifndef MATH_LINE2D_H_
#define MATH_LINE2D_H_

#include "./Point2D.h"

class Line2D {
 private:
  GETSET(Point2D, m_startPoint, StartPoint);
  GETSET(Point2D, m_endPoint, EndPoint);

 public:
  bool operator==(const Line2D& i_rhs) const;

  Line2D();
  Line2D(Point2D i_startPoint,
         Point2D i_endPoint);

  /// Get the intersection of the two lines
  bool intersects(const Line2D& i_line) const;

  /// get the intersection of the line and the point
  /// Where the point defines a line through origin
  Point2D getIntersectingPoint(const Point2D&) const;
  Point2D getIntersectingPoint(const Line2D &i_line2D) const;
  /// Get the length of the line defined by the two points
  traceType getLength();
};

#endif  // MATH_LINE2D_H_
