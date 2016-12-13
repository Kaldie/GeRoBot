// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <JointController.h>
#include <ArduinoMotorDriver.h>
#include <StateSequence.h>
#include <SequenceVector.h>
#include <BaseJoint.h>
#include <BaseMotor.h>
#include <PinState.h>

JointController::JointController()
  :m_jointPointerVector({}),
   m_actuator(ArduinoMotorDriver("/dev/ttyUSB*")),
   m_sequenceVector() {
}


JointController::~JointController()
{}


bool JointController::validateJoint(const BaseJoint::JointPointer& i_baseJoint) const {
  // pins should not be other then 2 t/m 7
  if (!i_baseJoint) {
    LOG_ERROR("Invalid JointPointer!");
  }
  PinVector pins = i_baseJoint->getMotor()->getPins();
  for (PinVector::const_iterator itr = pins.begin();
      itr != pins.end();
      itr++) {
    
    if (((*itr) > std::get<1>(m_actuator.getJointPinRange())) ||
	((*itr) < std::get<0>(m_actuator.getJointPinRange()))) {
      LOG_INFO("Pin: " << (*itr) << " is not within range!!");
      return false;
    }
  }
  return true;
}


bool JointController::validateJointVector(
    const JointController::JointPointerVector& i_jointVector) const {
  // determines if the pinIterator-> is able to grow into a good JointVector
  for (JointController::JointPointerVector::const_iterator itr = i_jointVector.begin();
       itr != i_jointVector.end();
       itr++) {
    // All joints should be valid
    if (!validateJoint(*itr))
      return false;
  }
  // Number of joints should not be other then 2....or max 2...
  if (i_jointVector.size() > 2)
    return false;
  // If the number of joints is 1 we're done!
  if (i_jointVector.size() == 1)
    return true;
  // Movement type of the joints should not be equal
  if (i_jointVector[0]->getMovementType() ==
      i_jointVector[1]->getMovementType())
    return false;
  // get all pins
  PinVector pinVector;
  for (auto jointIterator = i_jointVector.begin();
       jointIterator != i_jointVector.end();
       jointIterator++) {
    PinVector jointPins = (*jointIterator)->getMotor()->getPins();
    pinVector.insert(pinVector.end(), jointPins.begin(), jointPins.end());
  }
  for (auto pinIterator = pinVector.begin();
       pinIterator != pinVector.end();
       pinIterator++) {
    if (std::find(pinIterator, pinVector.end(), *pinIterator) ==
        pinVector.end()) {
      LOG_DEBUG("Joint is not available, defines the same pins!");
      return false;
    }
  }
  return true;
}


bool JointController::addJoint(const BaseJoint::JointPointer& i_joint) {
  /*
    This gonna be fun!!
    We want to store the joints in a pinIterator->, however they are defined as base joints
    If we want to use the derived stuff, we need to:
    clone it
    put the clone on the heap 
    put a pointer in the m_jointPointerVector
  */
  if (!hasJoint(i_joint)) {
    m_jointPointerVector.push_back(i_joint);

    if (!validateJointVector(m_jointPointerVector)){
      m_jointPointerVector.pop_back();
      LOG_ERROR("Joint is not valid!");
    }
  }
  LOG_DEBUG("Resetting pin state sequence");
  LOG_DEBUG("Current size: " << m_jointPointerVector.size());

  resetPinStateSequence();
  return true;
}


BaseJoint::JointPointer JointController::getJoint(const BaseJoint::MovementType &i_movementType) const {
  if (m_jointPointerVector.size() == 0)
    LOG_ERROR("No joints defined yet");

  for (JointController::JointPointerVector::const_iterator itr = m_jointPointerVector.begin();
       itr != m_jointPointerVector.end();
       itr++) {
    if ((*itr)->getMovementType() == i_movementType) {
      return *itr;
    }
  }
  LOG_ERROR("Could not find joint with the correct movement type!");
}


void JointController::resetPinStateSequence() {
  StateSequence stateSequence;
  for (auto& joint : m_jointPointerVector) {
    stateSequence.addToSequence(joint->getMotor()->getCurrentPinState(), true);
  }
  m_sequenceVector.appendStateSequence(stateSequence, false);
  m_sequenceVector.clean();
}


bool JointController::hasJoint(const BaseJoint::JointPointer& i_jointPointer) const {
  for (JointPointerVector::const_iterator itr = m_jointPointerVector.begin();
      itr != m_jointPointerVector.end();
      itr++) {
    if (i_jointPointer == (*itr))
      return true;
  }
  return false;
}


const BaseJoint::JointPointer JointController::getRootJoint() const {
  for (auto jointPointer : m_jointPointerVector) {
    if (jointPointer->getParent().expired()) {
      return jointPointer;
    }
  }
  LOG_ERROR("Could not find a parent joint!");
}


void JointController::moveSteps(const std::string& i_directionString,
                                const int& i_numberOfSteps) {
  resolveJoint(i_directionString)->
    moveSteps(i_directionString, i_numberOfSteps, &m_sequenceVector);
}


//  Brief: Actuate the robot from the given pin state sequence
void JointController::actuate() {
  m_actuator.actuate();
  resetPinStateSequence();
}


void JointController::uploadSequence(const bool& i_condense) {
  LOG_DEBUG("Sending: " << m_sequenceVector.numberOfSequences() << " sequences!");
  LOG_DEBUG("Sending: " << m_sequenceVector.numberOfSteps() << " steps!");
  m_sequenceVector.normalise();
  if (i_condense) {
    m_sequenceVector.condenseVector();
  }
#ifdef DEBUG
  unsigned long sendCount = 0;
  unsigned long totalSize = m_sequenceVector.getSequenceVector().size();
#endif
  for (const auto& stateSequence : m_sequenceVector) {
    /// Get the integer sequence of this pin state
    if (stateSequence.getNumberOfRepetitions() > 0) {
      m_actuator.upload(stateSequence.createArduinoBuffer());
#ifdef DEBUG
      ++sendCount;
      LOG_DEBUG("Percentage send: " <<
                static_cast<float>(sendCount) / totalSize * 100 << "%.");
#endif
    }
  }
}


int JointController::resolveEndStopHit(JointPointerVector* o_jointVector,
				       std::vector<std::string>* o_direction) {
  o_jointVector->clear();
  o_direction->clear();
  int result = m_actuator.resolveEndStopHit();

  PinState pinState;
  std::tuple<int, int> pinRange(m_actuator.getJointPinRange());
  for (int i= std::get<0>(pinRange);
       i <= std::get<1>(pinRange); ++i) {
      pinState.update(i, 0);
  }
  // if you mod with the max pin number + 1
  // you get the integer value of the pins 0 to max pin number
  pinState.setNumericValue(result % (1 << (std::get<1>(pinRange) + 1)));
  
  std::string jointDirection;
  for (const auto& joint : m_jointPointerVector) {
    if (joint->getJointStatus(pinState, &jointDirection)) {
      o_jointVector->push_back(joint);
      o_direction->push_back(jointDirection);
    }
  }
  return result;
}



BaseJoint::JointPointer JointController::resolveJoint(const std::string& i_movementDirection) const {
  if (i_movementDirection == "CCW" or i_movementDirection == "CW") {
    return getJoint(BaseJoint::Rotational);
  }
  if (i_movementDirection == "IN" or i_movementDirection == "OUT") {
    return getJoint(BaseJoint::Translational);
  }
  LOG_ERROR("Could not resolve movement direction: " << i_movementDirection);
}


BaseJoint::JointPointer JointController::resolveJoint(const BaseJoint::MovementType& i_movementType) const {
  return getJoint(i_movementType);
}
