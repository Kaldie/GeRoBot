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
		      const std::shared_ptr<Robot>& i_robot)
  : BaseCalibration(i_joint, i_robot, MotorSpeed),
    m_calibratePullIn(false),
    m_pullIn(0),
    m_pullInLength(10),
    m_calibrateAcceleration(false),
    m_acceleration(0.0),
    m_calibrateMaximumSpeed(false),
    m_maximumSpeed(0),
    m_searchRange(0.3),
    m_stepSize(1),
    m_delay(1500),
    m_prevController(i_robot->getSpeedController()) {
  m_robot->setSpeedController(std::make_shared<ManualSpeedController>());
}


MotorSpeedCalibration::~MotorSpeedCalibration() {
  m_robot->setSpeedController(m_prevController);
}

/// Preform the calibration -> produce the end results
void MotorSpeedCalibration::execute() {
  //TODO magic
  bool hasSucceded(true);
  if (m_calibratePullIn) {
    m_pullInSucces = calibratePullIn();
    hasSucceded &= m_pullInSucces;
  }
  if (m_calibrateAcceleration) {
    m_accelerationSucces = calibrateAcceleration();
    hasSucceded &= m_accelerationSucces;
  }
  if (m_calibrateMaximumSpeed) {
     m_maximumSpeedSucces = calibrateMaximumSpeed();
     hasSucceded &= m_maximumSpeedSucces;
  }
  m_finished = hasSucceded;
}


/// indicate if execution is possible -> all requirements are met
bool MotorSpeedCalibration::isReady() const {
  // currently it only works with drivers, maybe in the future :)
  return m_joint->getMotor()->getType() == BaseMotor::StepperDriver;
}


/// apply the values retrieved during the calibration to the joint
void MotorSpeedCalibration::apply() {
  std::shared_ptr<StepperDriver> driver =
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
  if (m_calibratePullIn) {
    driver->setPullIn(m_pullIn);
  }
  if (m_calibrateAcceleration) {
    driver->setIncremental(m_acceleration);
  }
  if (m_calibrateMaximumSpeed) {
    driver->setMaxSpeed(m_maximumSpeed);
  }  
}


std::string MotorSpeedCalibration::getJointDirection() const {
  std::vector<traceType> range = m_joint->getRange();
  traceType position = m_joint->getPosition();
  bool isInner = std::abs(position - range[0]) < std::abs(position - range[1]);
  return BaseCalibration::getJointDirection(isInner);
}


bool MotorSpeedCalibration::calibratePullIn() {  
  m_finished = false;
  // determine the start and stop point
  std::shared_ptr<StepperDriver> driver =
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
  int testSpeed = driver->getPullIn() * (1 - m_searchRange);
  int end = driver->getPullIn() * (1 + m_searchRange);
  // create a vector in which we describe all the pull in speeds
  std::vector<int> speeds;
  while (testSpeed < end) {
    speeds.push_back(testSpeed);
    testSpeed += m_stepSize;
  } 
  // start with the tests untill done is true
  // done is set by the testSpeed method
  // which returns m_isFinished which will be set by the user via the gui
  std::vector<int>::iterator currentSpeed = speeds.begin();
  bool done(false);
  std::vector<int> speedVector(1,0);
  while (!done && (currentSpeed != speeds.end())) {
    speedVector[0] = *currentSpeed;
    done = MotorSpeedCalibration::testSpeed(speedVector, m_pullInLength);
    if (!done) {
      ++currentSpeed;
    }
  }
  // store the results
  m_pullIn = *currentSpeed;
  // if the user didn't interven we known the pull in can be higher
  return (m_pullIn >= end) || (m_pullIn == driver->getPullIn() * (1 - m_searchRange));
}


bool MotorSpeedCalibration::calibrateMaximumSpeed() {
  m_finished = false;
  // determine the start and stop point
  std::shared_ptr<StepperDriver> driver =
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
  int testMaximum = driver->getMaximumSpeed() * (1 - m_searchRange);
  int end = driver->getMaximumSpeed() * (1 + m_searchRange);
  // create a vector in which we describe all the accelerations
  std::vector<int> maximums;
  while (testMaximum < end) {
    maximums.push_back(testMaximum);
    testMaximum += m_stepSize;
  }

  // start with the tests untill done is true
  // done is set by the testSpeed method
  // which returns m_isFinished which will be set by the user via the gui
  std::vector<int>::iterator currentMaximum = maximums.begin();
  bool done(false);

  while (!done && (currentMaximum != maximums.end())) {
    std::vector<int> speedVector;
    int currentSpeed = driver->getPullIn();
    while (currentSpeed < *currentMaximum) {
      speedVector.push_back(currentSpeed);
      currentSpeed += driver->getIncremental();
    }
    done = testSpeed(speedVector, 1);
    if (!done) {
      ++currentMaximum;
    }
  }
  m_maximumSpeed = *currentMaximum;
  return (m_maximumSpeed < end) || (driver->getMaximumSpeed() * (1 - m_searchRange));  
}




  

bool MotorSpeedCalibration::calibrateAcceleration() {
  m_finished = false;
  // determine the start and stop point
  std::shared_ptr<StepperDriver> driver =
    std::static_pointer_cast<StepperDriver>(m_joint->getMotor());
  int testAcceleration = driver->getIncremental() * (1 - m_searchRange);
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
  std::vector<int>::iterator currentAcceleration = accelerations.begin();
  bool done(false);

  while (!done && (currentAcceleration != accelerations.end())) {
    std::vector<int> speedVector;
    int currentSpeed = driver->getPullIn();
    int max = driver->getMaxSpeed();
    while (currentSpeed < max) {
      speedVector.push_back(currentSpeed);
      currentSpeed += *currentAcceleration;
    }
    done = testSpeed(speedVector, 1);
    if (!done) {
      ++currentAcceleration;
    }
  }
  m_acceleration = *currentAcceleration;
  return (m_acceleration < end) || (m_acceleration == driver->getIncremental() * (1 - m_searchRange));  
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

