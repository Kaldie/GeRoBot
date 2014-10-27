#ifndef Line2D_h
#define Line2D_h
#include "Point2D.h"
#include <macroHeader.h>

class Line2D
{
 private:
  GETSET(Point2D,m_startPoint,StartPoint);
  GETSET(Point2D,m_endPoint,EndPoint);
  
 public:
  Line2D();
	Line2D(Point2D i_startPoint, Point2D i_endPoint);
	
	Point2D getIntersectingPoint(const Line2D&) const;
	Point2D getIntersectingPoint(const Point2D&) const;
	float getLength();
};

#endif
