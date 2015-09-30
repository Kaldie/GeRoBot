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
                            Point2D(-10,0)
                            });
    Polygon2D a(x);
    TS_ASSERT(a.isInside(Point2D(5,5)));
    TS_ASSERT(a.isInside(Point2D(5,0)));
    TS_ASSERT(a.isInside(Point2D(10,0)));
    TS_ASSERT(a.isInside(Point2D(-10,0)));
    TS_ASSERT(!a.isInside(Point2D(-10 - 0.01 ,0)));
    TS_ASSERT(!a.isInside(Point2D(-10,0 - 0.01)));
    TS_ASSERT(!a.isInside(Point2D(10 + 0.01 ,0)));
    /*
      TS_ASSERT_EQUALS(Point2D(100, 0).getAngleToOrigin(), 0);
    TS_ASSERT_DELTA(Point2D(100, 100).getAngleToOrigin(), PI/4, 0.0001);
    TS_ASSERT_DELTA(Point2D(-100, 100).getAngleToOrigin(), PI - PI/4, 0.0001);
    TS_ASSERT_DELTA(Point2D(-100, -100).getAngleToOrigin(),
                    PI +  PI/4, 0.0001);
    */
  }

  void testAngleBetweenPoints() {
    /*
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
    */
  }

  void testQPointFCast() {
    /*
    Point2D point(10.0,-10.0);
    QPointF qPoint = point;
    TS_ASSERT_EQUALS(QPointF(10.0,10.0),
                     qPoint);
    TS_ASSERT_DIFFERS(QPointF(10.0,-10.0),
                     qPoint);
    */
  }

  void testConstrucionFromQPointF() {
    /*
    QPointF qPoint(-10,25);
    Point2D point(qPoint);
    TS_ASSERT_EQUALS(Point2D(-10.0, -25.0),
                     point);
    TS_ASSERT_DIFFERS(Point2D(10.0, 25.0),
                     point);
    */
  }
};

#endif  //  MATH_UNIT_TEST_POLYGONTEST_H_
