// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <PinState.h>


PinState::PinState()
    : m_pinVector{},
      m_numericValue(0) {
  resetPinState();
}


PinState::PinState(const PinVector& i_pins)
    : m_pinVector(i_pins),
      m_numericValue(0) {
  setPins(i_pins);
}


void PinState::resetPinState() {
  std::vector<int> zero {};
  m_pinVector.swap(zero);
  m_numericValue = 0;
}


void PinState::setPins(const PinVector& i_pins) {
  // Reset the pin state and put pin defined in i_pins to default state
  resetPinState();
  for (auto itr = i_pins.begin();
       itr != i_pins.end();
       itr++) {
    m_pinVector.push_back(*itr);
    LOG_INFO("Setting: " << (*itr) << " to: " << DEFAULT_STATE);
    if (DEFAULT_STATE)
      m_numericValue += getNumericPinValue(*itr);    
  }
}

void PinState::setPinsToDefault() {
  const PinVector pinVector(m_pinVector);
  setPins(pinVector);
}


void PinState::update(const int& i_pinNumber,
                      const int& i_pinValue) {
  if (std::find(
          m_pinVector.begin(),
          m_pinVector.end(),
          i_pinNumber) != m_pinVector.end()) {
    // if the pin already has been defined
    if (getPinState(i_pinNumber) != i_pinValue) {
      // if the pin value is different to what it was
      if (i_pinValue)
        m_numericValue += getNumericPinValue(i_pinNumber);
      else
        m_numericValue -= getNumericPinValue(i_pinNumber);
    }
  } else {
    m_pinVector.push_back(i_pinNumber);
    if (i_pinValue)
      m_numericValue += getNumericPinValue(i_pinNumber);
  }
}


void PinState::update(const PinState& i_pinState) {
  for (auto itr = i_pinState.m_pinVector.begin();
      itr != i_pinState.m_pinVector.end();
      itr++) {
    update(*itr, i_pinState.getPinState(*itr));
  }
}


int PinState::getNumericPinValue(const int& i_pinNumber) const {
  return 1 << i_pinNumber;
}


int PinState::getPinState(const int& i_pinNumber) const {
  PinVector::const_iterator itr = std::find(m_pinVector.begin(),
                                            m_pinVector.end(),
                                            i_pinNumber);
  if (itr == m_pinVector.end()) {
    LOG_ERROR("Pin not available!");
  }
  return (m_numericValue & getNumericPinValue(i_pinNumber)) >> i_pinNumber;
}


void PinState::displayPinState() const {
  LOG_DEBUG("Display Pin State: ");
  for (auto itr = m_pinVector.begin();
       itr != m_pinVector.end();
       itr++) {
    LOG_DEBUG("Pin: " << (*itr) << " is in state: " << getPinState(*itr));
  }
}


bool PinState::operator==(const PinState& rhs) const {
  if(rhs.getNumericValue() != getNumericValue())
    return false;
  std::vector<int> lhsPinVector = getPinVector();
  std::vector<int> rhsPinVector = rhs.getPinVector();
  std::sort(lhsPinVector.begin(),
            lhsPinVector.end());
  std::sort(rhsPinVector.begin(),
            rhsPinVector.end());
  if (lhsPinVector!= rhsPinVector)
    return false;

  return true;
}
