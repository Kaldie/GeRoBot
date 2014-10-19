#include "Vector2D.h"
#include <ActuatorDefinitions.h>

Vector2D& Vector2D::rotate(float angle)
{
    float s = sinf(angle);
    float c = cosf(angle);
    
    float nx = c * x - s * y;
    float ny = s * x + c * y;
    
    x = nx;
    y = ny;
    
    return (*this);
}

const float Vector2D::getAlpha() const
{
  /* the domein of the atan is slighly different then we want it to be.
     So given the quadrant of the vector, add a value to put it in our wanted domein
  */    
  
  float angle=atan2((float)y,(float)x);
  if(angle<0)
    angle+=2*PI;

  return angle;
}
