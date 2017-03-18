// Copyright [2017] Ruud Cools

#include <macroHeader.h>
#include <PointCalibration.h>
#include <Robot.h>
#include <JointController.h>
#include <BaseJoint.h>
#include <Point2D.h>
#include <MovementRegistrator.h>
#include <BaseTraceCalculator.h>
#include <Trace.h>
#include <CalibrationOutput.h>

PointCalibration::PointCalibration(const std::shared_ptr<BaseJoint>& i_joint,
				   const std::shared_ptr<Robot>& i_robot)
  :  BaseCalibration(i_joint, i_robot, BaseCalibration::PointCalibration),
     m_registrator(std::make_shared<MovementRegistrator>()) {
  m_robot->registerMovementRegistrator(m_registrator);
}


PointCalibration::~PointCalibration() {
  m_robot->deregisterMovementRegistrator(m_registrator);
}


void PointCalibration::execute() {
  if (!isReady()) {
    return;
  }
  traceType currentStepSize = m_joint->getMovementPerStep();
  for (unsigned int i = 0;
       i < m_stepsBetweenPoints.size();
       i++) {
    m_stepSizes.push_back
      (currentStepSize * (m_priorEstimateSteps[i]/ m_stepsBetweenPoints[i]));
  }
  createOutput();
  m_finished = true;
}


bool PointCalibration::isReady() const {
  if (m_points.size() < 2) {
    return false;
  }
  return m_stepsBetweenPoints.size() == m_points.size() - 1;
}


void PointCalibration::apply() { 
  double averageStepSize(0.0);
  for (const auto& stepSize : m_stepSizes) {
    averageStepSize += stepSize;
  }
  averageStepSize /= m_stepSizes.size();
  m_joint->setMovementPerStep(averageStepSize);
}

  

void PointCalibration::addPoint2D(const Point2D& i_point) {
  // add a point to which will be traveled
  m_points.push_back(i_point);
  if (m_stepsBetweenPoints.size() < m_points.size() - 1) {
    m_stepsBetweenPoints.push_back(0);
  }
  if (m_priorEstimateSteps.size() < m_points.size() - 1) {
    m_priorEstimateSteps.push_back(0);
  }
}


bool PointCalibration::removePoint2D(const Point2D& i_point) {
  auto iterator = std::find(m_points.begin(), m_points.end(), i_point);
  if (iterator == m_points.end()) {
    LOG_DEBUG("Could not find the point that needs to be removed.");
    return false;
  }
  int distance = std::distance(m_points.begin(), iterator) -1;
  if (distance >= (int)m_priorEstimateSteps.size()) {
    LOG_DEBUG("Could not find the estimated step of the point");
    return false;
  }
  if (distance >= (int)m_stepsBetweenPoints.size()) {
    LOG_DEBUG("Could not find the steps between points for this point");
    return false;
  }
  
  m_points.erase(iterator);
  // Want to delete the 
  if (distance == -1) {
    LOG_DEBUG("Want to delete the first point! No need to delete the other stuff!");
    return true;
  }
  m_priorEstimateSteps.erase(m_priorEstimateSteps.begin() + distance);
  m_stepsBetweenPoints.erase(m_stepsBetweenPoints.begin() + distance);
  return true;  
}


void PointCalibration::recordPoint(const Point2D& i_point) {
  try {
    recordPoint(i_point,
		m_registrator->getGlobalMap().at(m_joint));
  } catch (std::out_of_range) {
    LOG_INFO("Joint did not move!");
    recordPoint(i_point,0);
  }
  m_registrator->resetGlobal();
}


void PointCalibration::recordPoint(const Point2D& i_point,
				   const int& i_stepsSet) {
  // estimate the error in the other joints
  JointMap errorMap = estimateErrorOtherJoints(i_point);
  addOtherJointErrorToOutput(errorMap, i_point);

  // update the robot position
  m_robot->setPosition(i_point);
  // tell the calibration unit at which position we are currently and how manny steps it took
  int pointNumber = std::distance(m_points.begin(),
				  std::find(m_points.begin(), m_points.end(), i_point));
  if (pointNumber == 0) {
    return;
  }
  m_stepsBetweenPoints[pointNumber - 1] = i_stepsSet;
  m_priorEstimateSteps[pointNumber - 1] = estimateStepsBetweenPoints(m_points[pointNumber-1],
								     m_points[pointNumber]);
}


int PointCalibration::estimateStepsBetweenPoints(const Point2D& i_start,
						 const Point2D& i_end) const {
  BaseTraceCalculator baseTraceCalculator(m_robot.get());
  Trace trace(i_start, i_end);
  std::vector<int> steps = baseTraceCalculator.getNumberOfSteps(trace, i_start);
  if (m_joint->getMovementType() == BaseJoint::Rotational) {
    return steps[0];
  } else if (m_joint->getMovementType() == BaseJoint::Translational) {
    return steps[1];
  } else {
    LOG_ERROR("Unknown joint type");
  }
}


PointCalibration::JointMap PointCalibration::estimateErrorOtherJoints(const Point2D& i_point) {
  JointController::JointPointerVector noneCalibratedJoints;
  JointController::JointPointerVector allJoints =
    m_robot->getJointController()->getJointPointerVector();
  for (auto& joint : allJoints) {
    if (joint != m_joint) {
      noneCalibratedJoints.push_back(joint);
    }
  }

  JointMap positionMap;
  for (auto& joint : noneCalibratedJoints) {
    positionMap[joint] = joint->getPosition();
  }

  m_robot->setPosition(i_point);
  JointMap errorMap;
  for (auto& joint : noneCalibratedJoints) {
    errorMap[joint] = positionMap[joint] - joint->getPosition();
  }
  
  for (auto& joint : noneCalibratedJoints) {
    joint->setPosition(positionMap[joint]);
  }
  return errorMap; 
}


void PointCalibration::addOtherJointErrorToOutput(const JointMap& i_map,
						  const Point2D& i_point) {
  std::string output("Uncertainties seen during point calibration at point: x ");
  output.append(std::to_string(i_point.x));
  output.append(", y: ");
  output.append(std::to_string(i_point.y));
  output.append(". \n");
  for (const auto& element : i_map) {
    if (element.first->getMovementType() == BaseJoint::Rotational) {
      output.append("Rotational joint: ");
    } else if (element.first->getMovementType() == BaseJoint::Translational) {
      output.append("Translational joint: ");
    } else {
      LOG_ERROR("Could not resolve joint");
    }
    output.append(" had an positional error of: ");
    output.append(std::to_string(element.second));
    output.append("/n");
  }
  m_output->addAsChapter(output);
}


void PointCalibration::createOutput() {
  int pointNumber(1);
  traceType averageStepSize(0.0);
  std::string output;
  for (const auto& stepSize : m_stepSizes) {
    output.append("The estimate step size determined between point");
    output.append(std::to_string(pointNumber-1));
    output.append(" and  ");
    output.append(std::to_string(pointNumber));
    output.append(" is: ");
    output.append(std::to_string(stepSize));
    output.append("\n");
    averageStepSize += stepSize;
    ++pointNumber;
  }
  averageStepSize /= m_stepSizes.size();
  output.append("The average movement per step is: ");
  output.append(std::to_string(averageStepSize));
  output.append("\n");
  m_output->addAsChapter(output);
}
