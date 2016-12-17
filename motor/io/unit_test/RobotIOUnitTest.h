// Copyright [2016] Ruud Cools
#ifndef MOTOR_IO_UNIT_TEST_ROBOTIOUNITTEST_H_
#define MOTOR_IO_UNIT_TEST_ROBOTIOUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <JointControllerIO.h>
#include <RobotIO.h>
#include <JointController.h>
#include <SpeedController.h>

class RobotIOUnitTest : public CxxTest::TestSuite {
 public:

  void testReadFromXMLFile() {
    RobotIO robotIO("defaultRobot.xml");
    robotIO.build();
    Robot::RobotPointer robot = robotIO.getRobotPointer();
    TS_ASSERT_EQUALS(robot->getJointController()->getNumberOfJoints(), 2);
    TS_ASSERT_EQUALS(robot->getSpeedController()->getType(), SpeedController::Relative);
    TS_ASSERT_EQUALS(robot->getSpeedController()->getType(), SpeedController::Relative);
    robotIO.store("lala.xml");
  }
};

#endif  // MOTOR_IO_UNIT_TEST_ROBOTIOUNITTEST_H_
