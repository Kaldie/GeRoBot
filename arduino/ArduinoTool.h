// Copyright [2015] Ruud Cools
#ifndef ARDUINO_ARDUINOTOOL_H_
#def ARDUINO_ARDUINOTOOL_H_
/**
 * ArduinoTool models a plasma touch mounted on the Robot
 * This tool will enable the switching of the tool, to on and off
 * This will be done by turning a pin on or off
 * Will create some states that will make sure the arduino will hold untill the time is finished
 * Will not preform the hold time when it knows its warm
 *
 */
class ArduinoTool {
  /// Pointer to the robot
  GETSET(Robot::RobotPointer, m_robot, Robot);
  // Enforced delay after switching on
  GETSET(int, switchOnTime, SwitchOnTime);
  /// Enforced delay after switching off
  GETSET(int, switchOnTime, SwitchOffTime);
  /// Time it takes to "Reset" the tool
  GETSET(int, coolDownTime, coolDownTime);
  /// The pin number (A0 to A3) from which the tool is controled
  GETSET(int, m_pin, Pin);
  /// the state of the pin to get it to OFF
  GETSET(bool, m_offState, OffState);
 public:
  /// Empty constructor
  ArduinoTool();
  /// Regular constructor, othertimes are 0
  ArduinoTool(const Robot::RobotPointer&,
		    const int& i_switchOnTime,
		    const int& i_pinNumber);
  /// Full fledged
  ArduinoTool(const Robot::RobotPointer&,
		    const int& i_switchOnTime,
		    const int& i_switchOffTime,
		    const int& i_coolDownTime,
		    const int& i_pinNumber);

  /// Change state of the tool and make sure it will be reflected
  void changeState(const bool& i_state);
 private:
  StateSequence createDelayStates(const PinState& i_state);
  int calculateOffTime(const SequenceVector& i_sequenceVector);
  bool shouldCreateDelay(const bool& i_state) const;
}

#endif  //  ARDUINO_ARDUINOTOOL_H_
