
#include <macroHeader.h>
#include "Vector2D.h"
#include "Point2D.h"
#include "Line2D.h"
#include <ByteBuffer.h>

void printPointInfo(Point2D i_point)
{  
  using namespace std;
  cout<<"Angle of point is: "<<i_point.getAlpha()*180/PI<<endl;
}


void testByteBuffer()
{
    ByteBuffer<int> byteBuffer(100);

    for(int i=0;i<100;i++)
	byteBuffer.set(i);
    for(int j=99;j>-1;j--)
	std::cout<<byteBuffer.get()<<std::endl;

}
    



int main()
{
  using namespace std;
  testByteBuffer();
  
  Point2D point1(1,2);
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
    
}
