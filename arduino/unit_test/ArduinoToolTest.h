// Copyright [2015] Ruud Cools

#ifndef ARDUINO_UNITTEST_ARDUINOTOOLTEST_H_
#define ARDUINO_UNITTEST_ARDUINOTOOLTEST_H_

#include <macroHeader.h>
#include <RobotIO.h>
#include <JointController.h>
#include "../ArduinoTool.h"


class ArduinoToolTest : public CxxTest::TestSuite {
 public:
  void testCreation() {
    RobotIO robotIO("defaultRobot.xml");
    robotIO.build();
    ArduinoTool tool;
    tool.changeState(true);
    ArduinoTool tool1(robotIO.getRobotPointer(),
		      10, 0, false);
    tool.changeState(true);
    ArduinoTool tool2(robotIO.getRobotPointer(),
		      10, 10, 0, 0, false);
    tool2.changeState(true);
  }


  void testChangeState1() {
    RobotIO robotIO("defaultRobot.xml");
    robotIO.build();
    Robot::RobotPointer robot(robotIO.getRobotPointer());
    SequenceVector* sequenceVector =
      robot->getJointController()->getSequenceVectorPointer();
    ArduinoTool tool(robot, 1050, 4, false);

    TS_ASSERT_EQUALS(sequenceVector->numberOfSequences(), 1);
    TS_ASSERT_EQUALS(sequenceVector->numberOfSteps(), 0);
    TS_ASSERT_THROWS(tool.changeState(true), std::runtime_error);
    tool.setPin(0);
    tool.changeState(true);
    TS_ASSERT_EQUALS(sequenceVector->
		     getSequenceVector()[1].getSpeed(), 200);
    TS_ASSERT_EQUALS(sequenceVector->
		     getSequenceVector()[1].getNumberOfRepetitions(), 2100);
    for (const auto& stateSequence : *sequenceVector) {
      stateSequence.displaySequence();
    }
  }


  void testChangeState2() {
    RobotIO robotIO("defaultRobot.xml");
    robotIO.build();
    Robot::RobotPointer robot(robotIO.getRobotPointer());
    SequenceVector* sequenceVector =
      robot->getJointController()->getSequenceVectorPointer();
    ArduinoTool tool(robot, 100, 100, 10, 0, false);
    tool.changeState(true);
    for (const auto stateSequence : *sequenceVector) {
      stateSequence.displaySequence();
    }
    TS_ASSERT_EQUALS(sequenceVector->
		     getSequenceVector()[1].getNumberOfRepetitions(), 200);
    TS_ASSERT_EQUALS(sequenceVector->
		     getSequenceVector()[1].getSpeed(), 200);
    TS_ASSERT_DELTA(105, 1/300.0 * 31500, 0.1);
  }


  void testChangeState3() {
    RobotIO robotIO("defaultRobot.xml");
    robotIO.build();
    Robot::RobotPointer robot(robotIO.getRobotPointer());
    SequenceVector* sequenceVector =
      robot->getJointController()->getSequenceVectorPointer();
    ArduinoTool tool(robot, 100, 10, 201, 0, false);
    tool.changeState(true);
    tool.changeState(false);
    tool.changeState(true);
    TS_ASSERT_EQUALS(sequenceVector->getSequenceVector()[3].getNumberOfRepetitions(), 1);
    TS_ASSERT_EQUALS(sequenceVector->getSequenceVector()[3].getSpeed(), 200);

    tool.setCoolDownTime(10);
    tool.setSwitchOffTime(100);
    tool.changeState(false);
    tool.changeState(true);
    TS_ASSERT_EQUALS(sequenceVector->numberOfSequences(), 6);
    TS_ASSERT_EQUALS(sequenceVector->getSequenceVector()[5].getNumberOfRepetitions(), 200);
    TS_ASSERT_EQUALS(sequenceVector->getSequenceVector()[5].getSpeed(), 200);
    for (const auto stateSequence : *sequenceVector) {
      stateSequence.displaySequence();
    }
  }

};



#endif  // ARDUINO_UNITTEST_ARDUINOTOOLTEST_H_
