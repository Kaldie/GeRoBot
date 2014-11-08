#ifndef Arc2D_H
#define Arc2D_H

#include <Circle2D.h>

class Arc2D{

	GETSET(Point2D,m_startPoint,StartPoint);
	GETSET(Point2D,m_endPoint,EndPoint);
	GETSET(Circle2D,m_circle2D,Circle2D);

 private:
	Arc2D();
	bool validate() const;
	
 public:
	Arc2D(const Point2D& i_startPoint,
				const Point2D& i_endPoint,
				const Circle2D& i_circle2D);
	
	Arc2D(const Point2D& i_startPoint,
				const Point2D& i_endPoint,
				const Point2D& i_centrePoint);
	
	Arc2D(const Point2D& i_startPoint,
				const Point2D& i_endPoint,
				const double& i_radius,
				const bool& i_isClockwise=true);
	
	bool isClockwise() const;

	double arcLength(const Point2D& i_startPoint,
									 const Point2D& i_endPoint) const;

	double arcLength() const;
};

#endif // Arc2D
