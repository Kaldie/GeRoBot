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
              Point2D(0, 0), true);

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
                                Point2D(0, 0), true),
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


  void testIsPointOnArc() {
    Arc2D arc(Point2D(10,0), Point2D(0.0078579097806003306, 9.9999969126622172), Point2D(0,0), false);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(7.07106,7.07106)), true);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(7.07106,-7.07106)), false);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(-7.07106,-7.07106)), false);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(-7.07106,7.07106)), false);
    arc.setIsClockwise(true);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(7.07106,7.07106)), false);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(-7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(-7.07106,7.07106)), true);
    
    Arc2D arc2(Point2D(10,0), Point2D(-9.9999969126622172, 0.0078579097806003306), Point2D(0,0), false);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(7.07106,7.07106)), true);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(7.07106,-7.07106)), false);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(-7.07106,-7.07106)), false);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(-7.07106,7.07106)), true);
    arc2.setIsClockwise(true);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(7.07106,7.07106)), false);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(-7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(-7.07106,7.07106)), false);
    
    Arc2D arc3(Point2D(10,0), Point2D(9.9999969126622172, -0.0078579097806003306), Point2D(0,0), false);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(7.07106,7.07106)), true);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(-7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(-7.07106,7.07106)), true);
    arc3.setIsClockwise(true);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(7.07106,7.07106)), false);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(7.07106,-7.07106)), false);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(-7.07106,-7.07106)), false);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(-7.07106,7.07106)), false);
  }

    void testIsPointOnArc2() {
      Arc2D arc(Point2D(10,0), Point2D(0, 10), Point2D(0,0), false);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(7.07106,7.07106)), true);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(7.07106,-7.07106)), false);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(-7.07106,-7.07106)), false);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(-7.07106,7.07106)), false);
    arc.setIsClockwise(true);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(7.07106,7.07106)), false);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(-7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc.isPointOnArc(Point2D(-7.07106,7.07106)), true);
    
    Arc2D arc2(Point2D(10,0), Point2D(-10, 0), Point2D(0,0), false);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(7.07106,7.07106)), true);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(7.07106,-7.07106)), false);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(-7.07106,-7.07106)), false);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(-7.07106,7.07106)), true);
    arc2.setIsClockwise(true);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(7.07106,7.07106)), false);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(-7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc2.isPointOnArc(Point2D(-7.07106,7.07106)), false);

    // full cirlce!
    Arc2D arc3(Point2D(10,0), Point2D(10, 0), Point2D(0,0), false);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(7.07106,7.07106)), true);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(-7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(-7.07106,7.07106)), true);
    arc3.setIsClockwise(true);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(7.07106,7.07106)), true);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(-7.07106,-7.07106)), true);
    TS_ASSERT_EQUALS(arc3.isPointOnArc(Point2D(-7.07106,7.07106)), true);
  }
    
    
    void testAngularDistance() {
      Arc2D arc(Point2D(0,10),Point2D(10,0),Point2D(0,0), false);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(10,0)),
		      PI * 1.5, 0.0001);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(-10,0)),
		      PI / 2.0, 0.0001);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(0,-10)),
		      PI, 0.0001);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(0,10)),
		      0, 0.0001);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(5,5)),
		      PI * 1.75, 0.0001);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(5,-5)),
		      PI * 1.25, 0.0001);

      arc.setIsClockwise(true);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(10,0)),
		      PI * 0.5, 0.0001);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(-10,0)),
		      PI * 1.5, 0.0001);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(0,-10)),
		      PI, 0.0001);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(0,10)),
		      0, 0.0001);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(5,5)),
		      PI * 0.25, 0.0001);
      TS_ASSERT_DELTA(arc.angularDistanceToStartPoint(Point2D(5,-5)),
		      PI * 0.75, 0.0001);
    }

    void testPointSort() {
      Arc2D arc(Point2D(0,10),Point2D(10,0),Point2D(0,0), false);
      std::vector<Point2D> vector, sortedVector;
      vector.push_back(Point2D(10,0));
      vector.push_back(Point2D(-10,0));
      vector.push_back(Point2D(0,-10));
      vector.push_back(Point2D(0,10));
      vector.push_back(Point2D(5,5));
      vector.push_back(Point2D(5,-5));
      
      sortedVector.push_back(Point2D(0,10));
      sortedVector.push_back(Point2D(-10,0));
      sortedVector.push_back(Point2D(0,-10));
      sortedVector.push_back(Point2D(5,-5));
      sortedVector.push_back(Point2D(10,0));
      sortedVector.push_back(Point2D(5,5));
      
      arc.sortPoints(vector.begin(), vector.end());
      auto sortedIter = sortedVector.begin();
      for (auto& point : vector) {
	TS_ASSERT_EQUALS(point, *sortedIter);
	++sortedIter;
      }
      
      arc.setIsClockwise(true);
      arc.sortPoints(vector.begin(), vector.end());
      sortedVector.clear();
      TS_ASSERT_EQUALS(sortedVector.size(),0);
      sortedVector.push_back(Point2D(0,10));
      sortedVector.push_back(Point2D(5,5));
      sortedVector.push_back(Point2D(10,0));
      sortedVector.push_back(Point2D(5,-5));
      sortedVector.push_back(Point2D(0,-10));
      sortedVector.push_back(Point2D(-10,0));

      sortedIter = sortedVector.begin();
      for (auto& point : vector) {
	LOG_DEBUG("Tested point: " << point.x << ", " << point.y);
	LOG_DEBUG("Expected point: " << sortedIter->x << ", " << sortedIter->y);
	TS_ASSERT_EQUALS(point, *sortedIter);
	++sortedIter;
      }
    }
  
};
#endif  // MATH_UNIT_TEST_ARCTEST_H_
