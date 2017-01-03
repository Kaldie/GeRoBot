// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_TRACETEST_H_
#define MATH_UNIT_TEST_TRACETEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include "../Polygon2D.h"

class TraceTest : public CxxTest::TestSuite {
 public:
  Point2D startPoint;
  Point2D endPoint;

  void setUp() {
    startPoint = Point2D(0, 0);
    endPoint = Point2D(-10 , 10);
  }

  void testCreation() {
    Trace trace(startPoint, endPoint);
    TS_ASSERT_EQUALS(trace.getStartPoint() , startPoint);
    TS_ASSERT_EQUALS(trace.getEndPoint() , endPoint);
    TS_ASSERT_EQUALS(trace.getTraceType() , Trace::Line);
  }

  void testTraceLine() {
    Trace trace(startPoint, endPoint);
    Line2D line = trace.getTraceLine();
    TS_ASSERT_EQUALS(line, Line2D(startPoint, endPoint));
  }

  void testIntersectionPoint() {
   Trace trace(startPoint, endPoint);
   LOG_DEBUG("Here");
   LOG_DEBUG("StartPoint Trace: " << trace.getStartPoint().x << " , " << trace.getStartPoint().y);
      LOG_DEBUG("endPoint Trace: " << trace.getEndPoint().x << " , " << trace.getEndPoint().y);
   Point2D intersection = trace.intersectingPoint(Point2D(5, 0));
   LOG_DEBUG("Here2");
   TS_ASSERT_EQUALS(intersection, Point2D(0, 0));

   Trace trace2(Point2D(0, 10), endPoint);
   TS_ASSERT_THROWS(trace2.intersectingPoint(Point2D(5, 0)),
		    std::runtime_error);
   Point2D intersection1 = trace2.intersectingPoint(Point2D(-5, 10));
   TS_ASSERT_EQUALS(intersection1, Point2D(-5, 10));
  }

  void testRotationDireciton() {
    Trace trace(startPoint, endPoint);
    TS_ASSERT_EQUALS(trace.getRotationDirectionToEndPoint(Point2D(-11, 10)), "CW");
    TS_ASSERT_EQUALS(trace.getRotationDirectionToEndPoint(Point2D(11, 10)), "CCW");
  }

  void testTranslationDirection() {
   Trace trace(startPoint, endPoint);
   TS_ASSERT_EQUALS(trace.getTranslationDirectionToEndPoint(Point2D(0, 0)), "OUT");
   TS_ASSERT_EQUALS(trace.getTranslationDirectionToEndPoint(Point2D(11, 10)), "IN");
  }

  void testPointPointers() {
    Trace trace(startPoint, endPoint);
    std::vector<Point2D*> pointers = trace.getPointPointers();
    TS_ASSERT_EQUALS(pointers.size(), 2);
    TS_ASSERT_EQUALS(*pointers[0], startPoint);
    TS_ASSERT_EQUALS(*pointers[1], endPoint);
  }


  void testEstimateTrace() {
    Trace trace(Point2D(0, 10), endPoint);
    std::vector<Point2D> points = trace.estimateTrace(6);
    TS_ASSERT_EQUALS(points.size(), 6);
    TS_ASSERT_DIFFERS(points[0], startPoint);
    TS_ASSERT_EQUALS(points[5], endPoint);
    for (int i = 0;
	 i < 6;
	 ++i) {
      LOG_DEBUG("Point: " << points[i].x << " , " << points[i].y);
      TS_ASSERT_EQUALS(points[i], Point2D(i*-2,10));
    }
  }

  void testPerpendicularDistance() {
    Trace trace(Point2D(1, 0), Point2D(1, 25));
    TS_ASSERT_DELTA(Trace(Point2D(1, 0), Point2D(1, 25)).
		    getPerpendicularDistance(Point2D(0, 10)), 1, 0.0001);
    TS_ASSERT_DELTA(Trace(Point2D(10, 0), Point2D(10, 25)).
		    getPerpendicularDistance(Point2D(0, 10)), 10, 0.0001);
    TS_ASSERT_DELTA(Trace(Point2D(1, 0), Point2D(26, 25)).
		    getPerpendicularDistance(Point2D(25, 25)), 0.7071, 0.0001);
    TS_ASSERT_DELTA(Trace(Point2D(-1, 0), Point2D(24, 25)).
		    getPerpendicularDistance(Point2D(25, 25)), 0.7071, 0.0001);
    TS_ASSERT_DELTA(Trace(Point2D(-10, 0), Point2D(15, 25)).
		    getPerpendicularDistance(Point2D(25, 25)), 0.7071*10, 0.0001);

  }
};

#endif  // MATH_UNIT_TEST_TRACETEST_H_
