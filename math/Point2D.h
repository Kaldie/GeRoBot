#ifndef Point2D_h
#define Point2D_h

#include "Vector2D.h"

class Point2D:public Vector2D
{ 
public:
  //construtor
  Point2D() {};

  Point2D(traceType r, traceType s) : Vector2D(r, s) {};

  Point2D& operator =(const Vector2D& v)
  {
    x = v.x;
    y = v.y;
    return (*this);
  };

  Point2D& operator *=(traceType t)
  {
    x *= t;
    y *= t;
    return (*this);
  };

  Point2D& operator /=(traceType t)
  {
    traceType f = 1.0/ t;
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

  Point2D operator *(traceType t) const
  {
    return (Point2D(x * t, y * t));
  };
  
  Point2D operator /(traceType t) const
  {
    traceType f = 1.0/ t;
    return (Point2D(x * f, y * f));
  };

  bool operator ==(const Point2D& i_other) const ;
  traceType getAngleBetweenPoints(const Point2D&) const;
  traceType getAngleToOrigin();
		
};


traceType Magnitude(const Vector2D& v);

traceType InverseMag(const Vector2D& v);

Vector2D operator *(traceType t, const Vector2D& v);

Point2D operator *(traceType t, const Point2D& p);

traceType Dot(const Vector2D& v1, const Vector2D& v2);

traceType SquaredMag(const Vector2D& v);

#endif
