// copyright [2015] Ruud Cools

#ifndef MOTOR_ENDSTOP_H_
#define MOTOR_ENDSTOP_H_

/**
 * Endstop represents stops we place on the robot to stop the robot for going further
 * The stop belong to a joint, and a position of that joint
 * Secondly, they need to be connected to a pin to the actuator.
 * Currently, this is implemented as a number in a bank of pins (C bank of the arduino currently)
 * Pin numbers can be shared between stops of the same joint.
 * But if there are stop on different joints, 
 * it will be wise to have them assainged to another pin
 */

class EndStop {
  GETSET(traceType, m_position, Position);
  GETSET(int, m_pinNumber, PinNumber);
  GETSET(bool, m_activationState, ActivationState);
  GETSET(std::string, m_activationDirection, ActivationDirection);

 public:
  typedef std::vector<EndStop> EndStopVector;

  /// compare all the properties one by 1
  bool operator==(const EndStop& i_rhs) const;

  EndStop();

  EndStop(const traceType& i_position);
  
  EndStop(const traceType& i_position,
	  const int& i_pinNumber);

  EndStop(const traceType& i_position,
	  const int& i_pinNumber,
	  const bool& i_activationState,
	  const std::string& i_activationDirection);
};

#endif  // MOTOR_ENDSTOP_H_
