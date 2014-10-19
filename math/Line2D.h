#ifndef Line2D_h
#define Line2D_h
#include "Point2D.h"

class Line2D
{
 private:
  Point2D m_startPoint;
  Point2D m_endPoint;
  
 public:
  Line2D()
    {
      m_startPoint=Point2D(0,0);
      m_endPoint=Point2D(0,0);
    };

  Line2D(Point2D i_startPoint, Point2D i_endPoint)
    {
      m_startPoint=i_startPoint;
      m_endPoint=i_endPoint;
    };

  const Point2D getStartPoint() const {return m_startPoint;};
  const Point2D getEndPoint() const {return m_endPoint;};
  const Point2D getIntersectingPoint(const Line2D&) const;
  const Point2D getIntersectingPoint(const Point2D&) const;
  const float getLength();
};

#endif
