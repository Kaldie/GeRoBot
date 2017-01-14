// Copyright [2015] Ruud Cools

#ifndef ARDUINO_UNITTEST_ARDUINOMOTORDRIVERTEST_H_
#define ARDUINO_UNITTEST_ARDUINOMOTORDRIVERTEST_H_

#include <macroHeader.h>
#include <RobotIO.h>
#include <ArduinoMotorDriver.h>
#include "../ArduinoTool.h"


class ArduinoMotorDriverTest : public CxxTest::TestSuite {
 public:
  void testCreation() {
    ArduinoMotorDriver driver = ArduinoMotorDriver();
    ArduinoMotorDriver driver2 = ArduinoMotorDriver("Port");
    TS_ASSERT_EQUALS(driver.getSerialRegularExpresion(), "");
    TS_ASSERT_EQUALS(driver2.getSerialRegularExpresion(), "Port");
  }

  void testBenchmark() {
    ArduinoMotorDriver driver = ArduinoMotorDriver("/dev/ttyUSB*");
    driver.benchmarkSD(3000);
  }

};



#endif  // ARDUINO_UNITTEST_ARDUINOMOTORDRIVERTEST_H_
