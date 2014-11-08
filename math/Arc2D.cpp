#include <macroHeader.h>
#include <Arc2D.h>
#include <Circle2D.h>

Arc2D::Arc2D(const Point2D& i_startPoint,
						 const Point2D& i_endPoint,
						 const Circle2D& i_circle2D):
	m_startPoint(i_startPoint),
	m_endPoint(i_endPoint),
	m_circle2D(i_circle2D)
{
	if(!m_circle2D.verifyPointOnCircle(m_startPoint))
		LOG_ERROR("Start point does not lay on the defined circle!");
	
	if(!m_circle2D.verifyPointOnCircle(m_endPoint))
		LOG_ERROR("End point does not lay on the defined circle!");
}


Arc2D::Arc2D(const Point2D& i_startPoint,
						 const Point2D& i_endPoint,
						 const Point2D& i_centrePoint):
	m_startPoint(i_startPoint),
	m_endPoint(i_endPoint),
	m_circle2D(i_startPoint,i_endPoint,i_centrePoint)
{}


Arc2D::Arc2D(const Point2D& i_startPoint,
						 const Point2D& i_endPoint,
						 const double& i_radius,
						 const bool& i_isClockwise):
	m_startPoint(i_startPoint),
	m_endPoint(i_endPoint),
	m_circle2D(i_startPoint,i_endPoint,i_radius,i_isClockwise)
{}


bool Arc2D::validate() const {
	if(!m_circle2D.verifyPointOnCircle(m_startPoint))
		LOG_ERROR("Start point does not lay on the circle!");

	if(!m_circle2D.verifyPointOnCircle(m_endPoint))
		LOG_ERROR("Start point does not lay on the circle!");
	
	return true;
}

bool Arc2D::isClockwise()const{
	/*
		In doubt => yes it is clockwise!
	*/

	validate();

	float startAngle=(m_startPoint-m_circle2D.getCentrePoint()).getAlpha();
	float stopAngle=(m_endPoint-m_circle2D.getCentrePoint()).getAlpha();
	LOG_DEBUG("Start and stop angle is:  "<<startAngle<<", "<<stopAngle<<" .");
	LOG_DEBUG("startAngle<=PI: "<<(startAngle<=(PI+0.000001)));
	LOG_DEBUG("stopAngle>startAngle && stopAngle<(PI+startAngle): "<<( stopAngle>startAngle && stopAngle<(PI+startAngle)));

	stopAngle-=startAngle;
	if(stopAngle<0)
		stopAngle+=2*PI;

	if(stopAngle<PI-0.000001)
		return false;
	else
		return true;
}


double Arc2D::arcLength(const Point2D& i_startPoint,
												const Point2D& i_endPoint) const 
{	
	validate();
	
	m_circle2D.verifyPointOnCircle(i_startPoint);
	m_circle2D.verifyPointOnCircle(i_endPoint);

	float startAngle=(i_startPoint-m_circle2D.getCentrePoint()).getAlpha();
	float stopAngle=(i_endPoint-m_circle2D.getCentrePoint()).getAlpha();

	LOG_DEBUG("Startpoint x,y: "<<i_startPoint.x<<", "<<i_startPoint.y);
	LOG_DEBUG("Endpoint x,y: "<<i_endPoint.x<<", "<<i_endPoint.y);
	double angle=startAngle-stopAngle;
	LOG_DEBUG("Anglular difference is: "<<angle*(180/PI));
	LOG_DEBUG("is clockwise : "<<isClockwise());
	LOG_DEBUG("Magnitude is: "<<m_circle2D.radius());

	if(isClockwise()){
		if(startAngle<stopAngle)
			startAngle+=2*PI;

		return (startAngle-stopAngle)*m_circle2D.radius();
	}
	else{
		if(startAngle>stopAngle)
			stopAngle+=2*PI;

		return (stopAngle-startAngle)*m_circle2D.radius();
	}
}


double Arc2D::arcLength()const{
	return arcLength(m_startPoint,m_endPoint);
}
