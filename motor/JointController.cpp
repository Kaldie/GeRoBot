// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <JointController.h>
#include <ArduinoMotorDriver.h>
#include <StateSequence.h>
#include <SequenceVector.h>
#include <BaseJoint.h>
#include <BaseMotor.h>

JointController::JointController()
    :m_actuator(ArduinoMotorDriver("/dev/ttyUSB*")) {
  LOG_DEBUG("yay2");
}

JointController::~JointController()
{}

bool JointController::validateJoint(const JointPointer& i_baseJoint) const {
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
    const JointPointerVector& i_jointVector) const {

  // determines if the pinIterator-> is able to grow into a good JointVector
  for (JointPointerVector::const_iterator itr = i_jointVector.begin();
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


bool JointController::addJoint(const JointPointer& i_joint) {
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
  LOG_INFO("Current size: " << m_jointPointerVector.size());
  resetPinStateSequence();
  return true;
}


JointPointerVector JointController::getJoints(
    const MovementType &i_movementType) {

  JointPointerVector requiredJoints;
  for (JointPointerVector::iterator itr = m_jointPointerVector.begin();
      itr != m_jointPointerVector.end();
      itr++) {
    if ((*itr)->getMovementType() == i_movementType) {
      requiredJoints.push_back((*itr));
    }
  }
  return requiredJoints;
}


JointPointer& JointController::getJoint(const MovementType &i_movementType) {
  if (m_jointPointerVector.size() < 1)
    LOG_ERROR("No joints defined yet");

  if (m_jointPointerVector.size() == 1)
    if (m_jointPointerVector[0]->getMovementType() == i_movementType)
      return m_jointPointerVector[0];

  if (m_jointPointerVector.size() == 2)
    for (JointPointerVector::iterator itr = m_jointPointerVector.begin();
         itr != m_jointPointerVector.end();
         itr++) {
      if ((*itr)->getMovementType() == i_movementType) {
        return *itr;
      }
    }
  LOG_ERROR("Could not find joint with the correct movement type!");
}


void JointController::resetPinStateSequence() {
  m_sequenceVector.clean();
}


bool JointController::hasJoint(const JointPointer& i_jointPointer) const {
  for (JointPointerVector::const_iterator itr = m_jointPointerVector.begin();
      itr != m_jointPointerVector.end();
      itr++) {
    if (i_jointPointer == (*itr))
      return true;
  }
  return false;
}


void JointController::moveStep(JointPointer& i_jointPointer,
                               const std::string& i_directionString,
                               const bool& i_appendToPreviousStep) {
  if (!hasJoint(i_jointPointer)) {
    LOG_ERROR("Joint not avalaible in this controller!");
  }

  std::string motorDirection =
      i_jointPointer->convertDirection(i_directionString);

  StateSequence stateSequence;
  i_jointPointer->getMotor()->moveStep(motorDirection, stateSequence);

  if (i_appendToPreviousStep) {
    m_sequenceVector.appendStateSequence(stateSequence, true);
  } else {
    m_sequenceVector.appendStateSequence(stateSequence, false);
  }
}


void JointController::moveSteps(JointPointer& i_jointPointer,
                                const std::string& i_directionString,
                                const int& i_numberOfSteps,
                                const bool i_append /* = false */) {
  // Check if the joint controller has this joint
  if (!hasJoint(i_jointPointer))
    LOG_ERROR("Joint not avalaible in this controller!");
  // TODO(Ruud): change this so it works with sequence vector
  i_jointPointer->getMotor()->moveSteps(i_directionString,
                                        i_numberOfSteps,
                                        m_sequenceVector);
}


//  Brief: Actuate the robot from the given pin state sequence
void JointController::actuate() {
  m_sequenceVector.normalise();
  for (PinStateSequenceVector::const_iterator pinStateSequenceIterator
           = m_sequenceVector.begin();
       pinStateSequenceIterator != m_sequenceVector.end();
       pinStateSequenceIterator++) {
#ifdef NDEBUG
    pinStateSequenceIterator->displaySequence();
#endif  /// NDEBUG statement that shows the sequences
    /// Get the integer sequence of this pin state
    if (pinStateSequenceIterator->getNumberOfRepetitions() > 0) {
      m_actuator.upload(pinStateSequenceIterator->createArduinoBuffer());
    }
#ifdef DEBUG
    auto x = pinStateSequenceIterator->createArduinoBuffer();
    for (auto i = x.begin();
         i != x.end();
         i++)
      LOG_DEBUG(*i);
#endif  /// Shows the actual message passed though the arduino driver
  }
  m_actuator.actuate();
  resetPinStateSequence();
}
