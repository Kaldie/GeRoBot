// Copyright [2015] Ruud Cools

#ifndef MOTOR_UNITTEST_CONSTANTSPEEDCONTROLLERUNITTEST_H_
#define MOTOR_UNITTEST_CONSTANTSPEEDCONTROLLERUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <ConstantSpeedController.h>
#include <BaseJoint.h>
#include <RotationalJoint.h>
#include <TranslationalJoint.h>
#include <JointController.h>
#include <StepperDriver.h>
#include <Trace.h>
#include <Robot.h>
#include <RobotIO.h>
#include <BaseTraceCalculator.h>

class ConstantSpeedControllerUnitTest : public CxxTest::TestSuite {
 public:

  void testConstructor() {
    ConstantSpeedController a = ConstantSpeedController();
    ConstantSpeedController b = ConstantSpeedController(10);
    TS_ASSERT(a.getRobotSpeed() == 0);
    TS_ASSERT(b.getRobotSpeed() == 10);
    TS_ASSERT(b.getMovementRegistrator());
  }


  void testActualSpeedControlling() {
      // In this test we actually will check the speed of the robot at the state sequence level
      Robot::RobotPointer robot = RobotIO("defaultRobot.xml").buildRobot();
      robot->setPosition(Point2D(50,0));
      // creation of the speedcontroller
      SpeedController::SpeedControllerPointer speedController =
          std::make_shared<ConstantSpeedController>(0.1);
      robot->setSpeedController(speedController);

      // check if the new registrator is the first registrator on the robot
      TS_ASSERT_EQUALS(speedController->getMovementRegistrator(),
		       robot->getMovementRegistrators().begin()->lock());

      // calculate a simple trace
      BaseTraceCalculator traceCalculator(robot.get());
      traceCalculator.calculateTrace(Trace(Point2D(50,0), Point2D(50,10)));

      /// check if the sequence vector is the expected lenght
      TS_ASSERT_EQUALS(robot->getJointController()->getSequenceVector().numberOfSequences(),
		       5);
      // known speeds
      std::vector<int> speeds({69,69,69,10,10});
      // compare speeds to known speeds
      int i = 0;
      for (const auto x : robot->getJointController()->getSequenceVector()) {
	TS_ASSERT_EQUALS(speeds[i], x.getSpeed());
	++i;
      }
  }


  void testMaxSpeed() {
    // the idea of this test is to test if the controller does not over power the motors
      Robot::RobotPointer robot = RobotIO("defaultRobot.xml").buildRobot();
      robot->setPosition(Point2D(50,0));
      // creation of the speedcontroller
      SpeedController::SpeedControllerPointer speedController =
          std::make_shared<ConstantSpeedController>(100);
      robot->setSpeedController(speedController);
      robot->prepareSteps("OUT", 1);
      std::shared_ptr<StepperDriver> driver = std::static_pointer_cast<StepperDriver>
	(robot->getJointController()->resolveJoint(BaseJoint::Translational)->getMotor());
      int speed;
      speedController->adviseSpeed(&speed);
      TS_ASSERT_EQUALS(speed,driver->getPullIn());

      driver->setPullIn(600);
      speedController->adviseSpeed(&speed);
      TS_ASSERT_EQUALS(speed,driver->getPullIn());
      
      speedController->setRobotSpeed(2.9);
      speedController->adviseSpeed(&speed);
      TS_ASSERT_EQUALS(speed,290);

      robot->prepareSteps("CCW", 2);
      speedController->adviseSpeed(&speed);
      TS_ASSERT_EQUALS(speed, 300);
  }
};
#endif  // MOTOR_UNITTEST_CONSTANTSPEEDCONTROLLERUNITTEST_H_
