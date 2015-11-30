// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_POINT2DTEST_H_
#define MATH_UNIT_TEST_POINT2DTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include "../Point2D.h"



class Point2DTest : public CxxTest::TestSuite {
 public:
  void testEqual() {
    TS_ASSERT(Point2D(1.15, 3.1415) == Point2D(1.15, 3.1415));
    TS_ASSERT(Point2D(1.15, 3.1415) != Point2D(5.15, 3.1415));
  }

  void testReduction() {
    TS_ASSERT((Point2D(10, 10) - Point2D(3, 4))== Point2D(7, 6));
    TS_ASSERT((Point2D(10, 10) - Point2D(3, 4)) != Point2D(6, 6));
  }

  void testAddition() {
    TS_ASSERT(Point2D(5, 4) + Point2D(2, 3) == Point2D(7, 7));
    TS_ASSERT(Point2D(5, 4) + Point2D(2, 3) != Point2D(5, 7));
  }

  void testMultiplactionInPlace() {
    TS_ASSERT(Point2D(5, 4) * 2 == Point2D(10, 8));
    TS_ASSERT(Point2D(5, 4) * 2 != Point2D(9, 8));
  }

  void testAngleToOrigin() {
    TS_ASSERT_EQUALS(Point2D(100, 0).getAngleToOrigin(), 0);
    TS_ASSERT_DELTA(Point2D(100, 100).getAngleToOrigin(), PI/4, 0.0001);
    TS_ASSERT_DELTA(Point2D(-100, 100).getAngleToOrigin(), PI - PI/4, 0.0001);
    TS_ASSERT_DELTA(Point2D(-100, -100).getAngleToOrigin(),
                    PI +  PI/4, 0.0001);
  }

  void testAngleBetweenPoints() {
    TS_ASSERT_DELTA(Point2D(11, 12).getAngleBetweenPoints(Point2D(11, 12)),
                    0, 0.0001);
    TS_ASSERT_DELTA(Point2D(10, 0).getAngleBetweenPoints(Point2D(0, 10)),
                    PI/-2, 0.0001);
    TS_ASSERT_DELTA(Point2D(-10, 0).getAngleBetweenPoints(Point2D(0, 10)),
                    PI/2, 0.0001);
    TS_ASSERT_DELTA(Point2D(10, 0).getAngleBetweenPoints(Point2D(0, -10)),
                    -PI*1.5, 0.0001);
    TS_ASSERT_DELTA(Point2D(-10, 0).getAngleBetweenPoints(Point2D(0, 10)),
                    PI * 0.5, 0.0001);
  }

#ifdef QT
  void testQPointFCast() {
    Point2D point(10.0,-10.0);
    QPointF qPoint = point;
    TS_ASSERT_EQUALS(QPointF(10.0,10.0),
                     qPoint);
    TS_ASSERT_DIFFERS(QPointF(10.0,-10.0),
                     qPoint);
  }


  void testConstrucionFromQPointF() {
    QPointF qPoint(-10,25);
    Point2D point(qPoint);
    TS_ASSERT_EQUALS(Point2D(-10.0, -25.0),
                     point);
    TS_ASSERT_DIFFERS(Point2D(10.0, 25.0),
                     point);
  }
#endif  // QT
};

#endif  // MATH_UNIT_TEST_POINT2DTEST_H_
