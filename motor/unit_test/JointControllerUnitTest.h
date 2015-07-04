// Copyright [2015] Ruud Cools

#ifndef MOTOR_UNIT_TEST_JOINTCONTROLLERUNITTEST_H_
#define MOTOR_UNIT_TEST_JOINTCONTROLLERUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <StepperDriver.h>
#include <RotationalJoint.h>
#include <TranslationalJoint.h>
#include <JointController.h>
#include <vector>
#include <string>


class JointControllerTestSuite : public CxxTest::TestSuite {
 public:
  StepperDriver stepperDriver1;
  StepperDriver stepperDriver2;
  RotationalJoint<StepperDriver> rotationalJoint;
  TranslationalJoint<StepperDriver> translationalJoint;
  JointController jointController;

  void setUp() {
    stepperDriver1.setPins({5, 6, 7});
    stepperDriver2.setPins({2, 3, 4});

    (*rotationalJoint.getMotor()) = stepperDriver1;
    (*translationalJoint.getMotor()) = stepperDriver2;
    JointController jointController;
    jointController.addJoint(rotationalJoint.clone());
    jointController.addJoint(translationalJoint.clone());
  }

  void tearDown(){
    jointController.resetPinStateSequence();
  }

  void testCreation(void) {
    JointController jointController2;
    TS_ASSERT_EQUALS(
        jointController2.getSequenceVector().numberOfSequences(),
        0);
    TS_ASSERT_EQUALS(jointController2.getJointPointerVector().size(),
                    0);
  }


  void testAddJoint(void) {
    LOG_INFO("JointControlerUnitTest::test AddJoint");
    StepperDriver stepperDriver3({5, 6, 7});
    StepperDriver stepperDriver4({2, 3, 4});
    RotationalJoint<StepperDriver> rotationalJoint2(50, 1, stepperDriver3);
    TranslationalJoint<StepperDriver>
        translationalJoint2(50, 1, stepperDriver4);

    JointController jointController2;

    TS_ASSERT(jointController.addJoint(rotationalJoint2.clone()));
    TS_ASSERT_THROWS(jointController.addJoint(rotationalJoint2.clone()),
                     std::runtime_error);

    TS_ASSERT(jointController.addJoint(translationalJoint2.clone()));
    TS_ASSERT_THROWS(jointController.addJoint(translationalJoint2.clone()),
                     std::runtime_error);
  }


  void testResetPinStateSequence() {
    SequenceVector pinStateVector =
        jointController.getSequenceVector();
    TS_ASSERT_EQUALS(pinStateVector.numberOfSequences(), 0);
    LOG_INFO("JointControlerUnitTest::test resetPinState");
    JointPointer rotationalJointPointer = jointController.getJoint(Rotational);
    jointController.moveStep(rotationalJointPointer, "CW", false);
    pinStateVector =
        jointController.getSequenceVector();

    TS_ASSERT_DIFFERS(pinStateVector.numberOfSequences(), 0);
    TS_ASSERT_EQUALS(pinStateVector.numberOfSequences(), 1);

    jointController.resetPinStateSequence();
    pinStateVector =
        jointController.getSequenceVector();

    TS_ASSERT_EQUALS(pinStateVector.numberOfSequences(), 1);
    StateSequence stateSequence = *pinStateVector.begin();
    TS_ASSERT_EQUALS(stateSequence.getPinStateVector().size(), 1);
    TS_ASSERT_EQUALS(stateSequence.getNumberOfRepetitions(), 0);
    TS_ASSERT_EQUALS((*pinStateVector.begin()).getIntegerSequence(),
                     std::vector<int> {224});
  }

  void testMoveStep() {
    LOG_INFO("JointControlerUnitTest::test move step");
    JointPointer rotationalJointPointer = jointController.getJoint(Rotational);
    JointPointer translationJointPointer =
        jointController.getJoint(Translational);

    jointController.moveStep(rotationalJointPointer, "CCW", true);

    TS_ASSERT_EQUALS(
        jointController.getSequenceVector().numberOfSequences(), 2);
    std::vector<int> integerSequence =
        (*(--jointController.getSequenceVector().end())).getIntegerSequence();

    TS_ASSERT_EQUALS(
        integerSequence,
        (std::vector<int> {128, 0, 128, 160}));

    // step also with the other joint
    jointController.moveStep(translationJointPointer, "OUT", true);

    LOG_DEBUG("Test if the pin state has the same number of steps");
    TS_ASSERT_EQUALS(jointController.getSequenceVector().numberOfSequences(),
                     2);

    integerSequence = (*(jointController.getSequenceVector().
                          end()-1)).getIntegerSequence();

    TS_ASSERT_EQUALS(
        integerSequence,
        (std::vector<int> {152, 8, 152, 188}));
  }

  void testMoveSteps() {
    LOG_INFO("JointControlerUnitTest:: test move steps");
    JointPointer rotationalJointPointer = jointController.getJoint(Rotational);
    JointPointer translationJointPointer =
        jointController.getJoint(Translational);

    jointController.moveSteps(rotationalJointPointer, "CW", 5);
    TS_ASSERT_EQUALS(
        jointController.getSequenceVector().numberOfSequences(), 4);

    if (jointController.getSequenceVector().numberOfSequences() != 4) {
      LOG_ERROR("Should be 4!");
    }

    std::vector<int> numberOfRepetitionsOfVector {0, 1, 4, 1};
    std::vector<std::vector<int>> integerSequence{{188},
                                                  {192, 64, 192},
                                                  {64, 192}, {224}};


    auto repetitionIter = numberOfRepetitionsOfVector.begin();
    auto integerSequenceIter = integerSequence.begin();
    auto sequenceIterator = jointController.getSequenceVector().begin();

    for (;
         sequenceIterator != jointController.getSequenceVector().end();
         sequenceIterator++, repetitionIter++, integerSequenceIter++) {
      TS_ASSERT_EQUALS(sequenceIterator->getNumberOfRepetitions(),
                       *repetitionIter);

      TS_ASSERT_EQUALS(sequenceIterator->getIntegerSequence(),
                       *integerSequenceIter);
    }

    TS_ASSERT(!jointController.getSequenceVector().isNormilized());

    SequenceVector sequenceVector = jointController.getSequenceVector();
    sequenceVector.normalise();
    TS_ASSERT(sequenceVector.isNormilized());

    try {
      LOG_DEBUG("aya");
      jointController.actuate();
      LOG_DEBUG("paya");
    }
    catch(std::runtime_error)
    {}
  }
};



#endif  // MOTOR_UNIT_TEST_JOINTCONTROLLERUNITTEST_H_
