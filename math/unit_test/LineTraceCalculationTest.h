// Copyright [2015] Ruud Cools
#ifndef MATH_UNIT_TEST_LINETRACECALCULATIONTEST_H_
#define MATH_UNIT_TEST_LINETRACECALCULATIONTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <StepperDriver.h>
#include <RotationalJoint.h>
#include <TranslationalJoint.h>
#include <JointController.h>
#include <Trace.h>
#include <LineTraceCalculator.h>

class LineTraceCalculationTest : public CxxTest::TestSuite {
 public:
  StepperDriver stepperDriver1;
  StepperDriver stepperDriver2;
  RotationalJoint<StepperDriver> rotationalJoint;
  TranslationalJoint<StepperDriver> translationalJoint;
  JointController::JointControllerPointer jointControllerPointer;

  void setUp() {
    stepperDriver1.setPins({5, 6, 7});
    stepperDriver2.setPins({2, 3, 4});

    (*rotationalJoint.getMotor()) = stepperDriver1;
    (*translationalJoint.getMotor()) = stepperDriver2;
    JointController jointController;
    jointController.addJoint(rotationalJoint.clone());
    jointController.addJoint(translationalJoint.clone());
    jointControllerPointer = std::make_shared<JointController>(jointController);
  }

  void testLineTraceCalculation() {
    Point2D startPoint(0, 50);
    Point2D endPoint(-150, 50);
    Trace trace(startPoint, endPoint);

    jointControllerPointer->resolveJoint(Translational)->setMovementPerStep(0.01);
    jointControllerPointer->resolveJoint(Rotational)->setMovementPerStep(0.01);

    trace.setRotationTolerance(
        jointControllerPointer->resolveJoint(Rotational)->getMovementPerStep()*1.5);

    trace.setTranslationTolerance(
        jointControllerPointer->resolveJoint(Translational)->getMovementPerStep()*1.5);

    jointControllerPointer->resolveJoint(Translational)->getMotor()->setHoldMotor(true);
    jointControllerPointer->resolveJoint(Rotational)->getMotor()->setHoldMotor(true);

    LineTraceCalculator lineTraceCalculator(jointControllerPointer);
    lineTraceCalculator.setWriteLog(true);
    lineTraceCalculator.calculateTrace(&trace, startPoint);

    /// Tests
    TS_ASSERT_EQUALS(jointControllerPointer->getSequenceVector().numberOfSequences(),
                     7411);
    TS_ASSERT_EQUALS(jointControllerPointer->getSequenceVector().numberOfSteps(),
                     26458);

    SequenceVector sequenceVector = jointControllerPointer->getSequenceVector();
    
    /// testing condensing on this big vector
    sequenceVector.normalise();
    long totalNumberOfReps = 0;
    for (auto& sequence : sequenceVector.getSequenceVector()) {
      totalNumberOfReps += sequence.getNumberOfRepetitions();
    }

    double unOptimisedAverageRep = static_cast<double>(totalNumberOfReps)/
        sequenceVector.numberOfSequences();

    /// test efficiency
    TS_ASSERT_DELTA(1.784, unOptimisedAverageRep, 0.001);

    /// test if condensing works
    TS_ASSERT(sequenceVector.condenseVector());

    int numberOfEmptySequences = 0;
    totalNumberOfReps = 0;
    for (auto& sequence : sequenceVector.getSequenceVector()) {
      if (sequence.getNumberOfRepetitions() == 0)
        numberOfEmptySequences += 1;
      totalNumberOfReps += sequence.getNumberOfRepetitions();
    }
    
    TS_ASSERT_EQUALS(numberOfEmptySequences, 4647);
    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 9037);
    
    double optimisedAverageRep = static_cast<float>(totalNumberOfReps)/
        (sequenceVector.numberOfSequences() - numberOfEmptySequences);
    /// check if the condensing improves information densitity
    TS_ASSERT(optimisedAverageRep > unOptimisedAverageRep);
    /// Check the actual number
    TS_ASSERT_DELTA(optimisedAverageRep, 2.8230, 0.001);

    for (auto& i : std::vector<int>({10, 1567, 6567, 8632})) {
      TS_ASSERT_EQUALS(
          sequenceVector.getSequenceVector()[i].getIntegerSequence(),
          std::vector<int>({}));
    }

    for (auto& i : std::vector<int>({56, 3567})) {
      TS_ASSERT_EQUALS(
          sequenceVector.getSequenceVector()[i].getIntegerSequence(),
          std::vector<int>({0, 144}));
    }

    for (auto& i : std::vector<int>({132, 2315})) {
      TS_ASSERT_EQUALS(
          sequenceVector.getSequenceVector()[i].getIntegerSequence(),
          std::vector<int>({0, 128}));
    }

    for (auto& i : std::vector<int>({9028, 9011, 9003, 8998, 8948, 8821})) {
      TS_ASSERT_EQUALS(
          sequenceVector.getSequenceVector()[i].getIntegerSequence(),
          std::vector<int>({0, 144, 0, 16, 0, 144}));
    }


    /* Code the find nice sequences
      int k = 0;
      for (auto& i: sequenceVector.getSequenceVector()) {
      //      std::cout << k << ", ";
      ++k;
      for (auto& j : i.getIntegerSequence()) {
      //        std::cout << j << ", ";
      }
      //      std::cout << std::endl;
      }
    */
    try {
      LOG_DEBUG("here!!");
      jointControllerPointer->actuate();
    } catch(std::runtime_error) {
      LOG_INFO("Could not find sizzle");
    }
  }
};
#endif  // MATH_UNITTEST_LINETRACECALCULATIONTEST_H_

