#ifndef Trace_h
#define Trace_h

#include <Point2D.h>
#include <Line2D.h>

class Trace
{
 private:

  GETSET(TRACE_TYPE,m_traceType,TraceType); 
  GETSET(Point2D,m_startPoint,StartPoint);
  GETSET(Point2D,m_endPoint,EndPoint);

  GETSET(float,m_rotationTolerance,RotationTolerance);
  GETSET(float,m_translationTolerance,TranslationTolerance);

	bool isValidStartAndEndPoint(const Point2D*,
															 const Point2D*);
	
	bool isWithinRange(const Point2D&, const Point2D&) const;
  bool isWithinRange(const float&, const float&) const;
  
 public:
  
  //constructors
  Trace();
  Trace(const Point2D&,  //Start Point
				const Point2D&); //End point
	
  Trace(const Point2D&,  //Start point
				const Point2D&,  //End point
				const TRACE_TYPE&);
  
  //Actual methods
  Line2D getTraceLine() const;
  
  bool isWithinEndRange(const Point2D&) const;
	bool isWithinBeginRange(const Point2D&) const;

	virtual Point2D intersectingPoint(const Point2D& i_currentPosition)const;
  
  std::string getRotationDirectionToEndPoint(const Point2D&) const;
	std::string getRotationDirection(const Point2D&, const Point2D&) const;
	
	std::string getTranslationDirectionToEndPoint(const Point2D&) const;
	std::string getTranslationDirection(const Point2D&, const Point2D&) const;
};

#endif
