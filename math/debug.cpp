
#include <macroHeader.h>
#include <assert.h>
#include "Vector2D.h"
#include "Point2D.h"
#include "Line2D.h"
#include <ByteBuffer.h>
#include <Arc2D.h>
#include <Circle2D.h>

void testCircle(){
  Point2D firstPoint(0,1),secondPoint(1,0),centrePoint(0,0);

  Circle2D circle(firstPoint,secondPoint,centrePoint);
  assert(circle.radius()==1);
  assert(circle.verifyPointOnCircle(Point2D(-1,0)));

  circle.setFirstPoint(Point2D(0,10));
  circle.setSecondPoint(Point2D(10,0));
  assert(circle.radius()==10);
  assert(!circle.verifyPointOnCircle(Point2D(-1,0)));

  Circle2D circle2(Point2D(1, 2), Point2D(2, 1), 1);
  assert(circle2.getCentrePoint() == Point2D(1, 1));
	
  try {
    Circle2D(firstPoint, Point2D(10, 0), centrePoint);
    assert(false == "Previous statement should trown an first point defines another radius then the second point error!");
  }
  catch(std::runtime_error)
  {
    LOG_DEBUG("Caught the could not make circle with bad points exception!");
  }
}


void testArc2D(){
	
	Arc2D arc(Point2D(0,1),Point2D(1,0),Point2D(0,0));

	assert((arc.arcLength()+0.0001)>(PI/2.0) &&
				 (arc.arcLength()-0.0001)<(PI/2.0));

	assert(arc.isClockwise());

	arc.setStartPoint(Point2D(-2,0));
	arc.setEndPoint(Point2D(2,0));
	arc.setCircle2D(Circle2D(Point2D(-2,0),Point2D(2,0),Point2D(0,0)));

	assert((arc.arcLength()+0.0001)>2*PI &&
				 (arc.arcLength()-0.0001)<2*PI);
	
	assert(arc.isClockwise());

	Circle2D newCircle=arc.getCircle2D();
	newCircle.setCentrePoint(Point2D(0,10));
	arc.setCircle2D(newCircle);

	assert(!arc.isClockwise());

	try{
		Arc2D arc2(Point2D(0,1),Point2D(2,0),Point2D(0,0));
		assert(false);
	}
	catch (std::runtime_error){
		LOG_DEBUG("Caught the start and stop point have a different magnitude compared to the centre point error!");
	}

	Arc2D arc3(Point2D(0,4.0),Point2D(4.0,0),4,true);
	assert(arc3.isClockwise());
	assert(arc3.getCircle2D().getCentrePoint()==Point2D(0,0));

	Arc2D arc4(Point2D(0,4),Point2D(4,0),4,false);
	assert(!arc4.isClockwise());
	assert(arc4.getCircle2D().getCentrePoint()==Point2D(4,4));
}


void testLine2D(){
  Point2D point1(1,2);
  Point2D point2(1,3);
  Vector2D vector(10,10);
  Line2D line1(Point2D(-2,-2),Point2D(2,2));
  Line2D line2(Point2D(2.5,5),Point2D(2.5,-5));
  Point2D intersectingPoint=line1.getIntersectingPoint(line2);
	assert(intersectingPoint==Point2D(2.5,2.5));
}


int main()
{
  using namespace std;
	testByteBuffer();
	testPoint2D();
	testCircle();
	testArc2D();
	testLine2D();
	/*  Point2D point1(1,2);
  Point2D point2(1,3);
  Vector2D vector(10,10);
  Line2D line1(Point2D(-2,-2),Point2D(2,2));
  Line2D line2(Point2D(2.5,5),Point2D(2.5,-5));
  Point2D intersectingPoint=line1.getIntersectingPoint(line2);
  cout<<"intersecting point lies at x: "<<intersectingPoint.x<<" y: "<<intersectingPoint.y<<endl;
  cout<<"Angle to origin is: "<<vector.getAlpha()*180/PI<<" degree"<<endl;
  cout<<"Angle between points is: "<<point2.getAngleBetweenPoints(point1)*180/PI<<" degree"<<endl;

  Point2D point3(1,1);
  printPointInfo(point3);
  point3.Set(-1,1);
  printPointInfo(point3);
  point3.Set(-1,-1);
  printPointInfo(point3);
  point3.Set(1,-1);
  printPointInfo(point3);
	*/
}
