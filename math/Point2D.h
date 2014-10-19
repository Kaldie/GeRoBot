#ifndef Point2D_h
#define Point2D_h

#include <math.h>
#include "Vector2D.h"

class Point2D:public Vector2D
{ 
public:
  //construtor
  Point2D() {};

  Point2D(float r, float s) : Vector2D(r, s) {};

  Point2D& operator =(const Vector2D& v)
  {
    x = v.x;
    y = v.y;
    return (*this);
  };

  Point2D& operator *=(float t)
  {
    x *= t;
    y *= t;
    return (*this);
  };

  Point2D& operator /=(float t)
  {
    float f = 1.0F / t;
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

  Point2D operator *(float t) const
  {
    return (Point2D(x * t, y * t));
  };
  
  Point2D operator /(float t) const
  {
    float f = 1.0F / t;
    return (Point2D(x * f, y * f));
  };

  const float getAngleBetweenPoints(const Point2D&) const;
  const float getAngleToOrigin();
};


inline Vector2D operator *(float t, const Vector2D& v)
{
  return (Vector2D(t * v.x, t * v.y));
}

inline Point2D operator *(float t, const Point2D& p)
{
  return (Point2D(t * p.x, t * p.y));
}

inline float Dot(const Vector2D& v1, const Vector2D& v2)
{
  return (v1 * v2);
}

inline float Magnitude(const Vector2D& v)
{
  return (sqrtf(v.x * v.x + v.y * v.y));
}

inline float InverseMag(const Vector2D& v)
{
  return (1.0F / sqrtf(v.x * v.x + v.y * v.y));
}

inline float SquaredMag(const Vector2D& v)
{
  return (v.x * v.x + v.y * v.y);
}

#endif
