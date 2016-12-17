// Copyright [2016] Ruud Cools

#ifndef MOTOR_IO_UNITTEST_ENDSTOPIOUNITTEST_H_
#define MOTOR_IO_UNITTEST_ENDSTOPIOUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <EndStopIO.h>
#include <RobotIO.h>

class EndStopIOUnitTest : public CxxTest::TestSuite {
 public:
  pugi::xml_node m_endStopNode;
  std::shared_ptr<RobotIO> m_robotIO;
  
  void setUp() {
    m_robotIO = std::make_shared<RobotIO>("EndStop_test.xml");
    m_endStopNode = m_robotIO->getNodeFromPath("./ROBOT/ENDSTOP");
  }

  void testBuild() {
    EndStopIO endStopIO(m_endStopNode);
    endStopIO.build();
    EndStop endStop = endStopIO.getEndStop();
    TS_ASSERT_EQUALS(endStop.getPosition(), 324);
    TS_ASSERT_EQUALS(endStop.getPinNumber(), 4);
    TS_ASSERT_EQUALS(endStop.getActivationState(), false);
    TS_ASSERT_EQUALS(endStop.getActivationDirection(), "CCW");
  }
    
  void testBitch() {
    EndStop endStop(3.1415, 2, true, "IN");
    EndStopIO endStopIO(m_endStopNode);
    TS_ASSERT(endStopIO.update(endStop));
    endStopIO.build();
    TS_ASSERT_EQUALS(endStopIO.getEndStop(), endStop);
  }
};

#endif
