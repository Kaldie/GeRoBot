#ifndef Vector2D_h
#define Vector2D_h

#include <math.h>

class Vector2D
{
 public:
  
  float   x;
  float   y;
  
  Vector2D() {}
        
  Vector2D(float r, float s)
    {
      x = r;
      y = s;
    }
 
  Vector2D& Set(float r, float s)
    {
      x = r;
      y = s;
      return (*this);
    }
  
  float& operator [](long k)
  {
    return ((&x)[k]);
  }
  
  const float& operator [](long k) const
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
  
  Vector2D& operator *=(float t)
    {
      x *= t;
      y *= t;
      return (*this);
    }
  
  Vector2D& operator /=(float t)
    {
      float f = 1.0F / t;
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
  
  Vector2D operator /(float t) const
  {
    float f = 1.0F / t;
    return (Vector2D(x * f, y * f));
  }

  Vector2D operator *(float t) const
    {
      return (Vector2D(x * t, y * t));
    }
  
  float operator *(const Vector2D& v) const
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
      return (*this /= sqrtf(x * x + y * y));
    }
      
  Vector2D& rotate(float angle);
  const float getAlpha() const;
};

#endif
