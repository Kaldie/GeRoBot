// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_CIRCLETEST_H_
#define MATH_UNIT_TEST_CIRCLETEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include "../Point2D.h"
#include "../Line2D.h"
#include "../Circle2D.h"


class CircleTest : public CxxTest::TestSuite {
 public:
  void testCreation() {
    Point2D firstPoint(0, 1), secondPoint(1, 0), centrePoint(0, 0);
    Circle2D circle(firstPoint, centrePoint);
    TS_ASSERT_EQUALS(circle.radius(), 1);
    TS_ASSERT(circle.isPointOnCircle(Point2D(-1, 0)));

    circle.setFirstPoint(Point2D(0, 10));
    TS_ASSERT_EQUALS(circle.radius(), 10);
    TS_ASSERT(!circle.isPointOnCircle(Point2D(-1, 0)));

    Circle2D circle2(Point2D(1, 2), Point2D(2, 1), 1);
    TS_ASSERT_EQUALS(circle2.getCentrePoint(), Point2D(1, 1));
  }

  void testNearestIntersection() {
    Circle2D cirlce(Point2D(-10, 0), Point2D(0, 0));
    Line2D line(Point2D(-100, 0), Point2D(-21, 0));
    TS_ASSERT_EQUALS(cirlce.nearestIntersection(Line2D(Point2D(-100, 0), Point2D(-0, 0))), Point2D(-10, 0));
    TS_ASSERT_EQUALS(cirlce.nearestIntersection(Line2D(Point2D(-100, 0), Point2D(-10, 0))), Point2D(-10, 0));
    TS_ASSERT_EQUALS(cirlce.nearestIntersection(Line2D(Point2D(-100, 0), Point2D(-20, 0))), Point2D(-10, 0));
    TS_ASSERT_EQUALS(cirlce.nearestIntersection(Line2D(Point2D(-100, 0), Point2D(10, 0))), Point2D(10, 0));

    TS_ASSERT_EQUALS(cirlce.nearestIntersection(Line2D(Point2D(1, 0), Point2D(10, 0))), Point2D(10, 0));
    TS_ASSERT_EQUALS(cirlce.nearestIntersection(Line2D(Point2D(1, 0), Point2D(100, 0))), Point2D(10, 0))
    TS_ASSERT_EQUALS(cirlce.nearestIntersection(Line2D(Point2D(1, 0), Point2D(5, 0))), Point2D(10, 0));

    TS_ASSERT_EQUALS(cirlce.nearestIntersection(Line2D(Point2D(0, 1), Point2D(0, 10))), Point2D(0, 10));
    TS_ASSERT_EQUALS(cirlce.nearestIntersection(Line2D(Point2D(0, 1), Point2D(0, -10))), Point2D(0, -10));

    TS_ASSERT_EQUALS(cirlce.nearestIntersection(Line2D(Point2D(0, 0), Point2D(5, 5))), Point2D(7.07107 , 7.07107));
    TS_ASSERT_EQUALS(cirlce.nearestIntersection(Line2D(Point2D(0, 0), Point2D(-5, -5))), -1 * Point2D(7.07107 , 7.07107));
  }
};
#endif  // MATH_UNIT_TEST_CIRCLETEST_H_
