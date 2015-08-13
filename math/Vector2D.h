#ifndef Vector2D_h
#define Vector2D_h

class Vector2D
{
 public:
  
  traceType   x;
  traceType  y;
  
  Vector2D() {}
        
  Vector2D(traceType r, traceType s)
    {
      x = r;
      y = s;
    }
 
  Vector2D& Set(traceType r, traceType s)
    {
      x = r;
      y = s;
      return (*this);
    }
  
  traceType& operator [](long k)
  {
    return ((&x)[k]);
  }
  
  const traceType& operator [](long k) const
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
  
  Vector2D& operator *=(traceType t)
    {
      x *= t;
      y *= t;
      return (*this);
    }
  
  Vector2D& operator /=(traceType t)
    {
      traceType f = 1.0F / t;
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
  
  Vector2D operator /(traceType t) const
  {
    traceType f = 1.0F / t;
    return (Vector2D(x * f, y * f));
  }

  Vector2D operator *(traceType t) const
    {
      return (Vector2D(x * t, y * t));
    }
  
  traceType operator *(const Vector2D& v) const
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
        
  Vector2D& normalize(void);
      
  Vector2D& rotate(traceType angle);
  traceType getAlpha() const;
};

#endif
