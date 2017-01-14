// Copyright Ruud Cools [2017]

#include <macroHeader.h>
#include <EndStopCalibration.h>
#include <Robot.h>
#include <EndStop.h>
#include <MovementRegistrator.h>
#include <JointController.h>
#include <CalibrationOutput.h>

EndStopCalibration::EndStopCalibration(const std::shared_ptr<BaseJoint>& i_joint,
				       const std::shared_ptr<Robot>& i_robot)
  : BaseCalibration(i_joint, i_robot, EndStopPosition),
    m_registrator(),
    m_hasRegistrator(false),
    m_doStepMovementCalibration(false),
    m_movementPerStep(0.0),
    m_jointPosition(0.0),
    m_finalStepRate(0.3) {
}


EndStopCalibration::~EndStopCalibration() {
  if (m_registrator) {
    m_robot->deregisterMovementRegistrator(m_registrator);
  }
}


bool EndStopCalibration::canDoMovementPerStepCalibration() const {
  return m_joint->getEndStops().size() > 1;
}
    

bool EndStopCalibration::setDoStepMovementCalibration(const bool& i_value) {
  if (!i_value) {
    m_doStepMovementCalibration = i_value;
    return true;
  }
  
  if (canDoMovementPerStepCalibration()) {
    m_doStepMovementCalibration = i_value;
    m_registrator = std::make_shared<MovementRegistrator>();
    m_robot->registerMovementRegistrator(m_registrator);
    return true;
  } else {
    m_doStepMovementCalibration = false;
    return false;
  }
}


void EndStopCalibration::execute() {
  // Do stuff that will update the position of the joint based on the first endstop
  executePositionUpdateOnEndStop(m_joint->getEndStops().front());
  if (m_doStepMovementCalibration) {
    executeMovementPerStep();
  }
}


void EndStopCalibration::executeMovementPerStep() {
  std::vector<int> estimatedSteps;
  std::vector<int> setSteps;
  std::vector<traceType> movementPerStep;
  for (auto itr = m_joint->getEndStops().begin() + 1;
       itr!=m_joint->getEndStops().end();
       ++itr) {
    if (m_registrator) {
      m_registrator->resetGlobal();
    }
    estimatedSteps.push_back(estimateSteps(*itr));
    executePositionUpdateOnEndStop(*itr);
    setSteps.push_back(m_registrator->getLocalMap().at(m_joint));
  }
  
  for (auto estimatedItr = estimatedSteps.begin(),setItr = setSteps.begin();
       estimatedItr != estimatedSteps.end() && setItr != setSteps.end();
       ++estimatedItr, ++setItr) {
    movementPerStep.push_back(*setItr / *estimatedItr * m_joint->getMovementPerStep());
  }
  createOutputForMovementPerStep(movementPerStep);
}


bool EndStopCalibration::executePositionUpdateOnEndStop
(const std::shared_ptr<EndStop>& i_endStop) {
  std::string movement(getJointMovementToEndStop(i_endStop));
  traceType distance = std::abs(i_endStop->getPosition() - m_joint->getPosition());
  int steps = distance / m_joint->getMovementPerStep();
  // set steps until we hit the the end stop
  while (true) {
    // the steps will be the final rate of the estimate steps
    steps *= m_finalStepRate;
    if (steps == 0) {
      steps = 1;
    }
    // make the robot set steps
    m_robot->prepareSteps(movement,steps);
    try {
      m_robot->actuate();
    } catch (std::runtime_error error) {
      if (strcmp(error.what(),"Endstop has been hit.") == 0) {
	/// endstop has been hit
	break;
      }
    }    
  }
  
  // update the joint
  std::shared_ptr<EndStop> endStop = m_robot->getJointController()->resolveEndStopHit();
  std::string output;
  bool result;
  if (endStop == i_endStop) {
    output.append("Updated joint position to: ");
    output.append(std::to_string(i_endStop->getPosition()));
    output.append(".\n Succesfully!\n");
    m_output->addAsChapter(output);
    result = true;
  } else {
    output.append("Not succesfully updated the joint position.\n")
      .append("Current position is: ")
      .append(std::to_string(m_joint->getPosition()))
      .append("./n Endpoint position is: ")
      .append(std::to_string(i_endStop->getPosition()));
    m_output->addAsChapter(output);
    result = false;
  }
  return result;
}


std::string EndStopCalibration::getJointMovementToEndStop
(const std::shared_ptr<EndStop> i_endStop) const{
  bool isLess = i_endStop->getPosition() < m_joint->getPosition();
  return getJointDirection(isLess);
}


bool EndStopCalibration::isReady() const {
  bool isReady(m_joint->getEndStops().size() > 0);
  if (m_doStepMovementCalibration) {
    isReady &= canDoMovementPerStepCalibration();
  }
  return isReady;
}
  

void EndStopCalibration::apply() {
  if (m_doStepMovementCalibration) {
    if (m_movementPerStep == 0.0) {
      LOG_ERROR("Movement per step calibration is requested but not set");
    }
    m_joint->setMovementPerStep(m_movementPerStep);
  }
}


int EndStopCalibration::estimateSteps(const std::shared_ptr<EndStop>& i_endStop) const {
  return std::abs(i_endStop->getPosition() - m_joint->getPosition()) / m_joint->getMovementPerStep();
}


void EndStopCalibration::createOutputForMovementPerStep
(const std::vector<traceType>& i_movementPerStep) {
  std::string output("The movement per step is estimated by driving it from one end stop to the other\n");
  traceType average(0.0);
  for (const int& movement : i_movementPerStep) {
    output.append("Movement per step was calculated as: ")
      .append(std::to_string(movement))
      .append(" mm/step./n");
    average += movement;
  }
  average /= i_movementPerStep.size();
  output.append("The average movement per step is: ")
    .append(std::to_string(average))
    .append(" mm/step");
  m_output->addAsChapter(output);
}
