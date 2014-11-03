#ifndef Vector2D_h
#define Vector2D_h

#include <cmath>

class Vector2D
{
 public:
  
  double   x;
  double  y;
  
  Vector2D() {}
        
  Vector2D(double r, double s)
    {
      x = r;
      y = s;
    }
 
  Vector2D& Set(double r, double s)
    {
      x = r;
      y = s;
      return (*this);
    }
  
  double& operator [](long k)
  {
    return ((&x)[k]);
  }
  
  const double& operator [](long k) const
  {
    return ((&x)[k]);
  }
  
  Vector2D& operator +=(const Vector2D& v)
    {
      x += v.x;
      y += v.y;
      return (*this);
    }
  
  Vector2D& operator -=(const Vector2D& v)
    {
      x -= v.x;
      y -= v.y;
      return (*this);
    }
  
  Vector2D& operator *=(double t)
    {
      x *= t;
      y *= t;
      return (*this);
    }
  
  Vector2D& operator /=(double t)
    {
      double f = 1.0F / t;
      x *= f;
      y *= f;
      return (*this);
    }
  
  Vector2D& operator &=(const Vector2D& v)
    {
      x *= v.x;
      y *= v.y;
      return (*this);
    }
  
  Vector2D operator -(void) const
  {
    return (Vector2D(-x, -y));
  }
  
  Vector2D operator +(const Vector2D& v) const
  {
    return (Vector2D(x + v.x, y + v.y));
  }
  
  Vector2D operator -(const Vector2D& v) const
  {
    return (Vector2D(x - v.x, y - v.y));
  }
  
  Vector2D operator /(double t) const
  {
    double f = 1.0F / t;
    return (Vector2D(x * f, y * f));
  }

  Vector2D operator *(double t) const
    {
      return (Vector2D(x * t, y * t));
    }
  
  double operator *(const Vector2D& v) const
  {
    return (x * v.x + y * v.y);
  }
        
  Vector2D operator &(const Vector2D& v) const
  {
    return (Vector2D(x * v.x, y * v.y));
  }
        
  bool operator ==(const Vector2D& v) const
  {
    return ((x == v.x) && (y == v.y));
  }
        
  bool operator !=(const Vector2D& v) const
  {
    return ((x != v.x) || (y != v.y));
  }
        
  Vector2D& normalize(void)
    {
      return (*this /= sqrt(x * x + y * y));
    }
      
  Vector2D& rotate(double angle);
  double getAlpha() const;
};

#endif
