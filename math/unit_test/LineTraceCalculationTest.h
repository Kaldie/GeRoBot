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
  Robot robot;
  void setUp() {
    StepperDriver stepperDriver1({5, 6, 7});
    StepperDriver stepperDriver2({2, 3, 4});
    TranslationalJoint<StepperDriver> translationalJoint(50.0, 0.01);
    RotationalJoint<StepperDriver> rotationalJoint(90.0, 0.01);
    (*rotationalJoint.getMotor()) = stepperDriver1;
    (*translationalJoint.getMotor()) = stepperDriver2;
    JointController jointController;
    jointController.addJoint(rotationalJoint.clone());
    jointController.addJoint(translationalJoint.clone());
    robot.setJointController(std::make_shared<JointController>(jointController));
    LOG_DEBUG("Number of joints: " << jointController.getNumberOfJoints());
    LOG_DEBUG("Position: " << robot.getJointController()->resolveJoint(Rotational)->getPosition());
    LOG_DEBUG("Number of joints: " << robot.getJointController()->getNumberOfJoints());
  }

  void testLineTraceCalculation() {
    Point2D startPoint(0, 50);
    Point2D endPoint(-150, 50);
    Trace trace(startPoint, endPoint);

    trace.setRotationTolerance(
        robot.getJointController()->resolveJoint(Rotational)->getMovementPerStep()*1.5);

    trace.setTranslationTolerance(
        robot.getJointController()->resolveJoint(Translational)->getMovementPerStep()*1.5);

    LineTraceCalculator lineTraceCalculator(&robot);
    lineTraceCalculator.setWriteLog(true);
    lineTraceCalculator.calculateTrace(trace);

    /// Tests
    TS_ASSERT_EQUALS(robot.getJointController()->getSequenceVector().numberOfSequences(),
                     7411);
    TS_ASSERT_EQUALS(robot.getJointController()->getSequenceVector().numberOfSteps(),
                     26458);

    SequenceVector sequenceVector = robot.getJointController()->getSequenceVector();
    
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
      robot.getJointController()->actuate();
    } catch(std::runtime_error) {
      LOG_INFO("Could not find sizzle");
    }
  }
};
#endif  // MATH_UNITTEST_LINETRACECALCULATIONTEST_H_

