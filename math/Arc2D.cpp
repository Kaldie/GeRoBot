// Copyright [2015] Ruud Cools

#include <macroHeader.h>
#include <Arc2D.h>
#include <Quadrant2D.h>
#include <Circle2D.h>

Arc2D::Arc2D(const Point2D& i_startPoint,
             const Point2D& i_endPoint,
             const Point2D& i_centrePoint,
             const bool& i_isClockwise)
  : Circle2D(i_startPoint, i_centrePoint),
    m_secondPoint(i_endPoint),
    m_isClockwise(i_isClockwise) {
  if (!isPointOnCircle(m_secondPoint)) {
    LOG_DEBUG("Given the first point: " << m_firstPoint <<
	      " and the radius: " << radius() );
    LOG_ERROR("Second point does not lay on the defined circle!");
  }
  m_isClockwise = i_isClockwise;
    }



Arc2D::Arc2D(const Point2D& i_startPoint,
             const Point2D& i_endPoint,
             const traceType& i_radius,
             const bool& i_isClockwise)
  : Circle2D(i_startPoint, i_endPoint, i_radius, i_isClockwise),
    m_secondPoint(i_endPoint),
    m_isClockwise(i_isClockwise)
{}


traceType Arc2D::spanAngle() const {
  if (m_firstPoint == m_secondPoint) {
    return 2 * PI;
  }
  traceType startAngle=(m_firstPoint-m_centrePoint).getAlpha();
  traceType stopAngle=(m_secondPoint-m_centrePoint).getAlpha();
  if (m_isClockwise) {
    if (startAngle < stopAngle)
      return 2 * PI - (stopAngle - startAngle);
    return startAngle-stopAngle;
  } else {
    if (startAngle > stopAngle) {
      return (2 * PI) - (startAngle - stopAngle);
    }
    return stopAngle-startAngle;
  }
}


traceType Arc2D::arcLength() const {
  validate();
  LOG_DEBUG("Startpoint x,y: " << m_firstPoint.x << ", " << m_firstPoint.y);
  LOG_DEBUG("Endpoint x,y: " << m_secondPoint.x << ", " << m_secondPoint.y);
  LOG_DEBUG("is clockwise : " << m_isClockwise);
  LOG_DEBUG("Magnitude is: " << radius());

  traceType angle = spanAngle();
  return angle * radius();
}


bool Arc2D::isPointOnArc(const Point2D& i_point,
			 const bool& i_ignoreRadius /*=false*/) const {
  if (!i_ignoreRadius) {
    if (!isPointOnCircle(i_point)) {
      return false;
    }
  }
  Quadrant2D firstQuadrant(m_firstPoint-m_centrePoint);
  Quadrant2D secondQuadrant(m_secondPoint-m_centrePoint);
  Quadrant2D pointQuadrant(i_point-m_centrePoint);
  // if the requested point lays in the same quadrant as the second point
  if ((firstQuadrant == pointQuadrant) || (secondQuadrant == pointQuadrant)) {
    Point2D requestedPoint(i_point-m_centrePoint);
    bool isOnArc(false);
    if (firstQuadrant == pointQuadrant) {
      isOnArc |= isPointOnArcWithEqualQuadrant((m_firstPoint-m_centrePoint), requestedPoint, m_isClockwise);
    }
    if (secondQuadrant == pointQuadrant) {
      isOnArc |=  isPointOnArcWithEqualQuadrant((m_secondPoint-m_centrePoint), requestedPoint, !m_isClockwise);
    }
    return isOnArc;
  }
  return isPointOnArcWithDifferentQuadrant(firstQuadrant, secondQuadrant, pointQuadrant);
}


bool Arc2D::isPointOnArcWithEqualQuadrant(const Point2D& firstPoint,
					  const Point2D& requestedPoint,
					  const bool& i_isClockwise) const {
  // it is in the same quadrant so we can do
  traceType firstPointAlpha(firstPoint.getAlpha());
  traceType requestedPointAlpha(requestedPoint.getAlpha());
  traceType spannedAngle(spanAngle());
  if (!i_isClockwise) {
    if (firstPointAlpha < requestedPointAlpha) {
      if ((firstPointAlpha + spannedAngle) > requestedPointAlpha) {
	return true;
      }
    }
  } else { // clockwise
    if (firstPointAlpha > requestedPointAlpha) {
      if ((firstPointAlpha - spannedAngle) < requestedPointAlpha) {
	return true;
      }
    }
  }
  return false;
}


bool Arc2D::isPointOnArcWithDifferentQuadrant(const Quadrant2D& firstQuadrant,
					      const Quadrant2D& secondQuadrant,
					      const Quadrant2D& requestedQuadrant) const{
  bool shouldIncrement(PI/2 < spanAngle());
  Quadrant2D quadrant = firstQuadrant;
  while (shouldIncrement ||  (quadrant!= secondQuadrant)) {
    shouldIncrement = false;
    if (quadrant == requestedQuadrant) {
      return true;
    }
    quadrant.increment(m_isClockwise);
  }
  return false;
}


void Arc2D::sortPoints(std::vector<Point2D>::iterator i_start,
		       std::vector<Point2D>::iterator i_end) const {
  std::sort(i_start, i_end, [this](Point2D l, Point2D r) {return compare(l, r);});
}


bool Arc2D::compare(const Point2D& i_first,
		    const Point2D& i_second) const {
  return angularDistanceToStartPoint(i_first) < angularDistanceToStartPoint(i_second);
}


float Arc2D::angularDistanceToStartPoint(const Point2D& i_point) const {
  traceType angle((i_point-m_centrePoint).getAlpha());
  traceType startAngle((m_firstPoint-m_centrePoint).getAlpha());
  traceType distance;
  if (m_isClockwise) {
    distance = startAngle -angle;
  } else {
    distance = angle - startAngle;
  }
  if (distance < 0) {
    distance+= 2 * PI;
  }
  return distance;
}
