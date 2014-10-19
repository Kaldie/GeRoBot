#ifndef Trace_h
#define Trace_h

#include <string>
#include <macroHeader.h>
#include <ActuatorDefinitions.h>
#include <Point2D.h>
#include <Line2D.h>

enum TRACE_TYPE{Line,
		Curve};

class Trace
{
 private:

  TRACE_TYPE m_traceType; 
  Point2D m_startPosition;
  Point2D m_endPosition;

  float m_translationTolerance;
  float m_rotationTolerance;
      

  bool isValidStartAndEndPosition(const Point2D*,
				  const Point2D*);

  const bool isWithinRange(const Point2D&, const Point2D&) const;
  const bool isWithinRange(const float&, const float&) const;
  
 public:
  
  //constructors
  Trace();
  Trace(const Point2D&,
	const Point2D&);

  Trace(const Point2D&,
	const Point2D&,
	const TRACE_TYPE&);
  
  //Getters and setters
  const Point2D& getStartPosition() const {return m_startPosition;};
  void setStartPosition(const Point2D& i_point2D){m_startPosition=i_point2D;};

  const Point2D& getEndPosition() const {return m_endPosition;};
  void setEndPosition(const Point2D& i_point2D){m_endPosition=i_point2D;};

  const TRACE_TYPE& getType() const {return m_traceType;};
  void setTraceType(const TRACE_TYPE &i_type){m_traceType=i_type;};

  const float& getRotationTolerance()const{return m_rotationTolerance;};
  void setRotationTolerance(const float& i_value){m_rotationTolerance=i_value;};

  const float& getTranslationTolerance() const {return m_translationTolerance;};
  void setTranslationTolerance(const float& i_value){m_translationTolerance=i_value;};

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
