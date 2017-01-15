//Copyright Ruud Cools [2017]
#include <macroHeader.h>
#include <CalibrationDirector.h>
#include <BaseJoint.h>
#include <Robot.h>
#include <JointController.h>
#include <PointCalibration.h>
#include <EndStopCalibration.h>
#include <MaximumSpeedCalibration.h>
#include <PullInCalibration.h>
#include <AccelerationCalibration.h>


CalibrationDirector::CalibrationDirector(const std::shared_ptr<Robot>& i_robot, 
					 const std::shared_ptr<BaseJoint>& i_joint /* = nullptr*/)
  :m_calibrations(),
   m_currentCalibration(),
   m_robot(i_robot),
   m_joint(i_joint),
   m_pointCalibration(true),
   m_motorCalibration(true),
   m_endstopPositionCalibration(true),
   m_endstopMovementCalibration(true) {
}

std::shared_ptr<BaseCalibration> CalibrationDirector::next() {
  if (m_calibrations.size() == 0) {
    if (m_joint) {
      getCalibrations(m_joint, &m_calibrations);
    } else {
      getCalibrations(&m_calibrations);
    }
    m_currentCalibration = m_calibrations.begin();
  }
  ++m_currentCalibration;
  if (m_currentCalibration == m_calibrations.end()) {
    m_calibrations.clear();
    return std::shared_ptr<BaseCalibration>();
  } else {
    return *m_currentCalibration;
  }
}
  

void CalibrationDirector::getCalibrations(const std::shared_ptr<BaseJoint>& i_joint,
					  CalibrationDirector::CalibrationVector* o_vector) const {
  CalibrationDirector::CalibrationVector calibrationVector;
  // end stop calibration
  bool hasEndStopPosition(false), hasEndStopMovement(false);
  if (m_endstopPositionCalibration) {
    std::shared_ptr<EndStopCalibration> calibration;
    if (i_joint->getEndStops().size() > 0) {
      calibration = std::make_shared<EndStopCalibration>(i_joint, m_robot);
      hasEndStopPosition = true;
      calibration->setDoStepMovementCalibration(false);
      if (m_endstopMovementCalibration) {
	if (i_joint->getEndStops().size() > 1) {
	  hasEndStopMovement = true;
	  calibration->setDoStepMovementCalibration(true);
	}
      }
    }
    if (calibration) {
      o_vector->push_back(calibration);
    }
  }
  if (m_pointCalibration) {
    if (!(hasEndStopPosition && hasEndStopMovement)) {
      o_vector->push_back(std::make_shared<PointCalibration>(i_joint, m_robot));
    }
  }

  if (m_motorCalibration) {
    o_vector->push_back(std::make_shared<AccelerationCalibration>(i_joint, m_robot));
    o_vector->push_back(std::make_shared<PullInCalibration>(i_joint, m_robot));
    o_vector->push_back(std::make_shared<MaximumSpeedCalibration>(i_joint, m_robot));
  }
}


void CalibrationDirector::getCalibrations(CalibrationDirector::CalibrationVector* o_vector) const {
  CalibrationVector calibrationVector;
  for (const auto& element: m_jointMap) {
    if (element.second) {
      getCalibrations(element.first, o_vector);
    }
  }
}


void CalibrationDirector::initialiseJointMap() {
  for (const auto& joint : m_robot->getJointController()->getJointPointerVector()) {
    m_jointMap[joint] = true;
  }
}


void CalibrationDirector::updateJointMap(const std::shared_ptr<BaseJoint>& i_joint,
					 const bool& i_include) {
  m_jointMap[i_joint] = i_include;
}


int CalibrationDirector::getNumberOfCalibrations() const {
  int count(0);
  for (const auto& element: m_jointMap) {
    if (element.second) {
      getNumberOfCalibrations(element.first, &count);
    }
  }
  return count;
}


void CalibrationDirector::getNumberOfCalibrations(const std::shared_ptr<BaseJoint> i_joint,
						 int* io_count) const {
  bool hasEndStopPosition(false), hasEndStopMovement(false);
  // count the end stop calibration.
  if (m_endstopPositionCalibration) {
    if (i_joint->getEndStops().size() > 0) {
      *io_count += 1;
	hasEndStopPosition = true;
      if (m_endstopMovementCalibration) {
	if (i_joint->getEndStops().size() > 1) {
	  *io_count += 1;
	}
      }
    }
  }
  // count the point calibration
  if (m_pointCalibration) {
    if (!(hasEndStopPosition && hasEndStopMovement)) {
      *io_count += 1;      
    }
  }
  // count the motor calibrations
  if (m_motorCalibration) {
        *io_count += 3;
  } 
}
