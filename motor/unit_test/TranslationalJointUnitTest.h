// Copyright [2015] Ruud Cools

#ifndef MOTOR_UNIT_TEST_TRANSLATIONALJOINTUNITTEST_H_
#define MOTOR_UNIT_TEST_TRANSLATIONALJOINTUNITTEST_H_

#include <cxxtest/TestSuite.h>
#include <StepperDriver.h>
#include <TranslationalJoint.h>
#include <macroHeader.h>
#include <vector>
#include <string>


class TranslationalJointUnitTest : public CxxTest::TestSuite {
 public:
  TranslationalJoint<StepperDriver> translationalJoint;

  void setUp() {
    translationalJoint.setPosition(50);
    translationalJoint.setMovementPerStep(1);
  }


  void testPredictStep() {
    translationalJoint.setRange(std::vector<traceType>{50, 180});
    Point2D point(50, 0);
    for (int i = 0;
         i < 45;
         i++) {
      translationalJoint.predictSteps(&point, "OUT", 1);
    }
    TS_ASSERT_EQUALS(point, Point2D(95, 0));
  }


  void testPredictSteps() {
      translationalJoint.setRange(std::vector<traceType>{50, 180});
    Point2D point(50, 0);

    std::string direction("OUT");
    std::string contraDirection("IN");
    translationalJoint.predictSteps(&point, direction, 90);
    LOG_DEBUG(point.x << ", "<< point.y);
    TS_ASSERT_EQUALS(point, Point2D(140, 0));
    translationalJoint.predictSteps(&point, contraDirection, 90);
    TS_ASSERT_EQUALS(point, Point2D(50, 0));
  }

  void testClone() {
    StepperDriver stepperDriver({5,7,4});
    *translationalJoint.getMotor() = stepperDriver;
    std::vector<traceType> rangeVector {0, 180};
    translationalJoint.setRange(rangeVector);
    int currentPosition = translationalJoint.getPosition();
    PinVector pinVector = translationalJoint.getMotor()->getPins();

    std::shared_ptr<BaseJoint> cloned = translationalJoint.clone();

    TS_ASSERT_EQUALS(cloned->getMotor()->getPins(), pinVector);
    TS_ASSERT_EQUALS(cloned->getPosition(), currentPosition);
    TS_ASSERT_EQUALS(cloned->getRange(), rangeVector);
    TS_ASSERT_EQUALS(cloned->getMovementPerStep(),
                     translationalJoint.getMovementPerStep());

    TS_ASSERT_EQUALS(cloned->getDirectionConversionMap(),
                     translationalJoint.getDirectionConversionMap());
  }
};

#endif  // MOTOR_UNIT_TEST_TRANSLATIONALJOINTUNITTEST_H_
