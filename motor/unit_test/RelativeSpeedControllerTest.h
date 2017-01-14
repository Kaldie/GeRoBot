// Copyright [2017] Ruud Cools

#ifndef MOTOR_UNIT_TEST_RELATIVESPEEDCONTROLLERTEST_H_
#define MOTOR_UNIT_TEST_RELATIVESPEEDCONTROLLERTEST_H_

#include <cxxtest/TestSuite.h>
#include <RelativeSpeedController.h>
#include <MovementRegistrator.h>

class RelativeSpeedControllerTest : public CxxTest::TestSuite {
 public:
  void testConstruction() {
    // test if its possible to create a speed controller and that teh movement registrator is also constructor
    // and its shared pointer valid
    RelativeSpeedController relativeSpeedController(75);
    std::shared_ptr<MovementRegistrator> registrator(relativeSpeedController.getMovementRegistrator());
    TS_ASSERT_EQUALS(static_cast<bool>(registrator), true);
  }


  void testActualSpeedControlling() {
      Robot::RobotPointer robot = RobotIO("defaultRobot.xml").buildRobot();
      robot->setPosition(Point2D(50,0));
      // creation of the speedcontroller
      SpeedController::SpeedControllerPointer speedController =
          std::make_shared<RelativeSpeedController>(75);
      robot->setSpeedController(speedController);
      // check if the new registrator is the first registrator on txhe robot
      TS_ASSERT_EQUALS(speedController->getMovementRegistrator(),
		       robot->getMovementRegistrators().begin()->lock())
      BaseTraceCalculator traceCalculator(robot.get());
      traceCalculator.calculateTrace(Trace(Point2D(50,0), Point2D(50,10)));

      /// check if the sequence vector is the expected lenght
      TS_ASSERT_EQUALS(robot->getJointController()->getSequenceVector().numberOfSequences(),
		       5);
      // known speeds
      std::vector<int> speeds({225,225,225,225,225});
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
          std::make_shared<RelativeSpeedController>(100);
      robot->setSpeedController(speedController);
      LOG_DEBUG("robot->prepareSteps(OUT, 1);");
      robot->prepareSteps("OUT", 1);
      std::shared_ptr<StepperDriver> driver = std::static_pointer_cast<StepperDriver>
	(robot->getJointController()->resolveJoint(BaseJoint::Translational)->getMotor());
      LOG_DEBUG("lalal");
      int speed;
      speedController->adviseSpeed(&speed);
      TS_ASSERT_EQUALS(speed,driver->getPullIn());

      driver->setPullIn(600);
      speedController->adviseSpeed(&speed);
      TS_ASSERT_EQUALS(speed,driver->getPullIn());
      
      speedController->setRobotSpeed(90);
      speedController->adviseSpeed(&speed);
      TS_ASSERT_EQUALS(speed,540);

      robot->prepareSteps("CCW", 2);
      speedController->adviseSpeed(&speed);
      TS_ASSERT_EQUALS(speed, 270);
  }

  
};

#endif  // MOTOR_UNIT_TEST_RELATIVESPEEDCONTROLLERTEST_H_
