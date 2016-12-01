// MyTestSuite1.h

#ifndef MOTOR_STEPPERDRIVERUNITTEST_H_
#define MOTOR_STEPPERDRIVERUNITTEST_H_


#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <vector>
#include <string>
#include <SequenceVector.h>
#include <StateSequence.h>
#include "../StepperDriver.h"

class StepperDriverUnitTest : public CxxTest::TestSuite {
 public:
  void testCreation(void) {
    LOG_DEBUG("Starting StepperDriverUnitTest::testCreation");
    StepperDriver stepperDriver;
    TS_ASSERT_EQUALS(stepperDriver.getType(), BaseMotor::MotorType::StepperDriver);
    TS_ASSERT_EQUALS(stepperDriver.numberOfStatesPerStep(),
                     2);
    TS_ASSERT_EQUALS(stepperDriver.getHoldMotor(),
                     false);
    PinVector pinVector{2, 3, 4};
    StepperDriver stepperDriver1(pinVector);
    TS_ASSERT_EQUALS(stepperDriver.getCurrentPinState().getPinVector(),
                     pinVector);
    std::string defaultDirection = "CCW";
    StepperDriver stepperDriver2(pinVector,
                                 defaultDirection,
                                 200, 200, 500);
    TS_ASSERT_EQUALS(stepperDriver2.getDefaultDirection(),
                     defaultDirection);
  }


  void testMoveStep() {
    LOG_DEBUG("Starting StepperDriverUnitTest:: testMoveStep");
    StateSequence stateSequence1;
    StepperDriver stepperDriver;
    stepperDriver.setHoldMotor(false);

    stepperDriver.moveStep("CCW", &stateSequence1);
    TS_ASSERT_EQUALS(stateSequence1.getIntegerSequence().size(),
                     4);
    std::vector<int> integerSequence1 = stateSequence1.getIntegerSequence();

    std::vector<int> benchMarkVector1 {
          DEFAULT_STATE * 16 + !DEFAULT_STATE * (4 + 8),
          !DEFAULT_STATE * (8 + 16),
          DEFAULT_STATE * 16 + !DEFAULT_STATE * (4 + 8),
          DEFAULT_STATE * (16+4) + !DEFAULT_STATE * (8)};

    TS_ASSERT_EQUALS(integerSequence1, benchMarkVector1);

    StateSequence pinStateSequence2;
    stepperDriver.setHoldMotor(true);
    stepperDriver.moveStep("CW",
                           &pinStateSequence2);

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
    SequenceVector sequenceVector;

    stepperDriver.moveSteps("CW", 10, &sequenceVector);

    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(),
                     3);

    StateSequence thisSequence = *sequenceVector.begin();

    TS_ASSERT_EQUALS(thisSequence.getNumberOfRepetitions(),
                     1);
    TS_ASSERT_EQUALS(thisSequence.getIntegerSequence().size(),
                     3);

    thisSequence = *(++sequenceVector.begin());
    TS_ASSERT_EQUALS(thisSequence.getNumberOfRepetitions(),
                     9);
    TS_ASSERT_EQUALS(thisSequence.getIntegerSequence().size(),
                     2);
    std::vector<int> benchmarkVector1 { DEFAULT_STATE * (16) + !DEFAULT_STATE * (32 + 64),
                                        DEFAULT_STATE * (16+32) + !DEFAULT_STATE * (64)};
    TS_ASSERT_EQUALS(thisSequence.getIntegerSequence(),
                     benchmarkVector1);

    //    thisSequence = *(sequenceVector.begin()+2);
    //TS_ASSERT_EQUALS(thisSequence.getNumberOfRepetitions(),
    //                1);
    //    TS_ASSERT_EQUALS(thisSequence.getIntegerSequence().size(),
    //                     1);
  }
};

#endif  // MOTOR_STEPPERDRIVERUNITTEST_H_
