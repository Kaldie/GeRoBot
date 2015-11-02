// Copyright [2015] Ruud Cools

#ifndef MATH_ROTATIONTRACE_H_
#define MATH_ROTATIONTRACE_H_

#include "./Trace.h"
#include "./Arc2D.h"
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

class RotationTrace: public Trace {
    GETSETPROTECTED(Point2D, m_centrePoint, CentrePoint);
    GETSET(bool, m_isClockwise, IsClockwise);

    /// The calculation will be done in parts, each part is a trace
    GETSET(std::vector<RotationTrace>, m_partialTraceVector, PartialTraceVector);

 public:
    typedef std::shared_ptr<RotationTrace> RotationTracePointer;
    Arc2D getArc() const;

    /**
     * Sugestion of its central point based on 2 Point2Ds
     * This method suggest a point that can be validly used as a central point based on 2 other points
     * This method can be used if, for instance, a LineTrace needs to be
     * converted to a RotationTrace and no central point is defined yet.
     */
    static Point2D suggestCentralPoint(const Point2D& i_startPoint,
                                       const Point2D& i_endPoint);

    /**
     * Given a point, determine the point on the
     * trace which will be intersected by the position and the base
     * This position is used to "correct" rotations or translation
     * used to get nearer to the end point.
     */
    virtual Point2D intersectingPoint(const Point2D& i_currentPosition) const;

    /**
     * Get the point between the start and stop position.
     * this point corrispond to the point which is used a suggestion
     * in suggestCentralPoint()
     */
    Point2D getPointBetweenStartAndStopPosition() const;

    /**
     * Split the trace to go from the first to the extremes to the end
     * If the there are one or more extreme points found. Create multiple traces
     * which can be calculated one after each other to get proper arc
     */
    std::vector<RotationTrace> getNecessaryTraces() const;

    std::vector<Point2D*> getPointPointers();

    void getStartStopAngle(double* i_startAngle,
                           double* i_stopAngle) const;

    void getExtremePoints(std::vector<Point2D>* i_list,
                          const double* i_angle = nullptr) const;

    /// Estimate the trace with a std::vector<Point2D> with size of i_numberOfPoints
    virtual std::vector<Point2D> estimateTrace(const int& i_numberOfPoints) const;

    /**
     * Given a line going through the origin and i_position,
     * determine the distance to the trace given a perpendicular line from i_position
     */
    virtual traceType getPerpendicularDistance(const Point2D& i_position) const;

    ///  Reverses the trace: endpoint become beginning and startpoint becomes end
    virtual void reverse();

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
     * @param[in] i_centrePoint Centre of the circle
     */
    RotationTrace(const Point2D& i_startPoint,
                  const Point2D& i_endPoint,
                  const Point2D& i_centrePoint);

    /**
     * constructor
     * @param[in] i_arc the arc that will be traced
     */
    RotationTrace(const Arc2D& i_arc);

 private:
    /// bockus function, all points are valid, if the trace can be constructed!
    virtual bool isValidStartAndEndPosition(const Point2D&,
                                            const Point2D&) {return true;}

    bool shouldAddExtremePoint(const traceType& i_startAngle,
                               const traceType& i_stopAngle,
                               const traceType& i_extremePoint) const;

    Point2D* getCentralPointFromArc() const;

    /**
     * The arguments will contain points which intersects the trace at extreme angles
     * Calculates and returns points which intesect the trace at the maximum and minimum angle
     * The angle is defined as the angle a line intersecting the
     * circle and the origin of the coordinate system.
     * @param[out] i_firstExtreme First extreme point
     * @param[out] i_secondExtreme Second extreme point
     */
    void getPointAtExtremeAngle(Point2D* i_firstExtreme,
                                Point2D* i_secondExtreme) const;
};
#endif  // MATH_ROTATIONTRACE_H_
