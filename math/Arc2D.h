// copyright [2015] Ruud Cools

#ifndef MATH_ARC2D_H_
#define MATH_ARC2D_H_

#include <Circle2D.h>
class Point2D;

class Arc2D :public Circle2D {
  GETSET(bool, m_isClockwise, IsClockwise);
 public:
  Arc2D(const Point2D& i_firstPoint,
        const Point2D& i_secondPoint,
        const Point2D& i_centrePoint);

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
 private:
  Arc2D();


};

#endif  // MATH_ARC2D_H_
