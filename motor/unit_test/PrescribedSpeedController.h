// Copyright [2015] Ruud Cools

#ifndef MOTOR_UNITTEST_PRESCRIBEDSPEEDCONTROLLERUNITTEST_H_
#define MOTOR_UNITTEST_PRESCRIBEDSPEEDCONTROLLERUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <PrescribedSpeedController.h>
#include <BaseJoint.h>

class PrescribedSpeedControllerUnitTest : public CxxTest::TestSuite {
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


  void testCreation(void) {
    // Does it work...at all
    SpeedController speedController;
    TS_ASSERT_EQUALS(speedController.getRobotSpeed(), 0);
    TS_ASSERT_EQUALS(speedController.getMotorFrequency(), 0);
    TS_ASSERT_EQUALS(speedController.getStepMap().size(), 0);
    /// Fully fledged
    SpeedController speedController2(100);
    TS_ASSERT_EQUALS(speedController2.getRobotSpeed(), 100);
  }


  void testNotifyStep1(void) {
    // Make the speed controller react when
    // it is told the robot makes a step
    SpeedController speedController(300);
    speedController.notifyStep(m_rotationalJoint,1);
    int speed;
    TS_ASSERT(speedController.adviseSpeed(&speed));
    TS_ASSERT_EQUALS(speed, 200);
  }


  void testAcceleration(void) {
    // Given a speed which cannot be attained
    // Check if the motor is accelerated to max
    SpeedController speedController(300);
    int speed = 0;
    int numberOfSteps = 11;
    for (int i = 0;
         i < numberOfSteps;
         ++i) {
      speedController.notifyStep(m_rotationalJoint, 1);
      TS_ASSERT(speedController.adviseSpeed(&speed));
      TS_ASSERT_EQUALS(speed, 200 + i * 30);
      speedController.acknowledgeSpeed(speed);
    }
  }


  void testBreak() {
    m_rotationalJoint->getMotor()->setSpeed(500);
    m_translationalJoint->getMotor()->setSpeed(500);
    SpeedController speedController(0);
    speedController.setAchievedSpeeds({30,30,30,30,30});
    int speed;
    int numberOfSteps = 3;
    traceType prevSpeed = std::numeric_limits<traceType>::max();
    for (int i = 1;
         i < numberOfSteps+1;
         ++i) {
      speedController.notifyStep(m_rotationalJoint, 1);
      TS_ASSERT(speedController.adviseSpeed(&speed));
      TS_ASSERT_EQUALS(speed, 500 - i * 30);
      LOG_DEBUG("Current robot speed: " << speedController.estimateCurrentSpeed());
      LOG_DEBUG("Current motor speed: " << speed);
      if (prevSpeed != 0) {
        // dont check the first setting!
        TS_ASSERT(prevSpeed > speedController.estimateCurrentSpeed());
      }
      prevSpeed = speedController.estimateCurrentSpeed();
      speedController.acknowledgeSpeed(speed);
    }
  }


  void testAccelerationToRobotSpeed() {
    // testing acceleration given a speed of the robot
    Point2D aPosition(0, 50.0);
    SpeedController speedController(10);
    int speed;
    int numberOfSteps = 11;
    m_translationalJoint->setMovementPerStep(0.025);
    for (int i = 1;
         i < numberOfSteps;
         ++i) {
      //      speedController.notifyStep(m_rotationalJoint, 5);
      speedController.notifyStep(m_translationalJoint, 4);
      speedController.adviseSpeed(&speed);
      TS_ASSERT(speed <= 400);
      TS_ASSERT(speedController.estimateCurrentSpeed() <= 10);
      LOG_DEBUG("Current robot speed: " << speedController.estimateCurrentSpeed());
      LOG_DEBUG("Current motor speed: " << speed);
      speedController.acknowledgeSpeed(speed);
    }
  }


  void testSpeedWhileIdle() {
    int speed = 0;

    SpeedController speedController(150);
    // both joints get a step
    speedController.notifyStep(m_translationalJoint,1);
    speedController.notifyStep(m_rotationalJoint,1);
    TS_ASSERT(speedController.adviseSpeed(&speed));
    TS_ASSERT_EQUALS(speed, 200);
    speedController.acknowledgeSpeed(speed);
    // Only the rotation joint gets a step
    speedController.notifyStep(m_rotationalJoint,2);
    TS_ASSERT(speedController.adviseSpeed(&speed));
    speedController.acknowledgeSpeed(speed);
    TS_ASSERT_EQUALS(speed,230);
    // both joints get a step, test wheter the translation joint speed is reduced
    speedController.notifyStep(m_translationalJoint, 1);
    speedController.notifyStep(m_rotationalJoint,1);
    TS_ASSERT(speedController.adviseSpeed(&speed));
    speedController.acknowledgeSpeed(speed);
    TS_ASSERT_EQUALS(m_translationalJoint->getMotor()->getSpeed(), 84);
    LOG_DEBUG(" Idle speed: " << m_translationalJoint->getMotor()->getSpeed());

    // Test it once more with more steps!
    speedController.notifyStep(m_rotationalJoint,3);
    TS_ASSERT(speedController.adviseSpeed(&speed));
    speedController.acknowledgeSpeed(speed);
    TS_ASSERT_EQUALS(speed,260);
    speedController.notifyStep(m_translationalJoint, 1);
    TS_ASSERT(speedController.adviseSpeed(&speed));
    TS_ASSERT_EQUALS(speed,200);
    speedController.acknowledgeSpeed(speed);
    TS_ASSERT_EQUALS(m_translationalJoint->getMotor()->getSpeed(), 76);
  }


  void testDrawLine() {
    m_translationalJoint->setMovementPerStep(0.25);
    SpeedController speedController(10);
    Point2D startPoint(0, 50.0);
    int speed;
    //    int prevSpeed = std::num
    for (int numberOfSteps = 100;
         numberOfSteps > 0;
         --numberOfSteps) {
      m_rotationalJoint->predictSteps(&startPoint, "CCW", 5);
      speedController.notifyStep(m_rotationalJoint, 5);
      m_translationalJoint->predictSteps(&startPoint, "OUT", 5);
      speedController.notifyStep(m_translationalJoint, 5);
      speedController.adviseSpeed(&speed);
      LOG_DEBUG("Advertised speed: " << speed);
      TS_ASSERT(speed >= 23 && speed <= 33);
      speedController.acknowledgeSpeed(speed);
    }
  }

};
#endif // MOTOR_UNITTEST_PRESCRIBEDSPEEDCONTROLLERUNITTEST_H_
