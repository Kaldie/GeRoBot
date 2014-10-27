#include "Point2D.h"
#include <math.h>

double Point2D::getAngleToOrigin()
{
  return getAlpha();
}

double Point2D::getAngleBetweenPoints(const Point2D &i_point2D) const
{
  return ((*this) -i_point2D).getAlpha();
}

//const float Point2D::get
