// Copyright [2015] Ruud Cools
#ifndef MOTOR_IO_UNITTEST_JOINTCONTROLLERIOUNITTEST_H_
#define MOTOR_IO_UNITTEST_JOINTCONTROLLERIOUNITTEST_H_

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

    StepperDriver::DriverPointer driver = std::static_pointer_cast<StepperDriver>(jointpointer->getMotor());
    
    TS_ASSERT_EQUALS(driver->getPullIn(),124);
    TS_ASSERT_EQUALS(driver->getPullOut(),123);
    TS_ASSERT_EQUALS(driver->getMaxSpeed(),12345);
    TS_ASSERT_EQUALS(driver->getCurrentPinState().getPinVector(),
		     std::vector<int>({5,6,7}));
    TS_ASSERT(driver->getHoldMotor());
      
    DirectionConversionMap map = {{"CCW","CCW"},{"CW","CW"}};
    TS_ASSERT_EQUALS(jointpointer->getDirectionConversionMap(),
		     map);
    TS_ASSERT_EQUALS(jointpointer->getRange(), std::vector<traceType>({0, 185 * PI/180}));
    TS_ASSERT_EQUALS(jointpointer->getPosition(), PI/2);
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

#endif  // MOTOR_IO_UNITTEST_JOINTCONTROLLERIOUNITTEST_H_
