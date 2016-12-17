#include <macroHeader.h>
#include <EndStop.h>
#include <BaseJoint.h>
#include <Point2D.h>


EndStop::EndStop()
  : EndStop(0.0, 0, false, "None") {
}


EndStop::EndStop(const traceType& i_position)
  : EndStop(i_position, 0, false, "None") {
}


EndStop::EndStop(const traceType& i_position,
		 const int& i_pinNumber)
  : EndStop(i_position, i_pinNumber, false, "None") {
}


EndStop::EndStop(const traceType& i_position,
		 const int& i_pinNumber,
		 const bool& i_activationState,
		 const std::string& i_activationDirection)
  : m_position(i_position),
    m_pinNumber(i_pinNumber),
    m_activationState(i_activationState),
    m_activationDirection(i_activationDirection){
}


bool EndStop::operator==(const EndStop& i_rhs) const {
  if (i_rhs.m_position != m_position) {
    LOG_DEBUG("Position is not equal: " << m_position << " != " <<
	      i_rhs.m_position);
    return false;
  }
  if (i_rhs.m_pinNumber != m_pinNumber) {
    LOG_DEBUG("PinNumber is not equal: " << m_pinNumber << " != " <<
	      i_rhs.m_pinNumber);
    return false;
  }
  if (i_rhs.m_activationState != m_activationState) {
    LOG_DEBUG("Activation state is not equal: " << m_activationState
	      << " != " << i_rhs.m_activationState);
    return false;
  }
  if (i_rhs.m_activationDirection.compare(m_activationDirection) != 0) {
    LOG_DEBUG("Acivation direction is off: " << m_activationDirection
	      << " != " << i_rhs.m_activationDirection);
    return false;
  }
  return true;      
}
