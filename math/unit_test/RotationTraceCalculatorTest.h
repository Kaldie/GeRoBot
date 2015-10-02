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
#include <RobotIO.h>

class RotationTraceCalculatorTest : public CxxTest::TestSuite {
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
  }

  void testRotationTraceCalculation() {
    Point2D startPoint(-0, 255);
    Point2D endPoint(-0, 255);
    Point2D centrePoint(-0, 345);
    RotationTrace trace(startPoint, endPoint, centrePoint);
    robot.setVirtualPosition(startPoint);
    robot.setPosition(startPoint);
    /*
      robot.getJointController()->resolveJoint(Translational)->setMovementPerStep(0.01);
    robot.getJointController()->resolveJoint(Rotational)->setMovementPerStep(0.01);

    trace.setRotationTolerance(
        robot.getJointController()->resolveJoint(Rotational)->getMovementPerStep()*0.5);

    trace.setTranslationTolerance(
        robot.getJointController()->resolveJoint(Translational)->getMovementPerStep()*0.5);
    trace.setIsClockwise(true);
    robot.getJointController()->resolveJoint(Translational)->getMotor()->setHoldMotor(true);
    robot.getJointController()->resolveJoint(Rotational)->getMotor()->setHoldMotor(true);
    */
    RobotIO robotBuilder("/home/ruud/project/gerobot/gui/defaultRobot.xml");
    robotBuilder.build();
    //    robotBuilder.getRobotPointer();
    Robot robot = *robotBuilder.getRobotPointer();
    robot.setVirtualPosition(startPoint);
    robot.setPosition(startPoint);

    RotationTraceCalculator rotationTraceCalculator(&robot);
    rotationTraceCalculator.setWriteLog(true);
    rotationTraceCalculator.calculateTrace(trace);
    SequenceVector vector = robot.getJointController()->getSequenceVector();
    LOG_DEBUG("Number of steps before condense: " << vector.numberOfSteps());
    //    vector.condenseVector();
    LOG_DEBUG("Number of steps after condense: " << vector.numberOfSteps());
    try {
        LOG_DEBUG("here!!");
        robot.actuate();
    } catch(std::runtime_error) {
        LOG_INFO("Could not find sizzle");
    }
    return;

    /// Tests
    TS_ASSERT_EQUALS(robot.getJointController()->getSequenceVector().numberOfSequences(),
                     4904);
    TS_ASSERT_EQUALS(robot.getJointController()->getSequenceVector().numberOfSteps(),
                     21948);

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
      robot.actuate();
    } catch(std::runtime_error) {
      LOG_INFO("Could not find sizzle");
    }
  }
};
#endif  // MATH_UNIT_TEST_ROTATIONTRACECALCULATORTEST_H_
