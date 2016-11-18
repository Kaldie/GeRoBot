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

class ConstantSpeedControllerUnitTest : public CxxTest::TestSuite {
 public:
  BaseJoint::JointPointer m_rotationalJoint;
  BaseJoint::JointPointer m_translationalJoint;

  void setUp() {
    m_rotationalJoint = std::make_shared<RotationalJoint<StepperDriver>>();
    m_rotationalJoint->setPosition(90.0 * PI/180);
    m_rotationalJoint->setMovementPerStep(0.001);
    m_rotationalJoint->setRange(std::vector<traceType>({0, PI}));
    m_rotationalJoint->setDirectionConversionMap
      (DirectionConversionMap({{"CCW","CCW"},{"CW","CW"}}));

    m_translationalJoint = std::make_shared<TranslationalJoint<StepperDriver>>();
    m_translationalJoint->setPosition(50);
    m_translationalJoint->setMovementPerStep(0.25);
    m_translationalJoint->setRange(std::vector<traceType>({40, 1800}));
    m_translationalJoint->setDirectionConversionMap
      (DirectionConversionMap({{"IN","CCW"},{"OUT","CW"}}));
    m_rotationalJoint->setChild(m_translationalJoint);
    m_translationalJoint->setParent(m_rotationalJoint);
  }

  void testConstructor() {
    ConstantSpeedController a = ConstantSpeedController();
    ConstantSpeedController b = ConstantSpeedController(10);
    TS_ASSERT(a.getRobotSpeed() == 0);
    TS_ASSERT(b.getRobotSpeed() == 10);
  }

  void testPrepareSpeedController() {
    JointController controller = JointController();
    controller.addJoint(m_rotationalJoint);
    controller.addJoint(m_translationalJoint);
    Trace trace = Trace(Point2D(1,1), Point2D(10,10));
    ConstantSpeedController b = ConstantSpeedController(40);
    b.prepareSpeedController(trace, controller);
    int speed;
    // test speed when the robot speed/ linear joint is limiting
    b.adviseSpeed(&speed);
    TS_ASSERT_EQUALS(speed, 160);

    // test speed when the max motor speed is limiting
    b.setRobotSpeed(1000);
    b.prepareSpeedController(trace, controller);
    b.adviseSpeed(&speed);
    TS_ASSERT_EQUALS(speed, m_translationalJoint->getMotor()->getMaximumSpeed());

    // test that increasing the motor speed increase the constant speed
    StepperDriver* driver = static_cast<StepperDriver*>(m_translationalJoint->getMotor());
    driver->setPullIn(300);
    b.prepareSpeedController(trace, controller);
    b.adviseSpeed(&speed);
    TS_ASSERT_EQUALS(speed, 300);

    /* test that increasing the motor which is not responsible for the most part of the movement
       is not responsible for setting the max speed */
    driver = static_cast<StepperDriver*>(m_rotationalJoint->getMotor());
    driver->setPullIn(500);
    b.prepareSpeedController(trace, controller);
    b.adviseSpeed(&speed);
    TS_ASSERT_EQUALS(speed, 300);
  }


  void testPrepareSpeedControllerForRotation() {
    JointController controller = JointController();
    controller.addJoint(m_rotationalJoint);
    controller.addJoint(m_translationalJoint);
    Trace trace = Trace(Point2D(10,0), Point2D(10,10));

    // test speed when the robot speed/ linear joint is limiting
    StepperDriver* driver = static_cast<StepperDriver*>(m_rotationalJoint->getMotor());
    driver->setPullIn(400);

    ConstantSpeedController b = ConstantSpeedController(1000);
    b.prepareSpeedController(trace, controller);
    int speed;
    // test speed when the robot speed is limiting
    b.adviseSpeed(&speed);
    TS_ASSERT_EQUALS(speed, 400);
  }

};
#endif  // MOTOR_UNITTEST_CONSTANTSPEEDCONTROLLERUNITTEST_H_
