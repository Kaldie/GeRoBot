// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_ARCTEST_H_
#define MATH_UNIT_TEST_ARCTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include "../Point2D.h"
#include "../Circle2D.h"
#include "../Arc2D.h"

class ArcTest : public CxxTest::TestSuite {
 public:
  void testCreation() {
    Arc2D arc(Point2D(0, 1),
              Point2D(1, 0),
              Point2D(0, 0));

    TS_ASSERT_DELTA(arc.arcLength(), PI / 2.0, 0.0001);
    TS_ASSERT(arc.getIsClockwise());

    arc.setFirstPoint(Point2D(-2, 0));
    arc.setSecondPoint(Point2D(2, 0));
    arc.setCentrePoint(Point2D(0, 0));

    TS_ASSERT_DELTA(arc.arcLength(), 2*PI, 0.0001);
    TS_ASSERT(arc.getIsClockwise());

    arc.setCentrePoint(Point2D(0, 10));
    TS_ASSERT(arc.getIsClockwise());

    TS_ASSERT_THROWS(Arc2D arc2(Point2D(0, 1),
                                Point2D(2, 0),
                                Point2D(0, 0)),
                     std::runtime_error);

    Arc2D arc3(Point2D(0, 4.0),
               Point2D(4.0, 0),
               4,
               true);
    TS_ASSERT(arc3.getIsClockwise());
    TS_ASSERT_EQUALS(arc3.getCentrePoint(),
                     Point2D(0, 0));

    Arc2D arc4(Point2D(0, 4),
               Point2D(4, 0),
               4,
               false);

    TS_ASSERT(!arc4.getIsClockwise());
    TS_ASSERT_EQUALS(arc4.getCentrePoint(),
                     Point2D(4, 4));
  }
};
#endif  // MATH_UNIT_TEST_ARCTEST_H_
