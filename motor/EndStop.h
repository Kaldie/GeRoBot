// copyright [2015] Ruud Cools

#ifndef MOTOR_ENDSTOP_H_
#define MOTOR_ENDSTOP_H_

class Point2D;

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
  GETSET(std::shared_ptr<BaseJoint>, m_joint, Joint);
  GETSET(traceType, m_position, Position);
  GETSET(int, m_pinNumber, PinNumber);

 public:
  EndStop();

  EndStop(const std::shared_ptr<BaseJoint>& i_joint,
	  const traceType& i_position);
  
  EndStop(const std::shared_ptr<BaseJoint>& i_joint,
	  const traceType& i_position,
	  const int& i_pinNumber);
};

#endif  // MOTOR_ENDSTOP_H_
