// Copyright [2017] Ruud Cools

#include <macroHeader.h>
#include <MovementRegistrator.h>
#include <BaseJoint.h>

void MovementRegistrator::notifySteps(JointMap* io_jointMap,
				      const std::weak_ptr<BaseJoint>& i_joint,
				      const int& i_numberOfSteps) {
  // set the motor speed to the pull in frequency
  // to ensure calculations works as expected
  auto element = io_jointMap->find(i_joint);
  if (element == io_jointMap->end()) {
    addElementToMap(io_jointMap, i_joint);
    element = io_jointMap->find(i_joint);
  }
  element->second += i_numberOfSteps;
}


int MovementRegistrator::numberOfStepsSet(const std::weak_ptr<BaseJoint>& i_joint,
					  const bool& i_isLocal /* =true*/) {
  if (i_isLocal) {
    return m_localMap[i_joint];
  } else {
    return m_globalMap[i_joint];
  }
}


int MovementRegistrator::numberOfStepsSet(const std::shared_ptr<BaseJoint>& i_joint,
					  const bool& i_isLocal /* =true*/) {
 return numberOfStepsSet(std::weak_ptr<BaseJoint>(i_joint), i_isLocal);
}




void MovementRegistrator::addElementToMap(JointMap* io_jointMap,
				     std::weak_ptr<BaseJoint> i_joint) {
  (*io_jointMap)[i_joint] = 0;
}


void MovementRegistrator::notifySteps(const std::shared_ptr<BaseJoint>& i_joint,
				      const int& i_numberOfSteps) {
  notifySteps(&m_localMap, i_joint, i_numberOfSteps);
  notifySteps(&m_globalMap, i_joint, i_numberOfSteps);
}


void MovementRegistrator::notifyActuation() {
  resetLocal();
}


void MovementRegistrator::resetLocal() {
  resetJointMap(&m_localMap);
}

void MovementRegistrator::resetGlobal() {
  resetJointMap(&m_globalMap);
}


void MovementRegistrator::resetJointMap(JointMap* o_map) {
  o_map->clear();
}
