#ifndef RotationTrace_H
#define RotationTrace_H

#include "Trace.h"

/*
	Rotation trace can be defined in 2 manners:

	Start point, end point, centre point or
	Start point, end point, arc length and isConcave (which is similar to isDirectionCW)
	this is similar to G-code

	Centre point is calculated using stuff on:
	http://cs.stackexchange.com/questions/19970/find-the-centre-of-a-circle-given-two-points-lying-on-it-and-its-radius
	or
	http://math.stackexchange.com/questions/27535/how-to-find-center-of-an-arc-given-start-point-end-point-radius-and-arc-direc

	Arm intersection with the arm will be calculated via:
	http://www.analyzemath.com/CircleEq/circle_line_intersection.html
*/


class RotationTrace:public Trace
{
	GETSET(Point2D,m_centrePoint,CentrePoint);
	// The calculation will be done in parts, each part is a trace
	GETSET(std::vector<RotationTrace>,m_partialTraceVector,PartialTraceVector);
	
 private:
	//almost everything is valid
	bool isValidStartAndEndPosition(const Point2D& i_startPoint,
																	const Point2D& i_endPoint) {return true;};
	
  Point2D getCentrePoint(const Point2D& i_startPoint,
												 const Point2D& i_endPoint,
												 const double& i_radius,
												 const bool& i_isClockwise);

	bool isClockwise(const Point2D& i_startPoint,
									 const Point2D& i_endPoint,
									 const Point2D& i_centrePoint)const;

	//initialise the partial trace vector
	void initialise();

	std::vector<RotationTrace> getNecessaryTraces();

	bool shouldAddExtremePoint(double i_startAngle,
														 double i_stopAngle,
														 double i_extremeAngle);

	//No default constructor allowed
	RotationTrace();

 public:
	double DOR();
	double radius() const;
	double arcLength() const;

	bool isClockwise()const;
	
	virtual Point2D intersectingPoint(const Point2D& i_currentPosition)const;

  //constructors
  RotationTrace(const Point2D& i_startPoint,
								const Point2D& i_endPoint,
								const double& i_radius,
								const bool& i_isConcave=true); /*Could also be interperted as i_isClockwise if P1.  */

	RotationTrace(const Point2D& i_startPoint,
								const Point2D& i_endPoint,
								const Point2D& i_centrePoint);

	void getExtremePoints(Point2D& i_firstExtreme,
												Point2D& i_secondExtreme);


	
};

#endif // RotationTrace
