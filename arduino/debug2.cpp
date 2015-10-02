// Copyright [2014] Ruud Cools

#include <chrono>
#include <thread>
#include <macroHeader.h>
#include <PinState.h>
#include "./ArduinoSerialConnection.h"
#include "./ArduinoMotorDriver.h"
#include <ByteBuffer.h>

int main() {
 using namespace std::chrono; // nanoseconds, system_clock, seconds
 using namespace std::this_thread;
 ArduinoMotorDriver driver("/dev/ttyUSB*");
//sleep_until(system_clock::now() + seconds(15));
 driver.benchmarkSD(116);
 // driver.sendTestBit();
 // driver.actuate();
 // driver.actuate();
//sleep_until(system_clock::now() + seconds(15));
// driver.benchmarkSD(20);
}
