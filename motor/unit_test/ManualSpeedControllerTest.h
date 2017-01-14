// Copyright [2017] Ruud Cools

#ifndef MOTOR_UNIT_TEST_MANUALSPEEDCONTROLLERTEST_H_
#define MOTOR_UNIT_TEST_MANUALSPEEDCONTROLLERTEST_H_

#include <cxxtest/TestSuite.h>
#include <ManualSpeedController.h>
#include <MovementRegistrator.h>

class ManualSpeedControllerTest : public CxxTest::TestSuite {
 public:
  void testConstruction() {
    // test if its possible to create a speed controller and that teh movement registrator is also constructor
    // and its shared pointer valid
    ManualSpeedController manualSpeedController(75);
    std::shared_ptr<MovementRegistrator> registrator(manualSpeedController.getMovementRegistrator());
    TS_ASSERT_EQUALS(static_cast<bool>(registrator), true);
  }


  void testActualSpeedControlling() {
    Robot::RobotPointer robot = RobotIO("defaultRobot.xml").buildRobot();
    robot->setPosition(Point2D(50,0));
    // creation of the speedcontroller
    SpeedController::SpeedControllerPointer speedController =
      std::make_shared<ManualSpeedController>(75);
    robot->setSpeedController(speedController);
    // check if the new registrator is the first registrator on txhe robot
    TS_ASSERT_EQUALS(speedController->getMovementRegistrator(),
		     robot->getMovementRegistrators().begin()->lock());
    BaseTraceCalculator traceCalculator(robot.get());
    traceCalculator.calculateTrace(Trace(Point2D(50,0), Point2D(50,10)));

    /// check if the sequence vector is the expected lenght
    TS_ASSERT_EQUALS(robot->getJointController()->getSequenceVector().numberOfSequences(),
		     6);
    // known speeds
    std::vector<int> speeds({75, 75, 75, 75, 75, 75});
    // compare speeds to known speeds
    int i = 0;
    for (const auto x : robot->getJointController()->getSequenceVector()) {
      TS_ASSERT_EQUALS(speeds[i], x.getSpeed());
      LOG_DEBUG(i);
      ++i;
    }
  }

  void testManualSpeeds() {
    Robot::RobotPointer robot = RobotIO("defaultRobot.xml").buildRobot();
    robot->setPosition(Point2D(50,0));
    // creation of the speedcontroller
    SpeedController::SpeedControllerPointer speedController =
      std::make_shared<ManualSpeedController>(12);
    std::shared_ptr<ManualSpeedController> manualSpeedController =
      std::static_pointer_cast<ManualSpeedController>(speedController);
    std::vector<int> speedVector({99,100,101, 102,103,104,105,106});
    manualSpeedController->setSpeedVector(speedVector);
    robot->setSpeedController(speedController);
    // check if the new registrator is the first registrator on txhe robot
    TS_ASSERT_EQUALS(speedController->getMovementRegistrator(),
		     robot->getMovementRegistrators().begin()->lock());
    for (int i=0; i < 17; ++i) {
      robot->prepareSteps("CCW",1);
    }
    std::vector<int> speeds({99,99,99,100,101,102,103,104,105,106,99,100,101,102,103,104,105,106,99});
    // compare speeds to known speeds
    int i = 0;
    for (const auto x : robot->getJointController()->getSequenceVector()) {
      TS_ASSERT_EQUALS(speeds[i], x.getSpeed());
      LOG_DEBUG(i);
      ++i;
    }
  }
};

#endif  // MOTOR_UNIT_TEST_MANUALSPEEDCONTROLLERTEST_H_
