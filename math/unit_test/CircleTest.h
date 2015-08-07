// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_CIRCLETEST_H_
#define MATH_UNIT_TEST_CIRCLETEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include "../Point2D.h"
#include "../Circle2D.h"


class CircleTest : public CxxTest::TestSuite {
 public:
  void testCreation() {
    Point2D firstPoint(0, 1), secondPoint(1, 0), centrePoint(0, 0);
    Circle2D circle(firstPoint, secondPoint, centrePoint);
    TS_ASSERT_EQUALS(circle.radius(), 1);
    TS_ASSERT(circle.isPointOnCircle(Point2D(-1, 0)));

    circle.setFirstPoint(Point2D(0, 10));
    circle.setSecondPoint(Point2D(10, 0));
    TS_ASSERT_EQUALS(circle.radius(), 10);
    TS_ASSERT(!circle.isPointOnCircle(Point2D(-1, 0)));

    Circle2D circle2(Point2D(1, 2), Point2D(2, 1), 1);
    TS_ASSERT_EQUALS(circle2.getCentrePoint(), Point2D(1, 1));

    TS_ASSERT_THROWS(Circle2D(firstPoint, Point2D(10, 0), centrePoint),
                     std::runtime_error);
  }
};
#endif  // MATH_UNIT_TEST_CIRCLETEST_H_
