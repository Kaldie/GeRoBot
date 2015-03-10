// Copyright [2014] Ruud Cools

#include <macroHeader.h>
#include "./BaseMotor.h"

// Constructors
BaseMotor::BaseMotor()
    : BaseMotor({1, 2, 3, 4},
                "CW",
                false)
{}

BaseMotor::BaseMotor(const PinVector& i_pins)
    : BaseMotor(i_pins,
                "CW",
                false)
{}


BaseMotor::BaseMotor(const PinVector& i_pins,
                     const std::string& i_defaultDirection)
    : BaseMotor(i_pins,
                i_defaultDirection,
                false)
{}


BaseMotor::BaseMotor(const PinVector& i_pins,
                     const std::string& i_defaultDirection,
                     const bool i_holdMotor)
    : m_defaultDirection(i_defaultDirection),
      m_holdMotor(i_holdMotor) {
  setPins(i_pins);
}


void BaseMotor::setPins(const PinVector& i_pins) {
    m_currentPinState.setPins(i_pins);
}

void BaseMotor::displayPinState() const {
    displayPinState(m_currentPinState);
}

void BaseMotor::displayPinState(const PinState& i_pinState) const {
    i_pinState.displayPinState();
}


void BaseMotor::displayPinStateSequence(
    const PinStateSequence& i_pinStateSequence) const {
  i_pinStateSequence.displaySequence();
}
