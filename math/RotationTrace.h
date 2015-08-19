// Copyright [2015] Ruud Cools

#ifndef MATH_ROTATIONTRACE_H_
#define MATH_ROTATIONTRACE_H_

#include <Arc2D.h>
#include "./Trace.h"

/**
 * Rotation trace is a track which defines a part of an arc.
 * Rotation trace can be defined in 2 manners:

 *  Start point, end point, centre point or
 * Start point, end point, arc length and isConcave (which is similar to isDirectionCW)
 * this is similar to G-code

 *  Centre point is calculated using stuff on:
 * http://cs.stackexchange.com/questions/19970/find-the-centre-of-a-circle-given-two-points-lying-on-it-and-its-radius
 * or
 * http://math.stackexchange.com/questions/27535/how-to-find-center-of-an-arc-given-start-point-end-point-radius-and-arc-direc

 * Arm intersection with the arm will be calculated via:
 * http://www.analyzemath.com/CircleEq/circle_line_intersection.html
 */

class RotationTrace:public Trace {
  GETSET(Arc2D, m_arc, Arc);

  /// The calculation will be done in parts, each part is a trace
    GETSET(std::vector<RotationTrace>, m_partialTraceVector, PartialTraceVector);

 private:
    /// bockus function, all points are valid, if the trace can be constructed!
    virtual bool isValidStartAndEndPosition(const Point2D&,
                                            const Point2D&) {return true;}

    bool shouldAddExtremePoint(traceType& i_startAngle,
                               traceType& i_stopAngle,
                               traceType& i_extremePoint) const;

    Point2D* getCentralPointFromArc() const;

public:
    virtual Point2D intersectingPoint(const Point2D& i_currentPosition)const;

    /**
     * constructor
     * @param[in] i_startPoint Start point
     * @param[in] i_endPoint end point
     * @param[in] i_radius Radius of the circle
     * @param[in] i_isClockwise Is the circle clockwise or ccw
     */
    RotationTrace(const Point2D& i_startPoint,
                  const Point2D& i_endPoint,
                  const traceType& i_radius,
                  const bool& i_isClockwise = true);

    /**
     * Default constructor
     */
    RotationTrace();


    /**
     * constructor
     * @param[in] i_startPoint Start point
     * @param[in] i_endPoint end point
     * @param[in] centrePoint Centre of the circle
     */
    RotationTrace(const Point2D& i_startPoint,
                  const Point2D& i_endPoint,
                  const Point2D& i_centrePoint);

    /**
     * constructor
     * @param[in] i_arc the arc that will be traced
     */
    RotationTrace(const Arc2D& i_arc);

    /**
     * Get the extremes of of the cirlce.
     * The extremes are points on the arc
     * with the most extreme angle
     */
    void getExtremePoints(Point2D& i_firstExtreme,
                          Point2D& i_secondExtreme) const;

    /**
     * Split the trace to go from the first to the extremes to the end
     * If the there are one or more extreme points found. Create multiple traces
     * which can be calculated one after each other to get proper arc
     */
    std::vector<RotationTrace> getNecessaryTraces() const;

    std::vector<Point2D*> getPointPointers();
};
#endif  // MATH_ROTATIONTRACE_H_
