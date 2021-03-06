// Copyright Ruud Cools [2017]
#include <macroHeader.h>
#include <AccelerationCalibration.h>
#include <Robot.h>
#include <BaseJoint.h>
#include <StepperDriver.h>
#include <BaseMotor.h>
#include <CalibrationOutput.h>


AccelerationCalibration::AccelerationCalibration(const std::shared_ptr<BaseJoint>& i_joint,
						 const std::shared_ptr<Robot>& i_robot) 
  : MotorSpeedCalibration(i_joint, i_robot, BaseCalibration::AccelerationCalibration),
    m_acceleration(0.0) {
}


void AccelerationCalibration::execute() {
  m_finished = false;
  prepareParameterVector(&StepperDriver::getIncremental);
  std::shared_ptr<StepperDriver> driver = 
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
  auto currentAcceleration(m_testVector.begin());
  auto end(m_testVector.end());
  while ((!m_finished) && (currentAcceleration != end)) {
    std::vector<int> speedVector;
    int currentSpeed = driver->getPullIn();
    int max = driver->getMaxSpeed();
    while (currentSpeed < max) {
      speedVector.push_back(currentSpeed);
      currentSpeed += *currentAcceleration;
    }
    // no 1 sized vector
    if (speedVector.size() > 1) {
      m_finished = testSpeed(speedVector, 1);
    }    
    if (!m_finished) {
      ++currentAcceleration;
    }
  }
  m_acceleration = *currentAcceleration;
  m_finished = true;
  m_hasSucceded = m_acceleration < *end;
}


void AccelerationCalibration::apply() {
  std::shared_ptr<StepperDriver> driver =
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
  driver->setIncremental(m_acceleration);
}


void AccelerationCalibration::createOutput() {
  std::stringstream stringStream;
  stringStream << "Calibrated acceleration is: " << m_acceleration << std::endl;
  m_output->addAsChapter(stringStream.str());  
}
