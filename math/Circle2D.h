#ifndef Circle2D_H
#define Circle2D_H

#include <Point2D.h>

class Circle2D{
	
	GETSET(Point2D,m_firstPoint,FirstPoint);
	GETSET(Point2D,m_secondPoint,SecondPoint);
	GETSET(Point2D,m_centrePoint,CentrePoint);

 private:
	Circle2D();
	Point2D getCentrePoint(const Point2D& i_firstPoint,
												 const Point2D& i_secondPointy,
												 const double& i_radius,
												 const bool& i_isClockwise);

 public:
	Circle2D(const Point2D& i_firstPoint,
					 const Point2D& i_secondPoint,
					 const Point2D& i_centrePoint);

	Circle2D(const Point2D& i_firstPoint,
					 const Point2D& i_secondPoint,
					 const double& i_radius,
					 const bool& i_isClockwise =true);

	double radius() const;

	bool verifyPointOnCircle(const Point2D& i_point) const;
};

#endif // Circle2D
