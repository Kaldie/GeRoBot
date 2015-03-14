// Copyright [2014] Ruud Cools
#ifndef MOTOR_TRACE_H_
#define MOTOR_TRACE_H_

#include <Point2D.h>
#include <Line2D.h>

class Trace {
 private:
  GETSET(TRACE_TYPE, m_traceType, TraceType);
  GETSET(Point2D, m_startPoint, StartPoint);
  GETSET(Point2D, m_endPoint, EndPoint);

  GETSET(float, m_rotationTolerance, RotationTolerance);
  GETSET(float, m_translationTolerance, TranslationTolerance);

  bool isValidStartAndEndPoint(const Point2D*,
                               const Point2D*);

  bool isWithinRange(const Point2D&, const Point2D&) const;
  bool isWithinRange(const float&, const float&) const;

  // no default constructor!
  Trace();
  virtual ~Trace(){};

 public:
  // constructors
  Trace(const Point2D& i_startPoint,
        const Point2D& i_endPoint);

  Trace(const Point2D& i_startPoint,
        const Point2D& i_endPoint,
        const TRACE_TYPE&);

  // Actual methods
  Line2D getTraceLine() const;

  bool isWithinEndRange(const Point2D&) const;
  bool isWithinBeginRange(const Point2D&) const;
  virtual Point2D intersectingPoint(const Point2D& i_currentPosition)const;

  std::string getRotationDirectionToEndPoint(const Point2D&) const;
  std::string getRotationDirection(const Point2D&, const Point2D&) const;

  std::string getTranslationDirectionToEndPoint(const Point2D&) const;
  std::string getTranslationDirection(const Point2D&, const Point2D&) const;
};

#endif  // MOTOR_TRACE_H_