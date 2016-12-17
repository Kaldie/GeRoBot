// Copyright [2015] Ruud Cools
#ifndef MOTOR_UNIT_TEST_JOINTCONTROLLERIOUNITTEST_H_
#define MOTOR_UNIT_TEST_JOINTCONTROLLERIOUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <StepperDriver.h>
#include <PinState.h>
#include <JointControllerIO.h>
#include <JointController.h>
#include <RobotIO.h>
#include <BaseJoint.h>

class JointControllerIOUnitTest : public CxxTest::TestSuite {
 public:

  void testBuild() {
    RobotIO robotIO("test_robot.xml");
    JointControllerIO jointControllerIO
      (robotIO.getNodeFromPath("ROBOT/JOINTCONTROLLER"));
    jointControllerIO.build();
    JointController::JointControllerPointer jointController = jointControllerIO.getJointController();
  }

  void testBuildValues() {
    RobotIO robotIO("test_robot.xml");
    JointControllerIO jointControllerIO
      (robotIO.getNodeFromPath("ROBOT/JOINTCONTROLLER"));
    jointControllerIO.build();
    JointController::JointControllerPointer jointController = jointControllerIO.getJointController();
    // test if the joint pointer vector is as expected
    TS_ASSERT_EQUALS(jointController->getJointPointerVector().size(), 2);
    //Test if the rotation joint has a child, the translational joint!
    TS_ASSERT_EQUALS(jointController->resolveJoint(BaseJoint::Rotational)->getChild().lock(),
                     jointController->resolveJoint(BaseJoint::Translational));
    TS_ASSERT_EQUALS(jointController->resolveJoint(BaseJoint::Rotational)->getParent().lock(),
                     nullptr);
    // Test if the translational joint has no child
    TS_ASSERT_EQUALS(jointController->resolveJoint(BaseJoint::Translational)->getChild().lock(),
                     nullptr);
    TS_ASSERT_EQUALS(jointController->resolveJoint(BaseJoint::Translational)->getParent().lock(),
                     jointController->resolveJoint(BaseJoint::Rotational));
    TS_ASSERT_EQUALS(jointController->getSequenceVector().getSequenceVector().size(), 1);
    // test if there is one state sequence is known
    TS_ASSERT_EQUALS
      (jointController->getSequenceVector().getSequenceVector()[0]
       .getPinStateVector().size(),1);
    // test if the usual pins are active
    TS_ASSERT_EQUALS
      (jointController->getSequenceVector().getSequenceVector()[0]
       .getIntegerSequence(), std::vector<int>({252}));
  }


  void testActuator() {
    RobotIO robotIO("test_robot.xml");
    JointControllerIO jointControllerIO
      (robotIO.getNodeFromPath("ROBOT/JOINTCONTROLLER"));
    jointControllerIO.build();
    JointController::JointControllerPointer controller = jointControllerIO.getJointController();
    ArduinoMotorDriver driver =  controller->getActuator();
    TS_ASSERT_EQUALS(driver.getSerialRegularExpresion(),"RuudWorld");
  }


  void testEasyUpdate() {
    // check if it is possible to update without inducing changes
    RobotIO robotIO("test_robot.xml");
    JointControllerIO jointControllerIO
      (robotIO.getNodeFromPath(robotIO.getNodeFromPath("ROBOT"),"JOINTCONTROLLER"));
    jointControllerIO.build();
    JointController::JointControllerPointer controller = jointControllerIO.getJointController();
    jointControllerIO.update(controller);
  }


  void testUpdateDependancy() {
    // check if it is possible to change the dependancy and
    // see that reflected back into the document
    RobotIO robotIO("test_robot.xml");
    JointControllerIO jointControllerIO
      (robotIO.getNodeFromPath("ROBOT/JOINTCONTROLLER"));
    jointControllerIO.build();
    JointController::JointControllerPointer controller = jointControllerIO.getJointController();
    controller->resolveJoint(BaseJoint::Translational)->setChild
      (controller->resolveJoint(BaseJoint::Rotational));
    jointControllerIO.update(controller);
    robotIO.store("write.xml");
    // loading the new xml document in a new RobotIO
    RobotIO robotIO2("write.xml");
    JointControllerIO jointControllerIO2
      (robotIO2.getNodeFromPath("ROBOT/JOINTCONTROLLER"));
    jointControllerIO2.build();
    controller = jointControllerIO2.getJointController();
    for (const auto& joint : controller->getJointPointerVector()) {
      TS_ASSERT(joint->getChild().lock());
      if (joint->getChild().lock() != controller->getJointPointerVector()[0] and
          joint->getChild().lock() != controller->getJointPointerVector()[1])
	LOG_ERROR("Should be able to find the child in the joint vector!");
    }
    std::remove("write.xml");
  }


  void testRemoveAJoint() {
    // check if it is possible to change the dependancy and
    // see that reflected back into the document
    RobotIO robotIO("test_robot.xml");
    JointControllerIO jointControllerIO
      (robotIO.getNodeFromPath("ROBOT/JOINTCONTROLLER"));
    jointControllerIO.build();
    JointController::JointControllerPointer controller = jointControllerIO.getJointController();
    JointController::JointPointerVector vector = controller->getJointPointerVector();
    vector.pop_back();
    controller->setJointPointerVector(vector);
    jointControllerIO.update(controller);
    robotIO.store("write2.xml");
    // loading the new xml document in a new RobotIO
    RobotIO robotIO2("write2.xml");
    JointControllerIO jointControllerIO2
      (robotIO2.getNodeFromPath("ROBOT/JOINTCONTROLLER"));
    jointControllerIO2.build();
    controller = jointControllerIO2.getJointController();
    TS_ASSERT_EQUALS(controller->getJointPointerVector().size(), 1);
    std::remove("write2.xml");
  } 
};


#endif  // MOTOR_UNIT_TEST_JOINTCONTROLLERIOUNITTEST_H_
