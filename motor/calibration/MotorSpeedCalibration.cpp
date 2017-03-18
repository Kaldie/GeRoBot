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
#include <JointController.h>
#include <CalibrationOutput.h>

MotorSpeedCalibration::MotorSpeedCalibration(const std::shared_ptr<BaseJoint>& i_joint,
					     const std::shared_ptr<Robot>& i_robot,
					     const BaseCalibration::Type i_type)
  : BaseCalibration(i_joint, i_robot, i_type),
    m_searchRange(0.3),
    m_stepSize(1),
    m_delay(1500),
    m_prevController(nullptr),
    m_testVector() {
}


MotorSpeedCalibration::~MotorSpeedCalibration() {
  if (m_prevController) {
    m_robot->setSpeedController(m_prevController);
  }
}


/// indicate if execution is possible -> all requirements are met
bool MotorSpeedCalibration::isReady() const {
  LOG_DEBUG("Is ready");
  // currently it only works with drivers, maybe in the future :)
  return m_joint->getMotor()->getType() == BaseMotor::StepperDriver &&
    m_robot->hasValidConnection();
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
  if (m_robot->getSpeedController()->getType() != SpeedController::Manual) {
    m_prevController = m_robot->getSpeedController();
    m_robot->setSpeedController(std::make_shared<ManualSpeedController>());
  }
  std::static_pointer_cast<ManualSpeedController>
    (m_robot->getSpeedController())->setSpeedVector(i_speedVector);
  for (unsigned int i = 0; i < i_speedVector.size(); ++i) {
    std::string jointDirection = getJointDirection();
    m_robot->prepareSteps(jointDirection, i_stepsPerSpeed);
  }
  m_robot->actuate();
  std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
  return m_finished;
}


void MotorSpeedCalibration::prepareParameterVector(const int&(StepperDriver::*i_function)() const) {
  std::shared_ptr<StepperDriver> driver =
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
  int currentTestValue = ((*driver).*i_function)() * (1 - m_searchRange);
  LOG_DEBUG("Starting value is : " << currentTestValue);
  int end = ((*driver).*i_function)() * (1 + m_searchRange);
  LOG_DEBUG("end value is : " << end);
  // create a vector in which we describe all the accelerations
  m_testVector.clear();
  while (currentTestValue < end) {
    m_testVector.push_back(currentTestValue);
    currentTestValue += m_stepSize;
  } 
}


void MotorSpeedCalibration::createOutput() {
  std::stringstream stringStream;
  stringStream << "Calibrated value is: " << getCalibratedParameter() << std::endl;
  m_output->addAsChapter(stringStream.str());  
}
