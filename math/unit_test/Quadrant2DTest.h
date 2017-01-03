#ifndef MATH_UNIT_TEST_QUADRANT2DTEST_H_
#define MATH_UNIT_TEST_QUADRANT2DTEST_H_

#include <cxxtest/TestSuite.h>
#include <macroHeader.h>
#include <Quadrant2D.h>
#include <Point2D.h>

class Quadrant2DTest : public CxxTest::TestSuite {

 public:
  void testConstruction() {
    // simply test if they will construct
    Quadrant2D a(Point2D(1,1));
    Quadrant2D b(1);
  }

  void testGetQuadrantNeighbout() {
    Quadrant2D quadrant(Point2D(1,1));
    int ccwQuadrant, cwQuadrant;
    quadrant.getQuardrantNeighbours(&ccwQuadrant, &cwQuadrant);
    TS_ASSERT_EQUALS(ccwQuadrant, 1);
    TS_ASSERT_EQUALS(cwQuadrant, 3);
    Quadrant2D quadrant2(Point2D(1, -1));
    quadrant2.getQuardrantNeighbours(&ccwQuadrant, &cwQuadrant);
    TS_ASSERT_EQUALS(ccwQuadrant, 0);
    TS_ASSERT_EQUALS(cwQuadrant, 2);
  }

  void testBorderPoint() {
    Quadrant2D quadrant(Point2D(1,1));
    Point2D borderPoint(quadrant.getBorderPoint(/*clockwise*/ false));
    TS_ASSERT_EQUALS(borderPoint, Point2D(0,1));
    borderPoint = quadrant.getBorderPoint(/*clockwise*/ true);
    TS_ASSERT_EQUALS(borderPoint, Point2D(1,0));
    
    Quadrant2D quadrant2(Point2D(-1,1));
    borderPoint = quadrant2.getBorderPoint(/*clockwise*/ false);
    TS_ASSERT_EQUALS(borderPoint, Point2D(-1,0));
    borderPoint = quadrant2.getBorderPoint(/*clockwise*/ true);
    TS_ASSERT_EQUALS(borderPoint, Point2D(0,1));
  }

  
  void testBorderAngle() {
    Quadrant2D quadrant(Point2D(1,1));
    float angle(quadrant.getBorderAngle(/*clockwise*/ false));
    TS_ASSERT_DELTA(angle, PI/2.0, 0.001);
    angle = quadrant.getBorderAngle(/*clockwise*/ true);
    TS_ASSERT_DELTA(angle, 0, 0.001);
    
    Quadrant2D quadrant2(Point2D(-1,1));
    angle = quadrant2.getBorderAngle(/*clockwise*/ false);
    TS_ASSERT_DELTA(angle, PI, 0.001);
    angle = quadrant2.getBorderAngle(/*clockwise*/ true);
    TS_ASSERT_DELTA(angle, PI/2.0, 0.001);

    Quadrant2D quadrant3(Point2D(1,-1));
    angle = quadrant3.getBorderAngle(/*clockwise*/ false);
    TS_ASSERT_DELTA(angle, 0, 0.001);
    angle = quadrant3.getBorderAngle(/*clockwise*/ true);
    TS_ASSERT_DELTA(angle, PI * 1.5, 0.001);
  }

  
  void testIncrement() {
    Quadrant2D quadrant(0);
    quadrant.increment(/*clockwise*/ false);
    TS_ASSERT_EQUALS(quadrant,Quadrant2D(1));
    quadrant.setQuadrantValue(3);
    quadrant.increment(/*clockwise*/ false);
    TS_ASSERT_EQUALS(quadrant,Quadrant2D(0));

    quadrant.increment(/*clockwise*/ true);
    TS_ASSERT_EQUALS(quadrant,Quadrant2D(3));
    quadrant.increment(/*clockwise*/ true);
    TS_ASSERT_EQUALS(quadrant,Quadrant2D(2));
    quadrant.increment(/*clockwise*/ true);
    TS_ASSERT_EQUALS(quadrant,Quadrant2D(1));
    quadrant.increment(/*clockwise*/ true);
    TS_ASSERT_EQUALS(quadrant,Quadrant2D(0));
  }

  void testBoolean() {
    Quadrant2D quadrantA(0);
    Quadrant2D quadrantB(0);
    TS_ASSERT_EQUALS(quadrantA, quadrantB);
    TS_ASSERT(!(quadrantA > quadrantB));
    TS_ASSERT(!(quadrantA < quadrantB));

    quadrantB.setQuadrantValue(3);
    TS_ASSERT_DIFFERS(quadrantA, quadrantB);
    TS_ASSERT(!(quadrantA > quadrantB));
    TS_ASSERT(quadrantA < quadrantB);
  }
};

#endif  // MATH_UNIT_TEST_QUADRANT2DTEST_H_
