// copyright [2015] Ruud Cools

#ifndef MATH_ARC2D_H_
#define MATH_ARC2D_H_

#include <Circle2D.h>
class Point2D;
class Quadrant2D;

class Arc2D :public Circle2D {
  GETSETPROTECTED(Point2D, m_secondPoint, SecondPoint);
  GETSET(bool, m_isClockwise, IsClockwise);
 public:
  Arc2D(const Point2D& i_startPoint,
	const Point2D& i_endPoint,
	const Point2D& i_centrePoint,
	const bool& i_isClockwise);

  Arc2D(const Point2D& i_firstPoint,
        const Point2D& i_secondPoint,
        const traceType& i_radius,
        const bool& i_isClockwise =true);

  traceType spanAngle() const;
  traceType arcLength() const;
  bool isPointOnArc(const Point2D& i_point,
		    const bool& i_ignoreRadius =false) const;

  /// Return the distance between the start point and the given point
  float angularDistanceToStartPoint(const Point2D& i_point) const;

  /// sort points prioritiing point close to the start point keeping in mind the rotation direction
  void sortPoints(std::vector<Point2D>::iterator i_start,
		  std::vector<Point2D>::iterator i_end) const;
 private:
  Arc2D();
  bool isPointOnArcWithEqualQuadrant(const Point2D& firstPoint,
				     const Point2D& requestedPoint,
				     const bool& i_isClockwise) const;
    
  bool isPointOnArcWithDifferentQuadrant(const Quadrant2D& firstQuadrant,
					 const Quadrant2D& secondQuadrant,
					 const Quadrant2D& requestedQuadrant) const;

  /// comperator which priorities points nearest to the start point
  bool compare(const Point2D& i_first, const Point2D& i_second) const;
};

#endif  // MATH_ARC2D_H_
