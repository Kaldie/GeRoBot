// Copyright [2014] Ruud Cools

#include <macroHeader.h>
#include <PinStateSequence.h>
#include <PinState.h>
#include "./ArduinoSerialConnection.h"
#include "./ArduinoMotorDriver.h"

int main() {
  ArduinoMotorDriver driver("/dev/ttyUSB*");
  driver.sendTestBit();
}

