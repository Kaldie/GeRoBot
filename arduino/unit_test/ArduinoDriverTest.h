// Copyright [2015] Ruud Cools

#ifndef ARDUINO_UNITTEST_ARDUINODRIVERTEST_H_
#define ARDUINO_UNITTEST_ARDUINODRIVERTEST_H_H_

#include <macroHeader.h>
#include <RobotIO.h>
#include <ArduinoMotorDriver.h>


class ArduinoDriverTest : public CxxTest::TestSuite {
 public:
  void testCreation() {
    return;
  }

  void testBenchmark() {
    ArduinoMotorDriver driver = ArduinoMotorDriver("/dev/ttyUSB*");
    //    driver.benchmarkSD(100);
  }

  void testFromRobot() {
    Robot::RobotPointer robot = RobotIO("defaultRobot.xml").buildRobot();
    robot->prepareSteps("IN", 1);
    robot->prepareSteps("OUT", 2);
    robot->prepareSteps("CCW", 2000);
    robot->actuate();
    
  }
};




#endif   // Arduino_tooltest
