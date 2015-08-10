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
};

#endif  // MATH_UNIT_TEST_LINE2DTEST_H_
