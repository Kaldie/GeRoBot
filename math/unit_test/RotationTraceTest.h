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
			 Point2D(0, 0), true);
    TS_ASSERT(trace2.getIsClockwise());
    trace2.getStartStopAngle(&startAngle, &stopAngle);
    TS_ASSERT_DELTA(startAngle, 180, 0.001);
    TS_ASSERT_DELTA(stopAngle, 0, 0.001);

    RotationTrace trace3(Point2D(10, 0),
			 Point2D(-10, 0),
			 Point2D(0, 0), false);
    TS_ASSERT(!trace3.getIsClockwise());
    trace3.getStartStopAngle(&startAngle, &stopAngle);
    TS_ASSERT_DELTA(stopAngle, 180, 0.001);
    TS_ASSERT_DELTA(startAngle, 0, 0.001);
  };


  void testNecessaryTraces() {
    // create off center trace
    RotationTrace trace(Point2D(10,90),
			Point2D(10,110),
			Point2D(10, 100), true);
    // check if the calculation of the spanned angle still works
    TS_ASSERT_DELTA(trace.getArc().spanAngle(), PI, 0.001);
    // check if the necessary traces are derived in the clockwise fashion
    std::vector<RotationTrace> vector = trace.getNecessaryTraces();

    LOG_DEBUG("Start [0]: " << vector[0].getStartPoint().x << ", " << vector[0].getStartPoint().y);
    LOG_DEBUG("End [0]: " << vector[0].getEndPoint().x << ", " << vector[0].getEndPoint().y);
    LOG_DEBUG("Start [1]: " << vector[1].getStartPoint().x << ", " << vector[1].getStartPoint().y);
    LOG_DEBUG("End [1]: " << vector[1].getEndPoint().x << ", " << vector[1].getEndPoint().y);
    LOG_DEBUG("Start [2]: " << vector[2].getStartPoint().x << ", " << vector[2].getStartPoint().y);
    LOG_DEBUG("End [2]: " << vector[2].getEndPoint().x << ", " << vector[2].getEndPoint().y);

    
    TS_ASSERT_EQUALS(2, vector.size());
    TS_ASSERT_EQUALS(vector[0].getEndPoint() , Point2D(0,100));
    // test if it works in counter clockwise stuff
    trace.setIsClockwise(false);
    vector = trace.getNecessaryTraces();
    TS_ASSERT_EQUALS(2, vector.size());
    TS_ASSERT_EQUALS(vector[0].getEndPoint() , Point2D(19.802, 98.0198));
    // Yay
  };

  void testNecessaryTraces2() {
    RotationTrace trace(Point2D(-50, 90),
			Point2D(50, 90),
			Point2D(0,90), false);
    std::vector<RotationTrace> vector = trace.getNecessaryTraces();
    TS_ASSERT_EQUALS(3, vector.size());
    LOG_DEBUG("Start [0]: " << vector[0].getStartPoint().x << ", " << vector[0].getStartPoint().y);
    LOG_DEBUG("End [0]: " << vector[0].getEndPoint().x << ", " << vector[0].getEndPoint().y);
    LOG_DEBUG("Start [1]: " << vector[1].getStartPoint().x << ", " << vector[1].getStartPoint().y);
    LOG_DEBUG("End [1]: " << vector[1].getEndPoint().x << ", " << vector[1].getEndPoint().y);
    LOG_DEBUG("Start [2]: " << vector[2].getStartPoint().x << ", " << vector[2].getStartPoint().y);
    LOG_DEBUG("End [2]: " << vector[2].getEndPoint().x << ", " << vector[2].getEndPoint().y);
    
    TS_ASSERT_EQUALS(vector[0].getEndPoint() , Point2D(-41.574, 62.2222));
  }

  void testNecessaryTraces3() {
    LOG_DEBUG("testNecessaryTraces3");
    RotationTrace trace(Point2D(-50, 90),
			Point2D(-50, 90),
			Point2D(0,90), false);
    std::vector<RotationTrace> vector = trace.getNecessaryTraces();
    TS_ASSERT_EQUALS(3, vector.size());
    LOG_DEBUG("Start [0]: " << vector[0].getStartPoint().x << ", " << vector[0].getStartPoint().y);
    LOG_DEBUG("End [0]: " << vector[0].getEndPoint().x << ", " << vector[0].getEndPoint().y);
    LOG_DEBUG("Start [1]: " << vector[1].getStartPoint().x << ", " << vector[1].getStartPoint().y);
    LOG_DEBUG("End [1]: " << vector[1].getEndPoint().x << ", " << vector[1].getEndPoint().y);
    LOG_DEBUG("Start [2]: " << vector[2].getStartPoint().x << ", " << vector[2].getStartPoint().y);
    LOG_DEBUG("End [2]: " << vector[2].getEndPoint().x << ", " << vector[2].getEndPoint().y);
    TS_ASSERT_EQUALS(vector[0].getEndPoint() , Point2D(-41.574, 62.2222));
  }

  
  void testNecessaryTraces4() {
    LOG_DEBUG("testNecessaryTraces4");
    RotationTrace trace(Point2D(-55, 0),
			Point2D(45, 0),
			Point2D(-5,0), false);
    std::vector<RotationTrace> vector = trace.getNecessaryTraces();
    TS_ASSERT_EQUALS(2, vector.size());
    LOG_DEBUG("Start [0]: " << vector[0].getStartPoint().x << ", " << vector[0].getStartPoint().y);
    LOG_DEBUG("End [0]: " << vector[0].getEndPoint().x << ", " << vector[0].getEndPoint().y);
    LOG_DEBUG("Start [1]: " << vector[1].getStartPoint().x << ", " << vector[1].getStartPoint().y);
    LOG_DEBUG("End [1]: " << vector[1].getEndPoint().x << ", " << vector[1].getEndPoint().y);
    TS_ASSERT_EQUALS(vector[0].getEndPoint() , Point2D(-5, -50));
  }

  void testCircleHasOrigin() {
    // The necessary traces need a different calculation of the circle is within the origin of the axis
    // As this seem to be posible on the hardware side we need to do it 2
    // first case, (0,0) as center
    RotationTrace trace(Point2D(-3, 4),
			Point2D(3, 4),
			Point2D(0, 0), true);
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
			Point2D(0, 0), true);
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
			Point2D(0, 0), true);
    std::vector<Point2D> points =  trace.estimateTrace(5);
    TS_ASSERT_EQUALS(points.size(), 5);
    std::vector<Point2D> knownPoints(
				     {Point2D(0, 4),
					 Point2D(4, 0),
					 Point2D(0, -4),
					 Point2D(-4, 0),
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
			Point2D(0, 0), true);
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
    TS_ASSERT_DELTA(RotationTrace(Point2D(10, 1), Point2D(10, 1), Point2D(0, 1), true).
		    getPerpendicularDistance(Point2D(0, 1)), 10, 0.0001);
    TS_ASSERT_DELTA(RotationTrace(Point2D(13,5), Point2D(13, 5), Point2D(3, 5), true).
		    getPerpendicularDistance(Point2D(3, 5)), 10, 0.0001);
  }

  void testExtremePointsFullCirlce() {
    LOG_DEBUG("testExtremePoints_FullCirlce");
    RotationTrace trace(Point2D(10, 1), Point2D(10, 1), Point2D(0, 1), true);
    std::vector<Point2D> pointVector;
    trace.getExtremePoints(&pointVector);
    TS_ASSERT_EQUALS(pointVector.size(),4);
    TS_ASSERT_EQUALS(pointVector[0], Point2D(10, 1));
    TS_ASSERT_EQUALS(pointVector[1], Point2D(0, -9));
    TS_ASSERT_EQUALS(pointVector[2], Point2D(-10, 1));
    TS_ASSERT_EQUALS(pointVector[3], Point2D(0, 11));

    LOG_DEBUG("Point [0]: " << pointVector[0].x << ", " << pointVector[0].y);
    LOG_DEBUG("Point [1]: " << pointVector[1].x << ", " << pointVector[0].y);
    LOG_DEBUG("Point [2]: " << pointVector[2].x << ", " << pointVector[0].y);
    LOG_DEBUG("Point [3]: " << pointVector[3].x << ", " << pointVector[0].y);
  }

  void testExtremePointsHalfCirlce2() {
    LOG_DEBUG("testExtremePoints_HalfCirlce2");
    RotationTrace trace(Point2D(-100, 75), Point2D(100, 75), Point2D(0, 75), false);
    std::vector<Point2D> pointVector;
    trace.getExtremePoints(&pointVector);
  }    

  void testNecessaryTracesHalfCirlce() {
    LOG_DEBUG("testExtremePoints_HalfCirlce");
    RotationTrace trace(Point2D(10, 1), Point2D(-10, 1), Point2D(0, 1), false);
    std::vector<Point2D> pointVector;
    trace.getExtremePoints(&pointVector);
    TS_ASSERT_EQUALS(pointVector.size(),1);
    TS_ASSERT_EQUALS(pointVector[0], Point2D(0, 11));
  }

  
  void testExtremePointsHalfCirlceMinusABit() {
    LOG_DEBUG("testExtremePointsHalfCirlceMinusABit");
    RotationTrace trace(Point2D(10, 1), Point2D(-2.58819, 10.659258), Point2D(0, 1), false);
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
