// Copyright [2014] Ruud Cools
#ifndef MATH_TRACE_H_
#define MATH_TRACE_H_

#include <Point2D.h>
#include <Line2D.h>

class Trace {
  GETSETPROTECTED(traceType, m_traceType, TraceType);
  GETSETPROTECTED(Point2D, m_startPoint, StartPoint);
  GETSETPROTECTED(Point2D, m_endPoint, EndPoint);
 public:
  // smart pointer definitions
  typedef std::shared_ptr<Trace> TracePointer;
  typedef std::vector<TracePointer> TracePointerVector;
  typedef std::weak_ptr<Trace> WeakTracePointer;

  enum TraceType{Line,
                 Curve};
  // constructors
  Trace();
  virtual ~Trace(){};

  Trace(const Point2D& i_startPoint,
        const Point2D& i_endPoint);

  // isEqual operator
  bool operator==(const Trace& rhs) const;
  bool operator!=(const Trace& rhs) const {return !operator==(rhs);}

  // Actual methods
  Line2D getTraceLine() const;
  /**
   * Given a point, determine the point on the
   * trace which will be intersected by the position and the base
   * This position is used to "correct" rotations or translation
   * used to get nearer to the end point.
   */
  virtual Point2D intersectingPoint(const Point2D& i_currentPosition) const;

  /**
   * Get direction which the robot needs to rotate to.
   * @param[in] i_currentPosition Point2D Current position of the robot
   */
  std::string getRotationDirectionToEndPoint(const Point2D&
                                             i_currentPosition) const;

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

  std::string getRotationDirection(const Point2D&, const Point2D&) const;

  virtual std::vector<Point2D*> getPointPointers();
  /**
   * give a vector with points that estimate the trace.
   * If the number of points correlate with the number of steps
   * then it gets a first order impression of the actual movement of the robot.
   * @param[in] i_numberOfPoints Number of points on the trace that will be returned
   */
  virtual std::vector<Point2D> estimateTrace(const int& i_numberOfPoints) const;

  /// Reverses the trace: endpoint become beginning and startpoint becomes end
  virtual void reverse();
  /// Returns if the given trace is connected with this trace
  bool isAbutting(const Trace& i_trace) const;
};
#endif  // MATH_TRACE_H_
