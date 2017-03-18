// Copyright Ruud Cools [2017]

#include <macroHeader.h>
#include <MaximumSpeedCalibration.h>
#include <BaseJoint.h>
#include <BaseMotor.h>
#include <StepperDriver.h>
#include <BaseJoint.h>
#include <Robot.h>
#include <CalibrationOutput.h>

MaximumSpeedCalibration::MaximumSpeedCalibration(const std::shared_ptr<BaseJoint>& i_joint,
						 const std::shared_ptr<Robot>& i_robot) 
  : MotorSpeedCalibration(i_joint, i_robot, BaseCalibration::MaximumCalibration),
    m_maximumSpeed(0) {
}


void MaximumSpeedCalibration::execute() {
  m_finished = false;
  prepareParameterVector(&StepperDriver::getMaxSpeed);

  std::shared_ptr<StepperDriver> driver = 
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
  auto currentMaximum = m_testVector.begin();
  auto end = m_testVector.end();
  while ((!m_finished) && (currentMaximum != end)) {
    std::vector<int> speedVector;
    int currentSpeed = driver->getPullIn();
    while (currentSpeed < *currentMaximum) {
      speedVector.push_back(currentSpeed);
      currentSpeed += driver->getIncremental();
    }
    if (speedVector.size() > 1) {
      m_finished = testSpeed(speedVector, 1);
    } 
    LOG_DEBUG("Is finshed: " << std::boolalpha << m_finished);
    LOG_DEBUG("Current Maximum is: " << *currentMaximum);
    if (!m_finished) {
      ++currentMaximum;
    }
  }
  m_maximumSpeed = *currentMaximum;
  m_finished = true;
  m_hasSucceded = m_maximumSpeed < *end;
}


void MaximumSpeedCalibration::apply() {
  std::shared_ptr<StepperDriver> driver =
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
    driver->setMaxSpeed(m_maximumSpeed);
}


void MaximumSpeedCalibration::createOutput() {
  std::stringstream stringStream;
  stringStream << "Calibrated maximum speed is: " << m_maximumSpeed << std::endl;
  m_output->addAsChapter(stringStream.str());
}
