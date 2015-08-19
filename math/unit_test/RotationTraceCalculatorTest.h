// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_ROTATIONTRACECALCULATORTEST_H_
#define MATH_UNIT_TEST_ROTATIONTRACECALCULATORTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <StepperDriver.h>
#include <RotationalJoint.h>
#include <TranslationalJoint.h>
#include <JointController.h>
#include <RotationTrace.h>
#include <RotationTraceCalculator.h>

class RotationTraceCalculatorTest : public CxxTest::TestSuite {
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
    jointController.addJoint(rotationalJoint.clone());
    jointController.addJoint(translationalJoint.clone());
  }

  void testRotationTraceCalculation() {

    Point2D startPoint(-10, 30);
    Point2D endPoint(-50, 30);
    Point2D centrePoint(-30, 30);
    RotationTrace trace(startPoint,
                        endPoint,
                        centrePoint);

    jointController.getJoint(Translational)->setMovementPerStep(0.01);
    jointController.getJoint(Rotational)->setMovementPerStep(0.01);

    trace.setRotationTolerance(
        jointController.getJoint(Rotational)->getMovementPerStep()*1);

    trace.setTranslationTolerance(
        jointController.getJoint(Translational)->getMovementPerStep()*1);

    jointController.getJoint(Translational)->getMotor()->setHoldMotor(true);
    jointController.getJoint(Rotational)->getMotor()->setHoldMotor(true);

    RotationTraceCalculator rotationTraceCalculator(&jointController);
    rotationTraceCalculator.setWriteLog(true);
    rotationTraceCalculator.calculateTrace(&trace, startPoint);

    /// Tests
    TS_ASSERT_EQUALS(jointController.getSequenceVector().numberOfSequences(),
                     4904);
    TS_ASSERT_EQUALS(jointController.getSequenceVector().numberOfSteps(),
                     21948);

    SequenceVector sequenceVector = jointController.getSequenceVector();
    
    /// testing condensing on this big vector
    sequenceVector.normalise();
    long totalNumberOfReps = 0;
    for (auto& sequence : sequenceVector.getSequenceVector()) {
      totalNumberOfReps += sequence.getNumberOfRepetitions();
    }

    double unOptimisedAverageRep = static_cast<double>(totalNumberOfReps)/
        sequenceVector.numberOfSequences();

    /// test efficiency
    TS_ASSERT_DELTA(2.2371, unOptimisedAverageRep, 0.001);

    /// test if condensing works
    TS_ASSERT(sequenceVector.condenseVector());

    int numberOfEmptySequences = 0;
    totalNumberOfReps = 0;
    for (auto& sequence : sequenceVector.getSequenceVector()) {
      if (sequence.getNumberOfRepetitions() == 0)
        numberOfEmptySequences += 1;
      totalNumberOfReps += sequence.getNumberOfRepetitions();
    }
    
    TS_ASSERT_EQUALS(numberOfEmptySequences, 3436);
    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 6198);
    
    double optimisedAverageRep = static_cast<float>(totalNumberOfReps)/
        (sequenceVector.numberOfSequences() - numberOfEmptySequences);
    /// check if the condensing improves information densitity
    TS_ASSERT(optimisedAverageRep > unOptimisedAverageRep);
    /// Check the actual number
    TS_ASSERT_DELTA(optimisedAverageRep, 3.7943, 0.001);

    for (auto& i : std::vector<int>({5707,6106,6084, 5714})) {
      TS_ASSERT_EQUALS(
          sequenceVector.getSequenceVector()[i].getIntegerSequence(),
          std::vector<int>({64, 192}));
    }

    for (auto& i : std::vector<int>({6188, 6182, 6172, 6102 })) {
      TS_ASSERT_EQUALS(
          sequenceVector.getSequenceVector()[i].getIntegerSequence(),
          std::vector<int>({64, 192, 64, 208, 64, 192}));
    }


    /* Code the find nice sequences
      int k = 0;
      for (auto& i: sequenceVector.getSequenceVector()) {
        std::cout << k << ", ";
        ++k;
        for (auto& j : i.getIntegerSequence()) {
          std::cout << j << ", ";
        }
        std::cout << i.getNumberOfRepetitions() << ", "<< std::endl;
      }
    */
    try {
      LOG_DEBUG("here!!");
      jointController.actuate();
    } catch(std::runtime_error) {
      LOG_INFO("Could not find sizzle");
    }
  }
};
#endif  // MATH_UNIT_TEST_ROTATIONTRACECALCULATORTEST_H_


