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
  void testCreation(void) {
    TranslationalJoint<StepperDriver> translationalJoint1;

    TranslationalJoint<StepperDriver> translationalJoint2(10.1,
                                                    0.3);

    StepperDriver driver;
    TranslationalJoint<StepperDriver> translationalJoint3(10.1,
                                                    0.3,
                                                    driver);

    TranslationalJoint<StepperDriver> translationalJoint4(0.0,
                                                    1.0,
                                                    DirectionConversionMap {
                                                      {"CCW","IN"},
                                                      {"CW","OUT"}
                                                    },
                                                    driver);
  }

  void testPredictStep() {
    TranslationalJoint<StepperDriver> translationalJoint(50, 1);
    translationalJoint.setRange(std::vector<traceType>{50, 180});
    Point2D point(0, 50);
    for (int i = 0;
         i < 45;
         i++) {
      translationalJoint.predictSteps(&point, "OUT", 1);
    }
    TS_ASSERT_EQUALS(point, Point2D(0, 95));
  }


  void testPredictSteps() {
    TranslationalJoint<StepperDriver> translationalJoint(50, 1);
    translationalJoint.setRange(std::vector<traceType>{50, 180});
    Point2D point(0, 50);

    std::string direction("OUT");
    std::string contraDirection("IN");
    translationalJoint.predictSteps(&point, direction, 90);
    LOG_DEBUG(point.x << ", "<< point.y);
    TS_ASSERT_EQUALS(point, Point2D(0, 140));
    translationalJoint.predictSteps(&point, contraDirection, 90);
    TS_ASSERT_EQUALS(point, Point2D(0, 50));
  }

  void testClone() {
    StepperDriver stepperDriver({5,7,4});
    TranslationalJoint<StepperDriver> translationalJoint(90, 1, stepperDriver);
    PinVector pinVector = translationalJoint.getMotor()->getPins();
    std::vector<traceType> rangeVector {0, 180};
    int currentPosition = translationalJoint.getPosition();
    translationalJoint.setRange(rangeVector);

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
