// Copyright Ruud Cools [2017]
#include <macroHeader.h>
#include <PullInCalibration.h>
#include <BaseJoint.h>
#include <BaseMotor.h>
#include <StepperDriver.h>
#include <BaseJoint.h>
#include <Robot.h>
#include <CalibrationOutput.h>


PullInCalibration::PullInCalibration(const std::shared_ptr<BaseJoint>& i_joint,
				     const std::shared_ptr<Robot>& i_robot) 
  : MotorSpeedCalibration(i_joint, i_robot, BaseCalibration::PullInCalibration),
    m_pullIn(0),
    m_pullInLength(10) {
}


void PullInCalibration::execute() {
  prepareParameterVector(&StepperDriver::getPullIn);
  auto currentSpeed = m_testVector.begin();
  auto end = m_testVector.end();
  // start with the tests untill done is true
  // done is set by the testSpeed method
  // which returns m_isFinished which will be set by the user via the gui
  std::vector<int> speedVector(1,0);
  while ((!m_finished) && (currentSpeed != end)) {
    speedVector[0] = *currentSpeed;
    m_finished = testSpeed(speedVector, m_pullInLength);
    if (!m_finished) {
      ++currentSpeed;
    }
  }
  // store the results
  m_pullIn = *currentSpeed;
  // if the user didn't interven we known the pull in can be higher
  m_finished = true;
  m_hasSucceded = m_pullIn < *end;
}


void PullInCalibration::apply() {
  std::shared_ptr<StepperDriver> driver =
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
  driver->setPullIn(m_pullIn);
}


void PullInCalibration::createOutput() {
  std::stringstream stringStream;
  stringStream << "Calibrated pull in speed is: " << m_pullIn << std::endl;
  m_output->addAsChapter(stringStream.str());  
}
