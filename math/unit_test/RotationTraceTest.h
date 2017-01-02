// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_ROTATIONTRACETEST_H_
#define MATH_UNIT_TEST_ROTATIONTRACETEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <StepperDriver.h>
#include <RotationalJoint.h>
#include <TranslationalJoint.h>
#include <JointController.h>
#include <RotationTrace.h>
#include <RotationTraceCalculator.h>

class RotationTraceTest : public CxxTest::TestSuite {
 public:
  void testCreation() {
    traceType startAngle, stopAngle;
    RotationTrace trace;
    RotationTrace trace2(Point2D(-10, 0),
			 Point2D(10, 0),
			 Point2D(0, 0));
    TS_ASSERT(trace2.getIsClockwise());
    trace2.getStartStopAngle(&startAngle, &stopAngle);
    TS_ASSERT_DELTA(startAngle, 180, 0.001);
    TS_ASSERT_DELTA(stopAngle, 0, 0.001);

    RotationTrace trace3(Point2D(10, 0),
			 Point2D(-10, 0),
			 Point2D(0, 0));
    TS_ASSERT(!trace3.getIsClockwise());
    trace3.getStartStopAngle(&startAngle, &stopAngle);
    TS_ASSERT_DELTA(stopAngle, 180, 0.001);
    TS_ASSERT_DELTA(startAngle, 0, 0.001);
  };


  void testNecessaryTraces() {
    // create off center trace
    RotationTrace trace(Point2D(10,90),
			Point2D(10,110),
			Point2D(10, 100));
    // check if the calculation of the spanned angle still works
    TS_ASSERT_DELTA(trace.getArc().spanAngle(), PI, 0.001);
      // check if the necessary traces are derived in the clockwise fashion
    std::vector<RotationTrace> vector = trace.getNecessaryTraces();
    TS_ASSERT_EQUALS(2, vector.size());
    TS_ASSERT_EQUALS(vector[0].getEndPoint() , Point2D(0,100));
    // test if it works in counter clockwise stuff
    trace.setIsClockwise(false);
    vector = trace.getNecessaryTraces();
    TS_ASSERT_EQUALS(2, vector.size());
    TS_ASSERT_EQUALS(vector[0].getEndPoint() , Point2D(19.802, 98.0198));
    // Yay
  };


  void testCircleHasOrigin() {
    // The necessary traces need a different calculation of the circle is within the origin of the axis
    // As this seem to be posible on the hardware side we need to do it 2
    // first case, (0,0) as center
    RotationTrace trace(Point2D(-3, 4),
			Point2D(3, 4),
			Point2D(0, 0));
    TS_ASSERT(trace.getIsClockwise());
    std::vector<RotationTrace> vector = trace.getNecessaryTraces();
    // in the clockwise thingy, we do not need any helper traces
    TS_ASSERT_EQUALS(vector.size(), 1);
    trace.setIsClockwise(false);
    vector = trace.getNecessaryTraces();
    TS_ASSERT_EQUALS(vector.size(), 3);
    for (auto& thisTrace : vector) {
      LOG_DEBUG("Start point: " << thisTrace.getStartPoint().x << " , " << thisTrace.getStartPoint().y);
      LOG_DEBUG("End point: " << thisTrace.getEndPoint().x << " , " << thisTrace.getEndPoint().y);
    }
  };


  void testCircleHasOriginAndSymetric() {
    // The necessary traces need a different calculation of the circle is within the origin of the axis
    // As this seem to be posible on the hardware side we need to do it 2
    // first case, (0,0) as center
    RotationTrace trace(Point2D(0, -4),
			Point2D(0, 4),
			Point2D(0, 0));
    TS_ASSERT(trace.getIsClockwise());
    std::vector<RotationTrace> vector = trace.getNecessaryTraces();
    // in the clockwise thingy, we do not need any helper traces
    TS_ASSERT_EQUALS(vector.size(), 2);
    TS_ASSERT_EQUALS(vector[0].getEndPoint(), Point2D(-4, 0));

    trace.setIsClockwise(false);
    vector = trace.getNecessaryTraces();
    TS_ASSERT_EQUALS(vector.size(), 2);
    TS_ASSERT_EQUALS(vector[0].getEndPoint(), Point2D(4, 0));
  };


  void testEstimateTrace() {
    RotationTrace trace(Point2D(0, 4),
                        Point2D(0, 4),
                        Point2D(0, 0));
    std::vector<Point2D> points =  trace.estimateTrace(5);
    TS_ASSERT_EQUALS(points.size(), 5);
    std::vector<Point2D> knownPoints(
               {Point2D(0, 4),
                Point2D(-4, 0),
                Point2D(0, -4),
                Point2D(4, 0),
                Point2D(0, 4)});

    auto knownItr = knownPoints.begin();
    for (const auto& X : points) {
        TS_ASSERT_EQUALS(X, *knownItr);
        ++knownItr;
    }
  }


  void testEstimateTrace2() {
    RotationTrace trace(Point2D(0, 4),
                        Point2D(0, -4),
                        Point2D(0, 0));
    trace.setIsClockwise(true);
    std::vector<Point2D> points =  trace.estimateTrace(3);
    TS_ASSERT_EQUALS(points.size(), 3);
    std::vector<Point2D> knownPoints(
               {Point2D(0, 4),
                Point2D(4, 0),
                Point2D(0, -4)});

    auto knownItr = knownPoints.begin();
    for (const auto& X : points) {
        TS_ASSERT_EQUALS(X, *knownItr);
        LOG_DEBUG("Point: " << X.x << " , " << X.y);
        ++knownItr;
    }
  }


  void testPerpendicularDistance() {
      TS_ASSERT_DELTA(RotationTrace(Point2D(10, 1), Point2D(10, 1), Point2D(0, 1)).
                    getPerpendicularDistance(Point2D(0, 1)), 10, 0.0001);
      TS_ASSERT_DELTA(RotationTrace(Point2D(13,5), Point2D(13, 5), Point2D(3, 5)).
                 getPerpendicularDistance(Point2D(3, 5)), 10, 0.0001);
  }

  void testExtremePointsFullCirlce() {
    LOG_DEBUG("testExtremePoints_FullCirlce");
    RotationTrace trace(Point2D(10, 1), Point2D(10, 1), Point2D(0, 1));
    std::vector<Point2D> pointVector;
    trace.getExtremePoints(&pointVector);
    TS_ASSERT_EQUALS(pointVector.size(),4);
    TS_ASSERT_EQUALS(pointVector[0], Point2D(0, 11));
    TS_ASSERT_EQUALS(pointVector[1], Point2D(-10, 1));
    TS_ASSERT_EQUALS(pointVector[2], Point2D(0, -9));
    TS_ASSERT_EQUALS(pointVector[3], Point2D(10, 1));
  }

  void testExtremePointsHalfCirlce() {
    LOG_DEBUG("testExtremePoints_HalfCirlce");
    RotationTrace trace(Point2D(10, 1), Point2D(-10, 1), Point2D(0, 1));
    std::vector<Point2D> pointVector;
    trace.getExtremePoints(&pointVector);
    TS_ASSERT_EQUALS(pointVector.size(),1);
    TS_ASSERT_EQUALS(pointVector[0], Point2D(0, 11));
  }

  void testExtremePointsHalfCirlceMinusABit() {
    LOG_DEBUG("testExtremePointsHalfCirlceMinusABit");
    RotationTrace trace(Point2D(10, 1), Point2D(-2.58819, 10.659258), Point2D(0, 1));
    std::vector<Point2D> pointVector;
    trace.getExtremePoints(&pointVector);
    TS_ASSERT_EQUALS(pointVector.size(),1);
    TS_ASSERT_EQUALS(pointVector[0], Point2D(0, 11));

    trace.setIsClockwise(true);
    pointVector.clear();
    trace.getExtremePoints(&pointVector);
    TS_ASSERT_EQUALS(pointVector.size(),3);
    TS_ASSERT_EQUALS(pointVector[0], Point2D(10, 1));
    TS_ASSERT_EQUALS(pointVector[1], Point2D(0, -9));
    TS_ASSERT_EQUALS(pointVector[2], Point2D(-10, 1));
  }

  
};
#endif  // MATH_UNIT_TEST_ROTATIONTRACETEST_H_
