#include <cmath>
#include <iostream>
#include <string>
#include <Point2D.h>
#include "Trace.h"


Trace::Trace():
  m_traceType(Line), 
  m_startPosition{0,0},
  m_endPosition{0,1},
  m_rotationTolerance(0.1),
  m_translationTolerance(0.1)
{
    LOG_INFO("Created Trace");
}


Trace::Trace(const Point2D& i_startPosition,
	     const Point2D& i_endPosition):
    m_traceType(Line),
    m_startPosition(i_startPosition),
    m_endPosition(i_endPosition),
    m_rotationTolerance(0.1),
    m_translationTolerance(0.1)
{
  if (!isValidStartAndEndPosition(&i_startPosition,&i_endPosition))
    {
	std::cerr<<"Start and end position is the same and thus not valid!"<<std::endl;
	throw 100;
    }
}


Trace::Trace(const Point2D& i_startPosition,
	     const Point2D& i_endPosition, 
	     const TRACE_TYPE& i_traceType):
    m_traceType(i_traceType),
    m_startPosition(i_startPosition),
    m_endPosition(i_endPosition),
    m_rotationTolerance(0.1),
    m_translationTolerance(0.1)    

{
  if (!isValidStartAndEndPosition(&i_startPosition,&
				  i_endPosition))
      {
	  std::cerr<<"Start and end position is the same and thus not valid!"<<std::endl;
	  throw 100;
      }


}


bool Trace::isValidStartAndEndPosition(const Point2D *i_startPosition,
				       const Point2D * i_endPosition)
{
  if ((*i_startPosition)==(*i_endPosition))
    {
	std::cerr<<"Start and end coordinates are the same!"<<std::endl;
      return false;
    }
  else
    return true;
}

const Line2D Trace::getTraceLine() const
{
  if (m_traceType==Line)
    {
	LOG_INFO("Start position is: "<<m_startPosition.x<<","<<m_startPosition.y);
	LOG_INFO("End position is: "<<m_endPosition.x<<","<<m_endPosition.y);
      return Line2D(m_startPosition,m_endPosition);
    }
  else
    return Line2D(Point2D(0,0),Point2D(0,0));
}

const bool Trace::isWithinRange(const float &i_reference,const float &i_verification) const
{

   if (i_verification+TOLERANCE<i_reference)
     return false;

   else if (i_verification-TOLERANCE>i_reference)
     return false;

   else
     return true;
}


const bool Trace::isWithinRange(const Point2D &i_referencePoint,const Point2D &i_verificationPoint) const
{
  if(isWithinRange(i_referencePoint.x,i_verificationPoint.x) and 
     isWithinRange(i_referencePoint.y,i_verificationPoint.y))
    return true;
  else
    return false;
}


const bool Trace::isWithinEndRange(const Point2D &i_point2D) const
{
  return isWithinRange(m_endPosition,i_point2D);
}

const bool Trace::isWithinBeginRange(const Point2D &i_point2D) const
{
  return isWithinRange(m_startPosition,i_point2D);
}


const std::string Trace::getRotationDirectionToEndPoint(Point2D const &i_point2D) const
{
  #ifndef NDEBUG  
  float pointAngle=i_point2D.getAlpha()*180/PI;
  float endPositionAngle=m_endPosition.getAlpha()*180/PI;
  #endif
  LOG_INFO("robot angle: "<<pointAngle<<", endpoint angle: "<<endPositionAngle);  
  return getRotationDirection(i_point2D,
			      m_endPosition);
}

const std::string Trace::getRotationDirection(Point2D const &i_currentPosition,
					      Point2D const &i_desiredPosition) const
{
  float currentPositionAngle=i_currentPosition.getAlpha()*180./PI;
  float endPositionAngle=m_endPosition.getAlpha()*180./PI;
  LOG_INFO(currentPositionAngle<<", "<<endPositionAngle);
  if(currentPositionAngle>endPositionAngle)
    return "CW";
  else if(currentPositionAngle<endPositionAngle)
    return "CCW";
  else
    return "STABLE";
}

const std::string Trace::getTranslationDirectionToEndPoint(Point2D const &i_point2D) const
{
  #ifndef NDEBUG
  float currentPointMagnitude=Magnitude(i_point2D);
  float endPointMagnitude=Magnitude(m_endPosition);
  #endif
  LOG_INFO("Current magnitude:" <<currentPointMagnitude);
  LOG_INFO("desired magnitude: "<<endPointMagnitude);
  
  return getTranslationDirection(i_point2D,m_endPosition);
}

const std::string Trace::getTranslationDirection(Point2D const &i_currentPosition,
					   Point2D const &i_desiredPosition) const
{
  float currentPointMagnitude=Magnitude(i_currentPosition);
  float desiredPointMagnitude=Magnitude(i_desiredPosition);

  if(currentPointMagnitude<desiredPointMagnitude)
    return "OUT";
  else if (currentPointMagnitude>desiredPointMagnitude)
    return "IN";
  else
    return "STABLE";
}


