// Copyright [2015] Ruud Cools
#ifndef ARDUINO_ARDUINOTOOL_H_
#define ARDUINO_ARDUINOTOOL_H_

#include <Robot.h>
#include <StateSequence.h>
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
  GETSET(int, m_switchOnTime, SwitchOnTime);
  /// Enforced delay after switching off
  GETSET(int, m_switchOffTime, SwitchOffTime);
  /// Time it takes to "Reset" the tool
  GETSET(int, m_coolDownTime, CoolDownTime);
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
              const int& i_pinNumber,
              const bool& i_offState);
  /// Full fledged
  ArduinoTool(const Robot::RobotPointer&,
              const int& i_switchOnTime,
              const int& i_switchOffTime,
              const int& i_coolDownTime,
              const int& i_pinNumber,
              const bool& i_offState);

  /// Change state of the tool and make sure it will be reflected
  void changeState(const bool& i_state);
 private:
  StateSequence createStateSequence(const PinState& i_state,
                                    const int& i_delay);
  int calculateOffTime() const;
  int getDelay(const bool& i_state) const;
};

#endif  //  ARDUINO_ARDUINOTOOL_H_
