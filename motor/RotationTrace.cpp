#include <macroHeader.h>
#include "RotationTrace.h"
#include <Point2D.h>


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
		LOG_ERROR("radius from start point and end point are not equal!\n"
							<<"StartPoint magnitude: "<<startPointMagnitude<<"!= EndPoint magnitude: "<<endPointMagnitude<<"!");
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
	return Magnitude(getStartPoint()-m_centrePoint);
}


double RotationTrace::arcLength()const 
{	
	float startAngle=(getStartPoint()-getCentrePoint()).getAlpha();
	float stopAngle=(getEndPoint()-getCentrePoint()).getAlpha();

	LOG_DEBUG("Startpoint x,y: "<<getStartPoint().x<<", "<<getStartPoint().y);
	LOG_DEBUG("Endpoint x,y: "<<getEndPoint().x<<", "<<getEndPoint().y);
	double angle=startAngle-stopAngle;
	LOG_DEBUG("Anglular difference is: "<<angle*(180/PI));
	LOG_DEBUG("is clockwise : "<<isClockwise());
	LOG_DEBUG("Magnitude is: "<<radius());

	if(isClockwise()){
		if(startAngle<stopAngle)
			startAngle+=2*PI;

		return (startAngle-stopAngle)*radius();
	}
	else{
		if(startAngle>stopAngle)
			stopAngle+=2*PI;

		return (stopAngle-startAngle)*radius();
	}
}


bool RotationTrace::isClockwise(const Point2D& i_startPoint,
																const Point2D& i_endPoint,
																const Point2D& i_centrePoint)const{
	/*
		In doubt => yes it is clockwise!
	*/

	float startAngle=(i_startPoint-i_centrePoint).getAlpha();
	float stopAngle=(i_endPoint-i_centrePoint).getAlpha();
	LOG_DEBUG("Start and stop angle is:  "<<startAngle<<", "<<stopAngle<<" .");
	LOG_DEBUG("startAngle<=PI: "<<(startAngle<=(PI+0.000001)));
	LOG_DEBUG("stopAngle>startAngle && stopAngle<(PI+startAngle): "<<( stopAngle>startAngle && stopAngle<(PI+startAngle)));

	stopAngle-=startAngle;
	if(stopAngle<0)
		stopAngle+=2*PI;

	if(stopAngle<PI)
		return false;
	else
		return true;
}


bool RotationTrace::isClockwise()const{
	//TODO: find prove of this....it seems to hold up in simple examples
	return isClockwise(getStartPoint(),getEndPoint(),getCentrePoint());
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
	
	double discriminant = b*b-4*a*c; //squared
	
	if(discriminant<0)
		throw std::runtime_error("No intersection, discriminant is 0");
	/*
		Line=startPoint+t*(endPoint-(0,0))
		if t > 1 there is an insection only it lies a bit furthur
		if 0<=t<=t it lies within the line piece
		if t<0 it lies backward on the of the line piece		
		We simply want the point nearest to the robot position 
	*/
	else if(discriminant==0){
		discriminant=sqrt(discriminant);
		//only 1 solution
		float t = (-b - discriminant)/(2*a);
		return t*i_currentPosition;
	}	
	
	else{
		discriminant=sqrt(discriminant);
		//We will have 2 so
		float t1 = (-b - discriminant)/(2*a);
		float t2 = (-b + discriminant)/(2*a);
		LOG_DEBUG("T1 : "<<t1<<", T2: "<<t2);
		
		if(std::abs(1-t1)<std::abs(1-t2))
			return t1*i_currentPosition;
		else
			return t2*i_currentPosition;
	}
}

	
void RotationTrace::getExtremePoints(Point2D& i_firstPoint,
																		 Point2D& i_secondPoint){
	/* Given a circle 
		   gives the two points for which the angle to the origin is
			 minimum or maximum
	*/
	if(Magnitude(m_centrePoint)!=0){
		Vector2D perpendicularToCentre(m_centrePoint.y,-1*m_centrePoint.x);
		Vector2D offset=radius()*perpendicularToCentre/Magnitude(m_centrePoint);
		i_firstPoint=m_centrePoint+offset;
		i_secondPoint=m_centrePoint-offset;
	}
	else{
		i_firstPoint=m_centrePoint+Point2D(radius(),0);
		i_secondPoint=m_centrePoint-Point2D(radius(),0);
	}
}


std::vector<RotationTrace> RotationTrace::getNecessaryTraces(){
	Point2D firstExtreme,secondExtreme;
	getExtremePoints(firstExtreme,secondExtreme);
	
	double firstExtremeAngle=firstExtreme.getAlpha();
	double secondExtremeAngle=secondExtreme.getAlpha();
	double startAngle=getStartPoint().getAlpha();
	double endPoint=getEndPoint().getAlpha();
	
	std::vector<RotationTrace> itermediateTraces;
	if(shouldAddExtremePoint(startAngle,endPoint,firstExtremeAngle))
		itermediateTraces.push_back(RotationTrace(getStartPoint(),firstExtreme,getCentrePoint()));
	
	if(shouldAddExtremePoint(startAngle,endPoint,secondExtremeAngle))
		itermediateTraces.push_back(RotationTrace(getStartPoint(),secondExtreme,getCentrePoint()));
	
	itermediateTraces.push_back(*this);
	return itermediateTraces;
}


bool RotationTrace::shouldAddExtremePoint(double i_startAngle,
																					double i_stopAngle,
																					double i_extremeAngle){
	i_stopAngle-=i_startAngle;
	if(i_stopAngle<0)
		i_stopAngle+=2*PI;
		
	i_extremeAngle-=i_startAngle;
	if(i_extremeAngle<0)
		i_extremeAngle+=2*PI;
		
	if(isClockwise()){
		if(i_extremeAngle>i_stopAngle)
			return true;
		else
			return false;
	}
	else{
		if(i_extremeAngle>i_stopAngle)
			return true;
		else
			return false;
	}
}
