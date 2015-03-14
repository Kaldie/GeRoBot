// MyTestSuite1.h

#ifndef MOTOR_STEPPERDRIVERUNITTEST_H_
#define MOTOR_STEPPERDRIVERUNITTEST_H_

#include <cxxtest/TestSuite.h>
#include <macroHeader.h>
#include <vector>
#include <string>
#include "../StepperDriver.h"

class StepperDriverUnitTest : public CxxTest::TestSuite {
 public:
  void testCreation(void) {
    LOG_DEBUG("Starting StepperDriverUnitTest::testCreation");
    StepperDriver stepperDriver;
    TS_ASSERT_EQUALS(stepperDriver.numberOfStatesPerStep(),
                     2);
    TS_ASSERT_EQUALS(stepperDriver.getHoldMotor(),
                     false);
    PinVector pinVector{2, 3, 4};
    StepperDriver stepperDriver1(pinVector);
    TS_ASSERT_EQUALS(stepperDriver.getPinStatePointer()->getPinVector(),
                     pinVector);
    std::string defaultDirection = "CCW";
    StepperDriver stepperDriver2(pinVector,
                                 defaultDirection);
    TS_ASSERT_EQUALS(stepperDriver2.getDefaultDirection(),
                     defaultDirection);
  }


  void testMoveStep() {
    LOG_DEBUG("Starting StepperDriverUnitTest:: testMoveStep");
    PinStateSequence pinStateSequence1;
    StepperDriver stepperDriver;
    stepperDriver.setHoldMotor(false);

    stepperDriver.moveStep("CCW",
                           pinStateSequence1);
    TS_ASSERT_EQUALS(pinStateSequence1.getIntegerSequence().size(),
                     4);
    std::vector<int> integerSequence1 = pinStateSequence1.getIntegerSequence();

    std::vector<int> benchMarkVector1 {
          DEFAULT_STATE * 16 + !DEFAULT_STATE * (4 + 8),
          !DEFAULT_STATE * (8 + 16),
          DEFAULT_STATE * 16 + !DEFAULT_STATE * (4 + 8),
          DEFAULT_STATE * (16+4) + !DEFAULT_STATE * (8)};

    TS_ASSERT_EQUALS(integerSequence1, benchMarkVector1);

    PinStateSequence pinStateSequence2;
    stepperDriver.setHoldMotor(true);
    stepperDriver.moveStep("CW",
                           pinStateSequence2);

    std::vector<int> integerSequence2 = pinStateSequence2.getIntegerSequence();

    TS_ASSERT_EQUALS(pinStateSequence2.getIntegerSequence().size(),
                     3);

    std::vector<int> benchMarkVector2 {
      DEFAULT_STATE * (8 + 16) + !DEFAULT_STATE * (4),
      DEFAULT_STATE * 8 + !DEFAULT_STATE * (4+16),
      DEFAULT_STATE * (8 + 16) + !DEFAULT_STATE * (4)};

    TS_ASSERT_EQUALS(integerSequence2, benchMarkVector2);
  }

  void testMoveSteps() {
    LOG_DEBUG("Starting StepperDriverUnitTest:: testMoveSteps");
    StepperDriver stepperDriver(std::vector<int>{3, 4, 5});
    PinStateSequenceVector pinStateSequenceVector;
    stepperDriver.moveSteps("CW",
                            10,
                            pinStateSequenceVector);

    TS_ASSERT_EQUALS(pinStateSequenceVector.size(),
                     3);

    TS_ASSERT_EQUALS(pinStateSequenceVector[0].getNumberOfRepetitions(),
                     1);
    TS_ASSERT_EQUALS(pinStateSequenceVector[0].getIntegerSequence().size(),
                     3);
    
    TS_ASSERT_EQUALS(pinStateSequenceVector[1].getNumberOfRepetitions(),
                     9);
    TS_ASSERT_EQUALS(pinStateSequenceVector[1].getIntegerSequence().size(),
                     2);
    std::vector<int> benchmarkVector1 { DEFAULT_STATE * (16) + !DEFAULT_STATE * (32 + 64),
                                        DEFAULT_STATE * (16+32) + !DEFAULT_STATE * (64)};
    TS_ASSERT_EQUALS(pinStateSequenceVector[1].getIntegerSequence(),
                     benchmarkVector1);

    TS_ASSERT_EQUALS(pinStateSequenceVector[2].getNumberOfRepetitions(),
                     1);
    TS_ASSERT_EQUALS(pinStateSequenceVector[2].getIntegerSequence().size(),
                     1);
  }

};

#endif  // MOTOR_STEPPERDRIVERUNITTEST_H_
