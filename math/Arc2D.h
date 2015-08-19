// copyright [2015] Ruud Cools

#ifndef MATH_ARC2D_H_
#define MATH_ARC2D_H_

#include <Circle2D.h>
class Point2D;

class Arc2D :public Circle2D {
 private:
  Arc2D();

 public:
  Arc2D(const Point2D& i_firstPoint,
        const Point2D& i_secondPoint,
        const Point2D& i_centrePoint);

  Arc2D(const Point2D& i_firstPoint,
        const Point2D& i_secondPoint,
        const traceType& i_radius,
        const bool& i_isClockwise =true);

  bool isClockwise() const;

  traceType arcLength(const Point2D& i_startPoint,
                   const Point2D& i_endPoint) const;

  traceType arcLength() const;
};

#endif  // MATH_ARC2D_H_
