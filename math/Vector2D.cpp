#include "Vector2D.h"
#include <macroHeader.h>

Vector2D& Vector2D::rotate(double angle)
{
    double s = sinf(angle);
    double c = cosf(angle);
    
    double nx = c * x - s * y;
    double ny = s * x + c * y;
    
    x = nx;
    y = ny;
    
    return (*this);
}

double Vector2D::getAlpha() const
{
  /* the domein of the atan is slighly different then we want it to be.
     So given the quadrant of the vector, add a value to put it in our wanted domein
  */    
  
  double angle=atan2(y,x);
  if(angle<0)
    angle+=2*PI;

  return angle;
}
