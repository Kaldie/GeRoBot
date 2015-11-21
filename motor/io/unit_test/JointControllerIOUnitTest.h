// Copyright [2015] Ruud Cools

#ifndef MOTOR_UNIT_TEST_JOINTCONTROLLERIOUNITTEST_H_
#define MOTOR_UNIT_TEST_JOINTCONTROLLERIOUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <JointControllerIO.h>
#include <RobotIO.h>

class JointControllerIOUnitTest : public CxxTest::TestSuite {
 public:
  JointControllerIO jointControllerIO;

  void setUp() {
    RobotIO robotIO("test_robot.xml");
    jointControllerIO.setNode(robotIO.getNode("JOINTCONTROLLER"););
  }

  void testBuild(void) {
    JointController jointController2;
    TS_ASSERT_EQUALS(1,1);
  }

};


#endif  // MOTOR_UNIT_TEST_JOINTCONTROLLERIOUNITTEST_H_
