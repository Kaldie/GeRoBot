#include <Point2D.h>
#include "RotationTrace.h"


RotationTrace::RotationTrace(const Point2D& i_startPoint,
														 const Point2D& i_endPoint,
														 const Point2D& i_centrePoint):
	Trace(i_startPoint,i_endPoint),
	m_centrePoint(i_centrePoint)
{
	double startPointMagnitude=Magnitude(i_startPoint-i_centrePoint);
	double endPointMagnitude=Magnitude(i_endPoint-i_centrePoint);
	
	if((startPointMagnitude+0.001>endPointMagnitude) &&
		  startPointMagnitude-0.001<endPointMagnitude){
		LOG_DEBUG("Magnitude are the same");
	}
	else{
		LOG_DEBUG("Start point magnitude: "<<Magnitude(i_startPoint-i_centrePoint));
		LOG_DEBUG("End point magnitude: "<<Magnitude(i_endPoint-i_centrePoint));
		LOG_ERROR("radius from start point and end point are not equal, which makes it an elips?");
	}
	
	setTraceType(Curve);
}


RotationTrace::RotationTrace(const Point2D& i_startPoint,
														 const Point2D& i_endPoint,
														 const double& i_radius,
														 const bool& i_isConcave /*= true*/):
	Trace(i_startPoint,i_endPoint)
{
	setTraceType(Curve);
	m_centrePoint= this->getCentrePoint(i_startPoint,i_endPoint,i_radius,i_isConcave);
}


Point2D RotationTrace::getCentrePoint(const Point2D& i_startPoint,
																			const Point2D& i_endPoint,
																			const double& i_radius,
																			const bool& i_isConcave){
	/*
		Centre point is calculated using stuff on:
    http://cs.stackexchange.com/questions/19970/find-the-centre-of-a-circle-given-two-points-lying-on-it-and-its-radius
	  or
		http://math.stackexchange.com/questions/27535/how-to-find-center-of-an-arc-given-start-point-end-point-radius-and-arc-direc
	*/
	
	Vector2D V = i_endPoint - i_startPoint;
	LOG_DEBUG("End point x,y "<<i_endPoint.x<<", "<<i_endPoint.y);
	LOG_DEBUG("Start point x,y "<<i_startPoint.x<<", "<<i_startPoint.y);
	LOG_DEBUG("Vector V x,y "<<V.x<<", "<<V.y);
	double h = sqrt( (i_radius * i_radius) - (Magnitude(V) * Magnitude(V) / 4));
	Vector2D unitH = 1 / Magnitude(V) * Vector2D(V.y,-1*V.x);
	
	if(i_isConcave)
		return i_startPoint + 0.5 * V + h * unitH;
	else
		return i_startPoint + 0.5 * V - h * unitH;
}


double RotationTrace::radius()const {
	return Magnitude(this->getStartPoint()-m_centrePoint);
}


double RotationTrace::arcLength()const 
{	
	LOG_DEBUG("Startpoint x,y"<<getStartPoint().x<<", "<<getStartPoint().y);
	LOG_DEBUG("Endpoint x,y"<<getEndPoint().x<<", "<<getEndPoint().y);
	double angle=getStartPoint().getAngleBetweenPoints(getEndPoint());
	LOG_DEBUG("Anglular difference is: "<<angle*(180/PI));
	return angle*radius();
}


bool RotationTrace::isConcave(const Point2D& i_startPoint,
															const Point2D& i_centrePoint)const{
	float anlgeBetweenStartAndCentre=(getStartPoint()-getCentrePoint()).getAlpha();
	LOG_DEBUG("Angle between start and stop is "<<anlgeBetweenStartAndCentre*180/PI<<" degree.");
	if(anlgeBetweenStartAndCentre>(180*PI/180))
		return false;
	else
		return true;
}

bool RotationTrace::isConcave()const{
	//TODO: find prove of this....it seems to hold up in simple examples
	return isConcave(getStartPoint(),getCentrePoint());
}


Point2D RotationTrace::intersectingPoint(const Point2D& i_currentPosition)const {
	/* calculating the intersecting points leads to solving a quardatic equition, vector style
		 where a,b and c are results based on vector shit
		 check out:
		 http://stackoverflow.com/questions/1073336/circle-line-collision-detection
	*/
	Vector2D f=(-1*this->getCentrePoint());
	Point2D d=i_currentPosition;
	
	double a=Dot(d,d);
	double b=2*(Dot(f,d));
	double c=Dot(f,f) - radius()*radius();
	
	double discriminant = b*b-4*a*c;
	
	if(discriminant<0)
		throw std::runtime_error("Discriminant is 0");
	/*
		Line=startPoint+t*(endPoint-(0,0))
		if t > 1 there is an insection only it lies a bit furthur
		if 0<=t<=t it lies within the line piece
		if t<0 it lies backward on the of the line piece		
	*/
	else if(discriminant==0){
		discriminant=sqrt(discriminant);
		//only 1 solution
		float t = (-b - discriminant)/(2*a);
		return i_currentPosition+t*i_currentPosition;
	}	

	
	else{
		discriminant=sqrt(discriminant);
		//We will have 2 so
		float t1 = (-b - discriminant)/(2*a);
		float t2 = (-b + discriminant)/(2*a);
		
		if(t1>t2)
			return t1*i_currentPosition;
		else
			return t2*i_currentPosition;
	}
}
