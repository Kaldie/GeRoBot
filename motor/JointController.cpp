// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <JointController.h>
#include <ArduinoMotorDriver.h>
#include <StateSequence.h>
#include <SequenceVector.h>
#include <BaseJoint.h>
#include <BaseMotor.h>

JointController::JointController()
  :m_jointPointerVector({}),
   m_actuator(ArduinoMotorDriver("/dev/ttyUSB*")) {
  LOG_DEBUG("yay2");
}

JointController::~JointController()
{}

bool JointController::validateJoint(const BaseJoint::JointPointer& i_baseJoint) const {
  // pins should not be other then 2 t/m 7
  PinVector pins = i_baseJoint->getMotor()->getPins();
  for (PinVector::const_iterator itr = pins.begin();
      itr != pins.end();
      itr++) {
    // Pin should be between 2 and 7...make sense for arduino only!
    if (((*itr)>7) | ((*itr) <2)) {
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


BaseJoint::JointPointer JointController::getJoint(const MovementType &i_movementType) {
  if (m_jointPointerVector.size() == 0)
    LOG_ERROR("No joints defined yet");

  if (m_jointPointerVector.size() == 1)
    if (m_jointPointerVector[0]->getMovementType() == i_movementType)
      return m_jointPointerVector[0];

  if (m_jointPointerVector.size() == 2)
    for (JointController::JointPointerVector::iterator itr = m_jointPointerVector.begin();
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
    stateSequence.addState(joint->getCurrentPinState(), true);
  }
  m_sequenceVector.appendSequence(stateSequence);
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


void JointController::moveSteps(const std::string& i_directionString,
                                const int& i_numberOfSteps) {
  BaseJoint::JointPointer joint = resolveJoint(i_directionString);
  joint->moveSteps(i_directionString, i_numberOfSteps, &m_sequenceVector);
}


//  Brief: Actuate the robot from the given pin state sequence
void JointController::actuate() {
  uploadSequence();
  m_actuator.actuate();
  resetPinStateSequence();
}


void JointController::uploadSequence() {
  m_sequenceVector.normalise();
  for (PinStateSequenceVector::const_iterator pinStateSequenceIterator
         = m_sequenceVector.begin();
       pinStateSequenceIterator != m_sequenceVector.end();
       pinStateSequenceIterator++) {
    /// Get the integer sequence of this pin state
    if (pinStateSequenceIterator->getNumberOfRepetitions() > 0) {
      m_actuator.upload(pinStateSequenceIterator->createArduinoBuffer());
    }
  }
}


BaseJoint::JointPointer JointController::resolveJoint(const std::string& i_movementDirection) {
  if (i_movementDirection == "CCW" or i_movementDirection == "CW") {
    return getJoint(Rotational);
  }
  if (i_movementDirection == "IN" or i_movementDirection == "OUT") {
    return getJoint(Translational);
  }
  LOG_ERROR("Could not resolve movement direction: " << i_movementDirection);
}


BaseJoint::JointPointer JointController::resolveJoint(const MovementType& i_movementType) {
  return getJoint(i_movementType);
}
