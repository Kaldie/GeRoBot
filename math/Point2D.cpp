#include <macroHeader.h>
#include "Point2D.h"

double Magnitude(const Vector2D& v)
{
  return (sqrtf(v.x * v.x + v.y * v.y));
}


double InverseMag(const Vector2D& v)
{
  return (1.0 / sqrtf(v.x * v.x + v.y * v.y));
}


Vector2D operator *(double t, const Vector2D& v)
{
  return (Vector2D(t * v.x, t * v.y));
}


Point2D operator *(double t, const Point2D& p)
{
  return (Point2D(t * p.x, t * p.y));
}

double Dot(const Vector2D& v1, const Vector2D& v2)
{
  return (v1 * v2);
}


double SquaredMag(const Vector2D& v)
{
  return (v.x * v.x + v.y * v.y);
}


double Point2D::getAngleToOrigin()
{
  return getAlpha();
}


double Point2D::getAngleBetweenPoints(const Point2D &i_point2D) const
{
	return this->getAlpha()-i_point2D.getAlpha();
}

//const float Point2D::get
