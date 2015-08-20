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
  void testCreation(void) {
    RotationalJoint<StepperDriver> rotationalJoint1;

    RotationalJoint<StepperDriver> rotationalJoint2(10.1,
                                                    0.3);

    StepperDriver driver;
    RotationalJoint<StepperDriver> rotationalJoint3(10.1,
                                                    0.3,
                                                    driver);

    RotationalJoint<StepperDriver> rotationalJoint4(0.0,
                                                    1.0,
                                                    DirectionConversionMap {
                                                      {"CCW","CCW"},
                                                      {"CW","CW"}
                                                    },
                                                    driver);
  }

  void testPredictStep() {
    RotationalJoint<StepperDriver> rotationalJoint(90, 1);
    rotationalJoint.setRange(std::vector<traceType>{0, 180});
    Point2D point(0, 50);
    for (int i = 0;
         i < 45;
         i++) {
      rotationalJoint.predictStep(point, "CCW");
    }
    TS_ASSERT_EQUALS(point, Point2D(-35.3553, 35.3553));
  }


  void testPredictSteps() {
    RotationalJoint<StepperDriver> rotationalJoint(90, 1);
    rotationalJoint.setRange(std::vector<traceType>{0, 180});
    Point2D point(0, 50);

    std::string direction("CW");
    std::string contraDirection("CCW");
    rotationalJoint.predictSteps(point, direction, 90);
    LOG_DEBUG(point.x << ", "<< point.y);
    TS_ASSERT_EQUALS(point, Point2D(50, 0));
    rotationalJoint.predictSteps(point, contraDirection, 90);
    TS_ASSERT_EQUALS(point, Point2D(0, 50));
    
  }

  void testClone() {

    StepperDriver stepperDriver({5,7,4});
    RotationalJoint<StepperDriver> rotationalJoint(90, 1, stepperDriver);
    PinVector pinVector = rotationalJoint.getMotor()->getPins();
    std::vector<traceType> rangeVector {0, 180};
    int currentPosition = rotationalJoint.getPosition();
    rotationalJoint.setRange(rangeVector);
    
    


    std::shared_ptr<BaseJoint> cloned = rotationalJoint.clone();

    TS_ASSERT_EQUALS(cloned->getMotor()->getPins(), pinVector);
    TS_ASSERT_EQUALS(cloned->getPosition(), currentPosition);
    TS_ASSERT_EQUALS(cloned->getRange(), rangeVector);
    TS_ASSERT_EQUALS(cloned->getMovementPerStep(),
                     rotationalJoint.getMovementPerStep());

    TS_ASSERT_EQUALS(cloned->getDirectionConversionMap(),
                     rotationalJoint.getDirectionConversionMap());
  }
};

#endif  // MOTOR_UNIT_TEST_ROTATIONALJOINTUNITTEST_H_
