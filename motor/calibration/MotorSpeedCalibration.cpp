// Copyright Ruud Cools [2017]
#include <macroHeader.h>
#include <MotorSpeedCalibration.h>
#include <Robot.h>
#include <BaseJoint.h>
#include <BaseMotor.h>
#include <ManualSpeedController.h>
#include <SpeedController.h>
#include <thread>
#include <chrono>
#include <StepperDriver.h>


MotorSpeedCalibration::MotorSpeedCalibration(const std::shared_ptr<BaseJoint>& i_joint,
					     const std::shared_ptr<Robot>& i_robot,
					     const BaseCalibration::Type i_type)
  : BaseCalibration(i_joint, i_robot, i_type),
    m_searchRange(0.3),
    m_stepSize(1),
    m_delay(1500),
    m_prevController(i_robot->getSpeedController()) {
  m_robot->setSpeedController(std::make_shared<ManualSpeedController>());
}


MotorSpeedCalibration::~MotorSpeedCalibration() {
  m_robot->setSpeedController(m_prevController);
}


/// indicate if execution is possible -> all requirements are met
bool MotorSpeedCalibration::isReady() const {
  // currently it only works with drivers, maybe in the future :)
  return m_joint->getMotor()->getType() == BaseMotor::StepperDriver;
}


std::string MotorSpeedCalibration::getJointDirection() const {
  std::vector<traceType> range = m_joint->getRange();
  traceType position = m_joint->getPosition();
  bool isInner = std::abs(position - range[0]) < std::abs(position - range[1]);
  return BaseCalibration::getJointDirection(isInner);
}


bool MotorSpeedCalibration::testSpeed(const std::vector<int>& i_speedVector,
				      const int& i_stepsPerSpeed) {
  // get the speed controller and set the speed vector
  std::static_pointer_cast<ManualSpeedController>
    (m_robot->getSpeedController())->setSpeedVector(i_speedVector);
  for (unsigned int i = 0; i < i_speedVector.size(); ++i) {
    std::string jointDirection = getJointDirection();
    m_robot->prepareSteps(jointDirection, i_stepsPerSpeed);
    m_robot->actuate();
    std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
  }
  return m_finished;
}


void MotorSpeedCalibration::prepareParameterVector(std::vector<int>::const_iterator* o_begin,
						   std::vector<int>::const_iterator* o_end,
						   const int&(StepperDriver::*i_function)()const) {

  std::shared_ptr<StepperDriver> driver =
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
  int testAcceleration = ((*driver).*i_function)() * (1 - m_searchRange);
  int end = driver->getIncremental() * (1 + m_searchRange);
  // create a vector in which we describe all the accelerations
  std::vector<int> accelerations;
  while (testAcceleration < end) {
    accelerations.push_back(testAcceleration);
    testAcceleration += m_stepSize;
  } 

  // start with the tests untill done is true
  // done is set by the testSpeed method
  // which returns m_isFinished which will be set by the user via the gui
  *o_begin = accelerations.begin();
  *o_end = accelerations.end();
}
