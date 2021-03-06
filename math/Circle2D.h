// Copyright [2015] Ruud Cools

#ifndef MATH_CIRCLE2D_H_
#define MATH_CIRCLE2D_H_

#include <Point2D.h>

class Line2D;  // used as reference

class Circle2D {
  GETSETPROTECTED(Point2D, m_firstPoint, FirstPoint);
  GETSETPROTECTED(Point2D, m_centrePoint, CentrePoint);

 private:
  Circle2D();

  /**
   * Calculate the centre point
   * Centre point is calculated using stuff on:
   * http://cs.stackexchange.com/questions/19970/find-the-centre-of-a-circle-given-two-points-lying-on-it-and-its-radius
   * or
   * http://math.stackexchange.com/questions/27535/how-to-find-center-of-an-arc-given-start-point-end-point-radius-and-arc-direc
   */
  Point2D getCentrePoint(const Point2D& i_firstPoint,
                         const Point2D& i_secondPoint,
                         const traceType& i_radius,
                         const bool& i_isClockwise);
 protected:
  virtual void validate() const;

 public:
  /**
   * Constructor using 3 Points2D
   * @param[in] i_firstPoint Point2D that defines 1 point on the cirlce
   * @param[in] i_secondPoint Point2D that defines the second point
   * @param[in] i_centrePoint Point2D that defines the centre of the circle
   */
  Circle2D(const Point2D& i_firstPoint,
           const Point2D& i_centrePoint);

  /**
   * Constructor using 2 Point2D, radius of the cirlce and if it is clockwise
   * @param[in] i_firstPoint Point2D that defines 1 point on the cirlce
   * @param[in] i_secondPoint Point2D that defines the second point
   * @param[in] i_radius radius of the circle
   * @param[in] i_isClockwise Is the circle clockwise?
   */
  Circle2D(const Point2D& i_firstPoint,
           const Point2D& i_secondPoint,
           const traceType& i_radius,
           const bool& i_isClockwise =true);

  /// returns the radius of the circle
  traceType radius() const;

  /// retuns if the point lies on the circle
  bool isPointOnCircle(const Point2D& i_point) const;

  std::vector<Point2D*> getPointPointers();

  Point2D nearestIntersection(const Line2D& i_line) const;
};

#endif  // MATH_CIRCLE2D_H_
