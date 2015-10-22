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

  void testIsEqual() {
    Line2D line(startPoint,
                endPoint);
    Line2D line2(startPoint,
                 endPoint);
    Line2D line3(startPoint + Point2D(1, 1),
                 endPoint + Point2D(1, 1));
    Line2D line4(startPoint + Point2D(1, 1),
                 endPoint);
    TS_ASSERT_EQUALS(line, line2);
    TS_ASSERT_DIFFERS(line, line3);
    TS_ASSERT_DIFFERS(line, line4);
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
    Line2D line(startPoint,    // Point2D(0, 10);
                endPoint);     // Point2D(10, 10);
    Line2D line2(startPoint,
		 endPoint);
    // they run in parellel, so no intersection
    TS_ASSERT(!line.intersects(line2));
    // clean intersection
    Line2D line3(Point2D(0,4),
		 Point2D(8,11));
    TS_ASSERT(line.intersects(line3));
    // too short
    Line2D line4(Point2D(0,4),
		 Point2D(8,9));
    TS_ASSERT(!line.intersects(line4));
    // too far
    Line2D line5(Point2D(10.5,4),
		     Point2D(20,9));
    TS_ASSERT(!line.intersects(line5));

    // intersects on point
    Line2D line6(Point2D(0, 0 ),
                 Point2D(0,11));
    TS_ASSERT(line.intersects(line6));

    // intersects on point end point of line8, which is a downward line, therefor does interesects
    Line2D line8(Point2D(0, 20 ),
                 Point2D(0, 10));
    TS_ASSERT(line.intersects(line8));

    // intersects on point start point of line8, which is a upward line, therefor does interesects
    Line2D line10(Point2D(0, 10 ),
                 Point2D(0, 20));
    TS_ASSERT(line.intersects(line10));

    // intersects on the end point of line7, line is upward, therefor does not intersect
    Line2D line7(Point2D(0, 0 ),
		 Point2D(0,10));
    TS_ASSERT(!line.intersects(line7));

    // intersects on the end point of line7, line is upward, therefor does not intersect
    Line2D line11(Point2D(0, 10 ),
                 Point2D(0, 0));
    TS_ASSERT(!line.intersects(line11));
  }

  void testIntersectCornerCase() {
    /*
     Given a vertical line
     we define an intersection
     if the line is intersected at from the startpoint
     until (not included) the end point
    */
    Line2D firstLine(Point2D(5, 2), Point2D(5, 10));
    Line2D secondLine(Point2D(0, 2), Point2D(6, 2)); // should intersect
    Line2D thirdLine(Point2D(0, 10), Point2D(6, 10)); // shouldn't intersect
    // test if when the line cuts at the start point it intersects
    TS_ASSERT_EQUALS(firstLine.getStartPoint(), firstLine.getIntersectingPoint(secondLine));
    TS_ASSERT(firstLine.intersects(secondLine));
    // test when the line cuts at the end point it does not intersect
    TS_ASSERT_EQUALS(firstLine.getEndPoint(), firstLine.getIntersectingPoint(thirdLine));
    TS_ASSERT(!firstLine.intersects(thirdLine));
  }
};

#endif  // MATH_UNIT_TEST_LINE2DTEST_H_
