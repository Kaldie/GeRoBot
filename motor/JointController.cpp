// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <JointController.h>
#include <ArduinoMotorDriver.h>
#include <PinStateSequence.h>
#include <BaseJoint.h>
#include <BaseMotor.h>


bool JointController::validateJoint(const JointPointer& i_baseJoint) const {
  // pins should not be other then 2 t/m 7
  PinVector pins = i_baseJoint->getMotor()->getPins();
  std::sort(pins.begin(), pins.end());

  int prevsPin = pins[0];
  for (PinVector::const_iterator itr = pins.begin();
      itr != pins.end();
      itr++) {
    // Pin should be between 2 and 7...make sense for arduino only!
    if (((*itr)>7) | ((*itr) <2)) {
      LOG_INFO("Pin: " << (*itr) << " is not within range!!");
      return false;
    }

    if (prevsPin-(*itr) > 1) {
      LOG_INFO("distance between pins is more then 1: " << prevsPin);
      LOG_INFO(*itr);
      return false;
    }

    prevsPin = (*itr);
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
  PinStateSequenceVector emptyPinStateSequenceVector;
  PinStateSequence cleanPinStateSequence;

  for (JointPointerVector::iterator itr = m_jointPointerVector.begin();
      itr != m_jointPointerVector.end();
      itr++) {
    if ( !cleanPinStateSequence.
         addToSequence((*itr)->getMotor()->getCurrentPinState())) {
      LOG_ERROR("Could not create empty PinState Sequence");
    }
    cleanPinStateSequence.setNumberOfRepetitions(0);
#ifndef NDEBUG
    cleanPinStateSequence.displaySequence();
#endif
  }

  emptyPinStateSequenceVector.push_back(cleanPinStateSequence);
  m_pinStateSequenceVector.swap(emptyPinStateSequenceVector);
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


void JointController::appendPinStateSequence(
    PinStateSequence& i_newPinStateSequence,
    const bool& i_merge) {

  if (i_newPinStateSequence.isEmpty())
    return;

  if (m_pinStateSequenceVector.size() == 0) {
    m_pinStateSequenceVector.push_back(i_newPinStateSequence);
    return;
  }

  if (m_pinStateSequenceVector.back().addToSequence(i_newPinStateSequence))
    return;

  if (i_merge) {
    PinStateSequence::
        mergePinStateSequences(&(m_pinStateSequenceVector.back()),
                               &i_newPinStateSequence);
  }
  m_pinStateSequenceVector.push_back(i_newPinStateSequence);
}


void JointController::normaliseSequenceVector() {
  if (m_pinStateSequenceVector.size() < 2)
    return;

  for (auto currentSequence = m_pinStateSequenceVector.begin() + 1;
       currentSequence !=  m_pinStateSequenceVector.end();
       currentSequence++) {
    currentSequence->setStateForSequence(
        (currentSequence-1)->getPinStateVector().back(),
        true, false);
  }
}


void JointController::moveStep(JointPointer& i_jointPointer,
                               const std::string& i_directionString,
                               const bool& i_appendToPreviousStep) {
  if (!hasJoint(i_jointPointer)) {
    LOG_ERROR("Joint not avalaible in this controller!");
  }

  std::string motorDirection =
      i_jointPointer->convertDirection(i_directionString);

  PinStateSequence pinStateSequence;
  i_jointPointer->getMotor()->moveStep(motorDirection, pinStateSequence);

  if (i_appendToPreviousStep) {
    appendPinStateSequence(pinStateSequence, true);
  } else {
    appendPinStateSequence(pinStateSequence, false);
  }
}


void JointController::moveSteps(JointPointer& i_jointPointer,
                                const std::string& i_directionString,
                                const int& i_numberOfSteps,
                                const bool i_append /* = false */) {
  // Check if the joint controller has this joint
  if (!hasJoint(i_jointPointer))
    LOG_ERROR("Joint not avalaible in this controller!");
  i_jointPointer->getMotor()->moveSteps(i_directionString,
                                        i_numberOfSteps,
                                        m_pinStateSequenceVector);
}


//  Brief: Actuate the robot from the given pin state sequence
void JointController::actuate() {
  normaliseSequenceVector();
  char* messageBuffer(NULL);
  size_t messageSize(0);
  for (PinStateSequenceVector::const_iterator pinStateSequenceIterator
           = m_pinStateSequenceVector.begin();
       pinStateSequenceIterator != m_pinStateSequenceVector.end();
       pinStateSequenceIterator++)
    // Get the integer sequence of this pin state
    pinStateSequenceIterator->createArduinoBuffer(messageBuffer,
                                                  messageSize);
    m_actuator.actuate(messageBuffer, messageSize);
    delete[] messageBuffer;
    messageBuffer = NULL;
  resetPinStateSequence();
}


bool JointController::isNormalisedPinStateSequenceVector() const  {
  PinState previousPinState =
      m_pinStateSequenceVector.front().getPinStateVector()[0];
  // Sequence loop
  for (auto pinStateSequenceIterator = m_pinStateSequenceVector.begin();
       pinStateSequenceIterator != m_pinStateSequenceVector.end();
       pinStateSequenceIterator++) {
    // PinState loop
    for (auto pinStateIterator =
             pinStateSequenceIterator->getPinStateVector().begin();
         pinStateIterator !=
             pinStateSequenceIterator->getPinStateVector().end();
         pinStateIterator++) {
      // Pin loop
      for (auto pinIterator  = previousPinState.getPinVector().begin();
           pinIterator != previousPinState.getPinVector().end();
           pinIterator++) {
        try {
          pinStateIterator->getPinState(*pinIterator);
        } catch(std::runtime_error exception) {
          /*
            PinState pinState = *pinStateIterator;
            pinState.update(*pinIterator,previousPinState.getPinState(*pinIterator));
            pinStateIterator->update(pinState);
          */
          return false;
        }  // end try catch block
      }  // end pin loop
      previousPinState = *pinStateIterator;
    }  // end pin state loop
  }  // end pin state sequence loop
  return true;
}


JointController::JointController()
    :m_actuator(ArduinoMotorDriver("/dev/ttyUSB*"))
{}

JointController::~JointController()
{}
