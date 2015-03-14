#ifndef Point2D_h
#define Point2D_h

#include "Vector2D.h"

class Point2D:public Vector2D
{ 
public:
  //construtor
  Point2D() {};

  Point2D(double r, double s) : Vector2D(r, s) {};

  Point2D& operator =(const Vector2D& v)
  {
    x = v.x;
    y = v.y;
    return (*this);
  };

  Point2D& operator *=(double t)
  {
    x *= t;
    y *= t;
    return (*this);
  };

  Point2D& operator /=(double t)
  {
    double f = 1.0/ t;
    x *= f;
    y *= f;
    return (*this);
  };

  Point2D operator -(void) const
  {
    return (Point2D(-x, -y));
  };
	
  Point2D operator +(const Vector2D& v) const
  {
    return (Point2D(x + v.x, y + v.y));
  };

  Point2D operator -(const Vector2D& v) const
  {
    return (Point2D(x - v.x, y - v.y));
  };
	    
  Vector2D operator -(const Point2D& p) const
  {
    return (Vector2D(x - p.x, y - p.y));
  };

  Point2D operator *(double t) const
  {
    return (Point2D(x * t, y * t));
  };
  
  Point2D operator /(double t) const
  {
    double f = 1.0/ t;
    return (Point2D(x * f, y * f));
  };

  bool operator ==(const Point2D& i_other) const ;
  double getAngleBetweenPoints(const Point2D&) const;
  double getAngleToOrigin();
		
};


double Magnitude(const Vector2D& v);

double InverseMag(const Vector2D& v);

Vector2D operator *(double t, const Vector2D& v);

Point2D operator *(double t, const Point2D& p);

double Dot(const Vector2D& v1, const Vector2D& v2);

double SquaredMag(const Vector2D& v);

#endif
