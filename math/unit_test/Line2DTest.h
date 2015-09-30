// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_LINE2DTEST_H_
#define MATH_UNIT_TEST_LINE2DTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include "../Point2D.h"
#include "../Circle2D.h"
#include "../Arc2D.h"
#include "../Line2D.h"

class Line2DTest : public CxxTest::TestSuite {
 public:
  Point2D startPoint;
  Point2D endPoint;


  void setUp() {
    startPoint = Point2D(0, 10);
    endPoint = Point2D(10, 10);
  }


  void testCreation() {
    Line2D line(startPoint,
                endPoint);
  }

  
  void testLength() {
    Line2D line(startPoint,
                endPoint);
    TS_ASSERT_EQUALS(line.getLength(), 10);
  }


  void testIntersectionLine() {
     Line2D line(startPoint,
                endPoint);
     Line2D line2(Point2D(4, 0),
                  Point2D(4, 40));
     Point2D intesection = line.getIntersectingPoint(line2);

     TS_ASSERT_EQUALS(intesection, Point2D(4, 10));
     TS_ASSERT_DIFFERS(intesection, Point2D(5, 10));
     TS_ASSERT_EQUALS(line2.getIntersectingPoint(line),
                      intesection);
  }


  void testIntersectionPoint() {
     Line2D line(startPoint,
                endPoint);
     Point2D intesection = line.getIntersectingPoint(Point2D(4, 40));
     TS_ASSERT_EQUALS(intesection, Point2D(1, 10));
  }

  void testIntesects() {
    Line2D line(startPoint,
		endPoint);
    Line2D line2(startPoint,
		 endPoint);
    // they run in parellel, so no intersection
    TS_ASSERT(!line.intersects(line2));
    Line2D line3(Point2D(0,4),
		 Point2D(8,11));
    // clean intersection
    TS_ASSERT(line.intersects(line3));
    Line2D line4(Point2D(0,4),
		 Point2D(8,9));
    // too short
    TS_ASSERT(!line.intersects(line4));
        Line2D line5(Point2D(10.5,4),
		     Point2D(20,9));
    // too far
    TS_ASSERT(!line.intersects(line5));

    Line2D line6(Point2D(0, 0 ),
		 Point2D(0,11));
    // intersects on point
    TS_ASSERT(line.intersects(line6));

    Line2D line8(Point2D(5, 0 ),
		 Point2D(5,15));
    // intersects on point
    TS_ASSERT(line.intersects(line8));

    Line2D line7(Point2D(0, 0 ),
		 Point2D(0,10));
    // intersects on point exectly
    TS_ASSERT(line.intersects(line7));
  }
};

#endif  // MATH_UNIT_TEST_LINE2DTEST_H_
