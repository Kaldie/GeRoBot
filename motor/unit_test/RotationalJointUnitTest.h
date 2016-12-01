// Copyright [2015] Ruud Cools

#ifndef MOTOR_UNIT_TEST_ROTATIONALJOINTUNITTEST_H_
#define MOTOR_UNIT_TEST_ROTATIONALJOINTUNITTEST_H_

#include <cxxtest/TestSuite.h>
#include <StepperDriver.h>
#include <RotationalJoint.h>
#include <macroHeader.h>
#include <vector>
#include <string>


class RotationalJointUnitTest : public CxxTest::TestSuite {
 public:
  RotationalJoint<StepperDriver> rotationalJoint2;

  void setUp(void) {
    rotationalJoint2.setPosition(0.0);
    rotationalJoint2.setMovementPerStep(1.0*PI/180);
    rotationalJoint2.setFixedLength(50.0);
  }


  void testCreation(void) {
    RotationalJoint<StepperDriver> rotationalJoint1;
  }

  
  void testPredictStep() {
    rotationalJoint2.setRange(std::vector<traceType>{0, 180});
    Point2D point(50, 0);
    for (int i = 1;
         i < 46;
         i++) {
      rotationalJoint2.predictSteps(&point, "CCW", 1);
      rotationalJoint2.setPosition(i*rotationalJoint2.getMovementPerStep());
    }
    TS_ASSERT_EQUALS(point, Point2D(35.3553, 35.3553));
  }


  void testPredictSteps() {
    Point2D point(50, 0);
    std::string direction("CW");
    std::string contraDirection("CCW");
    rotationalJoint2.predictSteps(&point, direction, 90);
    rotationalJoint2.setPosition(-90*rotationalJoint2.getMovementPerStep());
    LOG_DEBUG(point.x << ", "<< point.y);
    TS_ASSERT_EQUALS(point, Point2D(0, -50));
    rotationalJoint2.predictSteps(&point, contraDirection, 90);
    rotationalJoint2.setPosition(90*rotationalJoint2.getMovementPerStep());
    TS_ASSERT_EQUALS(point, Point2D(50, 0));
  }

  void testClone() {
    PinVector pinVector = rotationalJoint2.getMotor()->getPins();
    std::vector<traceType> rangeVector {0, 180};
    rotationalJoint2.setRange(rangeVector);
    int currentPosition = rotationalJoint2.getPosition();

    std::shared_ptr<BaseJoint> cloned = rotationalJoint2.clone();
    TS_ASSERT_EQUALS(cloned->getMotor()->getPins(), pinVector);
    TS_ASSERT_EQUALS(cloned->getPosition(), currentPosition);
    TS_ASSERT_EQUALS(cloned->getRange(), rangeVector);
    TS_ASSERT_EQUALS(cloned->getMovementPerStep(),
                     rotationalJoint2.getMovementPerStep());

    TS_ASSERT_EQUALS(cloned->getDirectionConversionMap(),
                     rotationalJoint2.getDirectionConversionMap());
  }
};

#endif  // MOTOR_UNIT_TEST_ROTATIONALJOINTUNITTEST_H_
