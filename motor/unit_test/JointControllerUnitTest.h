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


class JointControllerUnitTest : public CxxTest::TestSuite {
 public:
  StepperDriver stepperDriver1;
  StepperDriver stepperDriver2;
  BaseJoint::JointPointer rotationalJoint;
  BaseJoint::JointPointer translationalJoint;
  JointController jointController;

  void setUp() {
    stepperDriver1.setPins({5, 6, 7});
    stepperDriver2.setPins({2, 3, 4});
    // Create rotational joint
    rotationalJoint = std::make_shared<RotationalJoint<StepperDriver>>();
    rotationalJoint->setMovementPerStep(1);
    rotationalJoint->setPosition(90);
    rotationalJoint->setRange(std::vector<traceType>({0, 2* PI}));
    rotationalJoint->setDirectionConversionMap({{"CCW", "CCW"}, {"CW","CW"}});

    translationalJoint = std::make_shared<TranslationalJoint<StepperDriver>>();
    translationalJoint->setMovementPerStep(1);
    translationalJoint->setPosition(50);
    translationalJoint->setRange(std::vector<traceType>({50, 1000}));
    translationalJoint->setDirectionConversionMap({{"IN", "CCW"}, {"OUT","CW"}});

    *rotationalJoint->getMotor() = stepperDriver1;
    rotationalJoint->setChild(translationalJoint);

    *translationalJoint->getMotor() = stepperDriver2;

    jointController = JointController();
    jointController.addJoint(rotationalJoint);
    jointController.addJoint(translationalJoint);
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
    BaseJoint::JointPointer rotationalJoint2 =
      std::make_shared<RotationalJoint<StepperDriver>>();
    rotationalJoint2->setPosition(50);
    rotationalJoint2->setMovementPerStep(1);
    *rotationalJoint2->getMotor() =  stepperDriver3;
    BaseJoint::JointPointer translationalJoint2 =
      std::make_shared<TranslationalJoint<StepperDriver>>();
    translationalJoint2->setPosition(50);
    translationalJoint2->setMovementPerStep(1);
    *translationalJoint2->getMotor() = stepperDriver4;
    JointController jointController2;
    jointController.setJointPointerVector({});
    TS_ASSERT(jointController.addJoint(rotationalJoint2));
    TS_ASSERT_THROWS(jointController.addJoint(rotationalJoint2->clone()),
                     std::runtime_error);

    TS_ASSERT(jointController.addJoint(translationalJoint2));
    TS_ASSERT_THROWS(jointController.addJoint(translationalJoint2->clone()),
                     std::runtime_error);
  }


  void testResetPinStateSequence() {
    SequenceVector pinStateVector =
        jointController.getSequenceVector();
    TS_ASSERT_EQUALS(pinStateVector.numberOfSequences(), 1);
    LOG_INFO("JointControlerUnitTest::test resetPinState");
    jointController.moveSteps("CW", 1);
    pinStateVector =
        jointController.getSequenceVector();

    TS_ASSERT_DIFFERS(pinStateVector.numberOfSequences(), 0);
    TS_ASSERT_EQUALS(pinStateVector.numberOfSequences(), 2);

    jointController.resetPinStateSequence();
    pinStateVector =
        jointController.getSequenceVector();

    TS_ASSERT_EQUALS(pinStateVector.numberOfSequences(), 1);
    StateSequence stateSequence = *pinStateVector.begin();
    TS_ASSERT_EQUALS(stateSequence.getPinStateVector().size(), 1);
    TS_ASSERT_EQUALS(stateSequence.getNumberOfRepetitions(), 0);
    (*pinStateVector.begin()).displaySequence();
    TS_ASSERT_EQUALS((*pinStateVector.begin()).getIntegerSequence(),
                     std::vector<int> {252});
  }

  void testMoveStep() {
    LOG_INFO("JointControlerUnitTest::test move step");
    jointController.moveSteps("CCW", 1);
    TS_ASSERT_EQUALS(
        jointController.getSequenceVector().numberOfSequences(), 2);
    std::vector<int> integerSequence =
        (*(--jointController.getSequenceVector().end())).getIntegerSequence();

    TS_ASSERT_EQUALS(
        integerSequence,
        (std::vector<int> {128, 0, 128, 160}));

    // step also with the other joint
    jointController.moveSteps( "OUT", 1);

    LOG_DEBUG("Test if the pin state has the same number of steps");
    TS_ASSERT_EQUALS(jointController.getSequenceVector().numberOfSequences(),
                     2);

    integerSequence = (*(jointController.getSequenceVector().
                          end()-1)).getIntegerSequence();
    (jointController.getSequenceVector().end()-1)->displaySequence();
    TS_ASSERT_EQUALS(
        integerSequence,
        (std::vector<int> {152, 8, 152, 160, 28}));
  }

  void testMoveSteps() {
    LOG_INFO("JointControlerUnitTest:: test move steps");
    BaseJoint::JointPointer rotationalJointPointer = jointController.resolveJoint(BaseJoint::Rotational);
    BaseJoint::JointPointer translationJointPointer =
      jointController.resolveJoint(BaseJoint::Translational);

    jointController.moveSteps("CW", 5);
    TS_ASSERT_EQUALS(
        jointController.getSequenceVector().numberOfSequences(), 4);

    if (jointController.getSequenceVector().numberOfSequences() != 4) {
      LOG_ERROR("Should be 4!");
    }

    std::vector<int> numberOfRepetitionsOfVector {0, 1, 4, 1};
    std::vector<std::vector<int>> integerSequence{{252},
                                                  {192, 64, 192},
                                                  {64, 192}, {224}};


    auto repetitionIter = numberOfRepetitionsOfVector.begin();
    auto integerSequenceIter = integerSequence.begin();
    auto sequenceIterator = jointController.getSequenceVector().begin();

    for (;
         sequenceIterator != jointController.getSequenceVector().end();
         sequenceIterator++, repetitionIter++, integerSequenceIter++) {
      sequenceIterator->displaySequence();
      TS_ASSERT_EQUALS(sequenceIterator->getNumberOfRepetitions(),
                       *repetitionIter);
      TS_ASSERT_EQUALS(sequenceIterator->getIntegerSequence(),
                       *integerSequenceIter);
    }

    TS_ASSERT(!jointController.getSequenceVector().isNormilized());

    SequenceVector sequenceVector = jointController.getSequenceVector();
    sequenceVector.normalise();
    LOG_DEBUG("Display normalised sequence!");
    for (auto& i : sequenceVector) {
      i.displaySequence();
    }
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
