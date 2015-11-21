// Copyright [2015] Ruud Cools

#ifndef MOTOR_UNIT_TEST_JOINTCONTROLLERIOUNITTEST_H_
#define MOTOR_UNIT_TEST_JOINTCONTROLLERIOUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <JointControllerIO.h>
#include <RobotIO.h>

class JointControllerIOUnitTest : public CxxTest::TestSuite {
 public:

  void testBuild() {
    RobotIO robotIO("test_robot.xml");
    JointControllerIO jointControllerIO
      (robotIO.getNodeFromPath(robotIO.getNodeFromPath("ROBOT"),"JOINTCONTROLLER"));
    jointControllerIO.build();
    JointController jointController = jointControllerIO.getJointController();
    //Test if the rotation joint has a child, the translational joint!
    TS_ASSERT_EQUALS(jointController.resolveJoint(BaseJoint::Rotational)->getChild(),
                     jointController.resolveJoint(BaseJoint::Translational));
    // Test if the translational joint has no child
    TS_ASSERT_EQUALS(jointController.resolveJoint(BaseJoint::Translational)->getChild(),
                     nullptr);
  }

  void testBuildValues() {

  }

};


#endif  // MOTOR_UNIT_TEST_JOINTCONTROLLERIOUNITTEST_H_
