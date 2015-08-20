// Copyright [2014] Ruud Cools
#ifndef MATH_TRACE_H_
#define MATH_TRACE_H_

#include <Point2D.h>
#include <Line2D.h>

class Trace {
 public:
  // smart pointer definitions
  typedef std::shared_ptr<Trace> TracePointer;
  typedef std::weak_ptr<Trace> WeakTracePointer;

  enum TraceType{Line,
                 Curve};
  // constructors
  Trace();
  virtual ~Trace(){};

  Trace(const Point2D& i_startPoint,
        const Point2D& i_endPoint);

  Trace(const Point2D& i_startPoint,
        const Point2D& i_endPoint,
        const TraceType&);

  // Actual methods
  Line2D getTraceLine() const;

  bool isWithinEndRange(const Point2D&) const;
  bool isWithinBeginRange(const Point2D&) const;
  virtual Point2D intersectingPoint(const Point2D& i_currentPosition) const;

  /**
   * Get direction which the robot needs to rotate to.
   * @param[in] i_currentPosition Point2D Current position of the robot
   */
  std::string getRotationDirectionToEndPoint(const Point2D&) const;

  /**
   * Translation direction towards the end poin
   * @param[in] i_currentPosition Point2D current position of the robot
   */
  std::string getTranslationDirectionToEndPoint(
      const Point2D& i_currentPosition) const;

  /**
   * Return the direction which the joint needs to move
   * @param[in] i_currentPosition Point2D, position the robot is now
   * @param[in] i_desiredPosition Point2D, position the robot needs to go
   */
  std::string getTranslationDirection(const Point2D& i_currentPosition,
                                      const Point2D& i_desiredPosition) const;

  virtual std::vector<Point2D*> getPointPointers();

 private:
  GETSET(TraceType, m_traceType, TraceType);
  GETSET(Point2D, m_startPoint, StartPoint);
  GETSET(Point2D, m_endPoint, EndPoint);
  GETSET(double, m_rotationTolerance, RotationTolerance);
  GETSET(double, m_translationTolerance, TranslationTolerance);

  bool isValidStartAndEndPoint(const Point2D*,
                               const Point2D*);

  bool isWithinRange(const Point2D&, const Point2D&) const;
  bool isWithinRange(const traceType&, const traceType&) const;
  std::string getRotationDirection(const Point2D&, const Point2D&) const;
};
#endif  // MATH_TRACE_H_
