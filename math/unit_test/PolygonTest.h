// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_POLYGONTEST_H_
#define MATH_UNIT_TEST_POLYGONTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include "../Polygon2D.h"



class PolygonTest : public CxxTest::TestSuite {
 public:
  void testEqual() {
    std::vector<Point2D>
      x({Point2D(0,0), Point2D(10,0),
	    Point2D(10,10),
	    Point2D(-10,10),
	    Point2D(-10,0),
	    Point2D(0,0)});
    Polygon2D a(x);
    Polygon2D b(x);
    // This test is a bit off because it is a square itself
    TS_ASSERT_DELTA(200.0, a.getSurface(), 0.001);
    TS_ASSERT(a == b);
    int numberOfPoints = b.getNumberOfPoints();
    TS_ASSERT_EQUALS(a.getNumberOfPoints(), numberOfPoints);
    b.addPoint2D(Point2D(-132,100));
    TS_ASSERT_EQUALS(b.getNumberOfPoints(), numberOfPoints + 1);
    TS_ASSERT(a != b);
  }

  void testAddition() {
        std::vector<Point2D>
      x({Point2D(0,0), Point2D(10,0),
	    Point2D(10,10),
	    Point2D(-10,10),
	    Point2D(-10,0),
	    Point2D(0,0)});
    Polygon2D a(x);
    Polygon2D b;
    TS_ASSERT(b.isEmpty());
    for (const auto& point : x) {
      b.addPoint2D(point);
    }
    TS_ASSERT(!b.isEmpty());
    TS_ASSERT(a == b);
  }


  void testAdditionInPlace() {
    std::vector<Point2D> x({Point2D(0,0),
                            Point2D(10,0),
                            Point2D(10,10),
                            Point2D(-10,10),
		            Point2D(-10,0),
                            Point2D(0,0)});
    Point2D roguePoint(10,10);
    std::vector<Point2D> y({Point2D(0,0),
                            Point2D(10,0),
                            Point2D(-10,10),
                            Point2D(-10,0),
                            Point2D(0,0)});
    Polygon2D a(x);
    Polygon2D b(y);
    b.addPoint2D(roguePoint, 2);
    TS_ASSERT(a == b);
  }


  void testIsClosed() {
    std::vector<Point2D> x({Point2D(0,0),
                            Point2D(10,0),
                            Point2D(10,10),
                            Point2D(-10,10),
		            Point2D(-10,0),
                            Point2D(0,0)});
    Polygon2D a(x);
    TS_ASSERT(a.isClosed());
    a.addPoint2D(Point2D(10, 10));
    TS_ASSERT(!a.isClosed());
  }

  void testIsInsideFromClosed() {
    std::vector<Point2D> x({Point2D(0,0),
                            Point2D(10,0),
                            Point2D(10,10),
                            Point2D(-10,10),
                            Point2D(-10,0),
                            Point2D(0,0)});
    Polygon2D a(x);
    a.exportToFile();
    TS_ASSERT(a.isInside(Point2D(5,5)));
    TS_ASSERT(a.isInside(Point2D(5,0)));
    TS_ASSERT(a.isInside(Point2D(9,6)));
    TS_ASSERT(a.isInside(Point2D(-10, 0)));
    // just over the border, downside
    TS_ASSERT(!a.isInside(Point2D(-10, -10 - 1.01)));
    // just over the border left side
    TS_ASSERT(!a.isInside(Point2D(-10 - 0.1, 0)));
    // just on the border right side
    TS_ASSERT(!a.isInside(Point2D(10 ,0)));

    // is at the right border, not inside
    TS_ASSERT(!a.isInside(Point2D(10,0)));



    /*
      TS_ASSERT_EQUALS(Point2D(100, 0).getAngleToOrigin(), 0);
    TS_ASSERT_DELTA(Point2D(100, 100).getAngleToOrigin(), PI/4, 0.0001);
    TS_ASSERT_DELTA(Point2D(-100, 100).getAngleToOrigin(), PI - PI/4, 0.0001);
    TS_ASSERT_DELTA(Point2D(-100, -100).getAngleToOrigin(),
                    PI +  PI/4, 0.0001);
    */
  }

  void testSurfaceTriangle() {
    std::vector<Point2D> x({Point2D(0,0),
                            Point2D(10,0),
                            Point2D(10,10),
                            Point2D(0,0)
                            });
    Polygon2D a(x);
    TS_ASSERT_DELTA(a.getSurface(), 50, 0.0001);
  }
};

#endif  //  MATH_UNIT_TEST_POLYGONTEST_H_
