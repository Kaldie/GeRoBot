#include <macroHeader.h>
#include "Vector2D.h"


Vector2D& Vector2D::rotate(traceType angle)
{
    traceType s = sinf(angle);
    traceType c = cosf(angle);
    
    traceType nx = c * x - s * y;
    traceType ny = s * x + c * y;
    
    x = nx;
    y = ny;
    
    return (*this);
}


/**
 * Get the angle of a vector, make sure its positive
 * the domein of the atan is slighly different then we want it to be.
 *  So given the quadrant of the vector, add a value to put it in our wanted domein
 */
traceType Vector2D::getAlpha() const {
  if((x * x + y * y) < 0.001) {
    std::cout<<"0 to 0";
    return 0;
  }

  traceType angle=atan2(y,x);
  if(angle<0)
    angle+=2*PI;
  return angle;
}

Vector2D Vector2D::normalize(void) {
  return (*this /= sqrt(x * x + y * y));
}
