//Copyright [2014] Ruud Cools

#include <macroHeader.h>
#include <Point2D.h>
#include <Line2D.h>
#include "./Trace.h"

Trace::Trace()
    : Trace(Point2D(0, 0), Point2D(0, 0.01)) {
}


Trace::Trace(const Point2D& i_startPoint,
             const Point2D& i_endPoint):
    m_traceType(Line),
    m_startPoint(i_startPoint),
    m_endPoint(i_endPoint),
    m_rotationTolerance(0.1),
    m_translationTolerance(0.1) {
  if (!isValidStartAndEndPoint(&i_startPoint, &i_endPoint))
    LOG_ERROR("Start and end position is the same and thus not valid!");
}


Trace::Trace(const Point2D& i_startPoint,
             const Point2D& i_endPoint,
             const Trace::TraceType& i_traceType):
    m_traceType(i_traceType),
    m_startPoint(i_startPoint),
    m_endPoint(i_endPoint),
    m_rotationTolerance(0.1),
    m_translationTolerance(0.1) {
if (!isValidStartAndEndPoint(&i_startPoint,
                             &i_endPoint))
  LOG_ERROR("Start and end position is the same and thus not valid!");
}


bool Trace::isValidStartAndEndPoint(const Point2D *i_startPoint,
                                    const Point2D * i_endPoint) {
  if ((*i_startPoint) == (*i_endPoint)) {
    LOG_WARNING("Start and end coordinates are the same!");
    return false;
  } else {
    return true;
  }
}

Line2D Trace::getTraceLine() const {
  if (m_traceType == Line) {
    LOG_INFO("Start position is: " << m_startPoint.x << "," << m_startPoint.y);
    LOG_INFO("End position is: " << m_endPoint.x << "," << m_endPoint.y);
    return Line2D(m_startPoint, m_endPoint);
  } else {
    return Line2D(Point2D(0, 0), Point2D(0, 0));
  }
}

bool Trace::isWithinRange(const traceType &i_reference,
                          const traceType &i_verification) const {
  if (i_verification+TOLERANCE < i_reference)
    return false;
  else if (i_verification-TOLERANCE>i_reference)
    return false;
  else
    return true;
}


bool Trace::isWithinRange(const Point2D &i_referencePoint,
                          const Point2D &i_verificationPoint) const {
  if (isWithinRange(i_referencePoint.x, i_verificationPoint.x) and
      isWithinRange(i_referencePoint.y, i_verificationPoint.y))
    return true;
  else
    return false;
}


bool Trace::isWithinEndRange(const Point2D &i_point2D) const {
  return isWithinRange(m_endPoint, i_point2D);
}

bool Trace::isWithinBeginRange(const Point2D &i_point2D) const {
  return isWithinRange(m_startPoint, i_point2D);
}


std::string Trace::
  getRotationDirectionToEndPoint(Point2D const &i_point2D) const {
#ifdef DEBUG
  traceType pointAngle = i_point2D.getAlpha()*180/PI;
  traceType endPointAngle = m_endPoint.getAlpha()*180/PI;

  LOG_INFO("robot angle: " << pointAngle
           << ", endpoint angle: " << endPointAngle);
#endif
  return getRotationDirection(i_point2D,
                              m_endPoint);
}

std::string Trace::getRotationDirection(Point2D const &i_currentPoint,
                                        Point2D const &i_desiredPoint) const {
  traceType currentPointAngle = i_currentPoint.getAlpha()*180./PI;
  traceType endPointAngle = m_endPoint.getAlpha()*180./PI;
  LOG_INFO(currentPointAngle << ", " << endPointAngle);

  if (currentPointAngle > endPointAngle)
    return "CW";
  else if (currentPointAngle < endPointAngle)
    return "CCW";
  else
    return "STABLE";
}

std::string Trace::
  getTranslationDirectionToEndPoint(Point2D const &i_point2D) const {
#ifdef DEBUG
  traceType currentPointMagnitude = Magnitude(i_point2D);
  traceType endPointMagnitude = Magnitude(m_endPoint);
  LOG_INFO("Current magnitude:"  << currentPointMagnitude);
  LOG_INFO("desired magnitude: " << endPointMagnitude);
#endif
  return getTranslationDirection(i_point2D, m_endPoint);
}


std::string Trace::
  getTranslationDirection(Point2D const &i_currentPoint,
                          Point2D const &i_desiredPoint) const {
  traceType currentPointMagnitude = Magnitude(i_currentPoint);
  traceType desiredPointMagnitude = Magnitude(i_desiredPoint);
	
  if (currentPointMagnitude < desiredPointMagnitude)
    return "OUT";
  else if (currentPointMagnitude>desiredPointMagnitude)
    return "IN";
  else
    return "STABLE";
}


Point2D Trace::intersectingPoint(const Point2D& i_currentPoint)const {
  return getTraceLine().getIntersectingPoint(i_currentPoint);
}

std::vector<Point2D*> Trace::getPointPointers() {
  std::vector<Point2D*> points;
  points.push_back(&m_startPoint);
  points.push_back(&m_endPoint);
  return points;
}
