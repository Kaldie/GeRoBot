#include <macroHeader.h>
#include "./Point2D.h"

traceType Magnitude(const Vector2D& v) {
  return (sqrtf(v.x * v.x + v.y * v.y));
}


traceType InverseMag(const Vector2D& v) {
  return (1.0 / sqrtf(v.x * v.x + v.y * v.y));
}


Vector2D operator *(traceType t, const Vector2D& v) {
  return (Vector2D(t * v.x, t * v.y));
}


Point2D operator *(traceType t, const Point2D& p) {
  return (Point2D(t * p.x, t * p.y));
}

traceType Dot(const Vector2D& v1, const Vector2D& v2) {
  return (v1 * v2);
}


traceType SquaredMag(const Vector2D& v) {
  return (v.x * v.x + v.y * v.y);
}


bool Point2D::operator==(const Point2D& i_other) const {
  traceType tolerance = 0.0001;
  if (!(this->x+tolerance > i_other.x &&
        this->x-tolerance < i_other.x))
    return false;

  if (!(this->y+tolerance > i_other.y &&
        this->y-tolerance < i_other.y))
    return false;

  return true;
}


traceType Point2D::getAngleToOrigin() {
  return getAlpha();
}


traceType Point2D::getAngleBetweenPoints(const Point2D &i_point2D) const {
  return this->getAlpha()-i_point2D.getAlpha();
}
