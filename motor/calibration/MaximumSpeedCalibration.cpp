// Copyright Ruud Cools [2017]

#include <macroHeader.h>
#include <MaximumSpeedCalibration.h>
#include <BaseJoint.h>
#include <BaseMotor.h>
#include <StepperDriver.h>
#include <BaseJoint.h>
#include <Robot.h>

MaximumSpeedCalibration::MaximumSpeedCalibration(const std::shared_ptr<BaseJoint>& i_joint,
						 const std::shared_ptr<Robot>& i_robot) 
  : MotorSpeedCalibration(i_joint, i_robot, BaseCalibration::MaximumCalibration),
    m_maximumSpeed(0) {
}


void MaximumSpeedCalibration::execute() {
  m_finished = false;
  std::vector<int>::const_iterator currentMaximum, end;
  prepareParameterVector(&currentMaximum, &end, &StepperDriver::getIncremental);

  std::shared_ptr<StepperDriver> driver = 
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());

  while (!m_finished && (currentMaximum != end)) {
    std::vector<int> speedVector;
    int currentSpeed = driver->getPullIn();
    while (currentSpeed < *currentMaximum) {
      speedVector.push_back(currentSpeed);
      currentSpeed += driver->getIncremental();
    }
    m_finished = testSpeed(speedVector, 1);
    if (!m_finished) {
      ++currentMaximum;
    }
  }
  m_maximumSpeed = *currentMaximum;
  m_finished = m_maximumSpeed < *end;
}


void MaximumSpeedCalibration::apply() {
  std::shared_ptr<StepperDriver> driver =
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
    driver->setMaxSpeed(m_maximumSpeed);
}
