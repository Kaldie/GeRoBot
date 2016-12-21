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
#include <ConstantSpeedController.h>


class RotationTraceCalculatorTest : public CxxTest::TestSuite {
 public:
  Robot robot;
  void setUp() {
    StepperDriver stepperDriver1({5, 6, 7});
    StepperDriver stepperDriver2({2, 3, 4});
    TranslationalJoint<StepperDriver> translationalJoint;
    translationalJoint.setPosition(50.0);
    translationalJoint.setMovementPerStep(0.01);
    RotationalJoint<StepperDriver> rotationalJoint;
    rotationalJoint.setPosition(PI/2);
    rotationalJoint.setMovementPerStep(PI/180 * 0.01);
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
    Point2D centrePoint(-0, 580);
    RotationTrace trace(startPoint, endPoint, centrePoint);
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
    RobotIO robotBuilder("defaultRobot.xml");
    robotBuilder.build();


    //    robotBuilder.getRobotPointer();
    Robot robot = *robotBuilder.getRobotPointer();
    robot.setPosition(startPoint);

    robot.getJointController()->resolveJoint(BaseJoint::Rotational)->setPosition(PI/2);
    robot.getJointController()->resolveJoint(BaseJoint::Translational)->setPosition(255);

    RotationTraceCalculator rotationTraceCalculator(&robot);
    rotationTraceCalculator.setWriteLog(true);
    rotationTraceCalculator.calculateTrace(trace);

    SequenceVector sequenceVector = robot.getJointController()->getSequenceVector();
    /*
      robot.getJointController()->moveSteps("OUT",2);
    sequenceVector.getSequenceVector()[2].displaySequence();
    StateSequence stateSequence = sequenceVector.getSequenceVector()[2];
    for (int i = 0;
         i < 79998;
         ++i) {
        sequenceVector.appendStateSequence(stateSequence, false);
    }
    //LOG_DEBUG("yaya" << sequenceVector.getSequenceVector().size());
    robot.getJointController()->setSequenceVector(sequenceVector);
    //    robot.getJointController()->setSequenceVector(sequenceVector);
    */
    robot.getJointController()->getSequenceVector().exportValue();
    SequenceVector vector = robot.getJointController()->getSequenceVector();
    LOG_DEBUG("Number of steps before condense: " << vector.numberOfSteps());
    //    vector.condenseVector();
    //    LOG_DEBUG("Number of steps after condense: " << vector.numberOfSteps());
    try {
        LOG_DEBUG("here!!");
        robot.actuate();
    } catch(std::runtime_error) {
        LOG_INFO("Could not find sizzle");
    }

    /// Tests
    TS_ASSERT_EQUALS(robot.getJointController()->getSequenceVector().numberOfSequences(),
                     104429);
    TS_ASSERT_EQUALS(robot.getJointController()->getSequenceVector().numberOfSteps(),
                     2992669);

    /// testing condensing on this big vector
    sequenceVector.normalise();
    long totalNumberOfReps = 0;
    for (auto& sequence : sequenceVector.getSequenceVector()) {
      totalNumberOfReps += sequence.getNumberOfRepetitions();
    }

    double unOptimisedAverageRep = static_cast<double>(totalNumberOfReps)/
        sequenceVector.numberOfSequences();

    /// test efficiency
    TS_ASSERT_DELTA(14.3216, unOptimisedAverageRep, 0.001);

    /// test if condensing works
    TS_ASSERT(sequenceVector.condenseVector());

    int numberOfEmptySequences = 0;
    totalNumberOfReps = 0;
    for (auto& sequence : sequenceVector.getSequenceVector()) {
      if (sequence.getNumberOfRepetitions() == 0)
        numberOfEmptySequences += 1;
      totalNumberOfReps += sequence.getNumberOfRepetitions();
    }

    TS_ASSERT_EQUALS(numberOfEmptySequences, 47537);
    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 129580);

    double optimisedAverageRep = static_cast<float>(totalNumberOfReps)/
        (sequenceVector.numberOfSequences() - numberOfEmptySequences);
    /// check if the condensing improves information densitity
    TS_ASSERT(optimisedAverageRep > unOptimisedAverageRep);
    /// Check the actual number
    TS_ASSERT_DELTA(optimisedAverageRep, 18.2293, 0.001);

    /**
     * does not work
    for (auto& i : std::vector<int>({5707,6106,6084, 5714})) {
      TS_ASSERT_EQUALS(
          sequenceVector.getSequenceVector()[i].getIntegerSequence(), std::vector<int>({64, 192}));
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
