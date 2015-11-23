// Copyright [2015] Ruud Cools
#ifndef MOTOR_UNIT_TEST_JOINTIOUNITTEST_H_
#define MOTOR_UNIT_TEST_JOINTCONTROLLERIOUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <StepperDriver.h>
#include <PinState.h>
#include <JointIO.h>
#include <RobotIO.h>
#include <BaseJoint.h>

class JointIOUnitTest : public CxxTest::TestSuite {
 public:
  void testBuild() {
    RobotIO robotIO("test_robot.xml");
    JointIO jointIO(robotIO.getNodeFromPath("ROBOT/JOINTCONTROLLER/JOINT"));
    jointIO.build();
  }

  void testBuildValues() { 
    RobotIO robotIO("test_robot.xml");
    JointIO jointIO(robotIO.getNodeFromPath("ROBOT/JOINTCONTROLLER/JOINT"));
    jointIO.build();
    BaseJoint::JointPointer jointpointer(jointIO.getJointPointer());
    // test StepperDriver
    StepperDriver* driver = static_cast<StepperDriver*>(jointpointer->getMotor());
    TS_ASSERT_EQUALS(driver->getPullIn(),200);
    TS_ASSERT_EQUALS(driver->getPullOut(),200);
    TS_ASSERT_EQUALS(driver->getMaxSpeed(),500);
    TS_ASSERT_EQUALS(driver->getCurrentPinState().getPinVector(),
		     std::vector<int>({5,6,7}));
    TS_ASSERT(driver->getHoldMotor());
    DirectionConversionMap map = {{"CCW","CCW"},{"CW","CW"}};
    TS_ASSERT_EQUALS(jointpointer->getDirectionConversionMap(),
		     map);
    TS_ASSERT_EQUALS(jointpointer->getRange(), std::vector<traceType>({0,185}));
    TS_ASSERT_EQUALS(jointpointer->getPosition(), 90.0);
    TS_ASSERT_EQUALS(jointpointer->getMovementType(), BaseJoint::Rotational);
    TS_ASSERT_DELTA(jointpointer->getMovementPerStep(),0.0016,0.01);
  }

  void testUpdate() {
    RobotIO robotIO("test_robot.xml");
    JointIO jointIO(robotIO.getNodeFromPath("ROBOT/JOINTCONTROLLER/JOINT"));
    jointIO.build();
    BaseJoint::JointPointer jointpointer(jointIO.getJointPointer());
  }
  
};

#endif  // MOTOR_UNIT_TEST_JOINTIOUNITTEST_H_
