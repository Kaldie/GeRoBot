#ifndef Trace_h
#define Trace_h

#include <string>
#include <macroHeader.h>
#include <Point2D.h>
#include <Line2D.h>

enum TRACE_TYPE{Line,
		Curve};

class Trace
{
 private:

  GETSET(TRACE_TYPE,m_traceType,TraceType); 
  GETSET(Point2D,m_startPosition,StartPosition);
  GETSET(Point2D,m_endPosition,EndPosition);

  GETSET(float,m_rotationTolerance,RotationTolerance);
  GETSET(float,m_translationTolerance,TranslationTolerance);


  bool isValidStartAndEndPosition(const Point2D*,
				  const Point2D*);

  const bool isWithinRange(const Point2D&, const Point2D&) const;
  const bool isWithinRange(const float&, const float&) const;
  
 public:
  
  //constructors
  Trace();
  Trace(const Point2D&,  //Start Point
	const Point2D&); //End point

  Trace(const Point2D&,  //Start point
	const Point2D&,  //End point
	const TRACE_TYPE&);
  
  //Actual methods
  const Line2D getTraceLine() const;
  
  const bool isWithinEndRange(const Point2D&) const;
  const bool isWithinBeginRange(const Point2D&) const;
  
  const std::string getRotationDirectionToEndPoint(const Point2D&) const;
  const std::string getRotationDirection(const Point2D&, const Point2D&) const;

  const std::string getTranslationDirectionToEndPoint(const Point2D&) const;
  const std::string getTranslationDirection(const Point2D&, const Point2D&) const;
};

#endif
