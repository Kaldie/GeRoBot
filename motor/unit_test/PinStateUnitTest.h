// MyTestSuite1.h

#ifndef MOTOR_UNIT_TEST_PINSTATEUNITTEST_H_
#define MOTOR_UNIT_TEST_PINSTATEUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <vector>
#include "./PinState.h"

class PinStateTestSuite : public CxxTest::TestSuite {
 public:
    void testCreation(void) {
      LOG_INFO("PinStateTestSuite::testCreation");
      PinState testPinState;
      int numericValue = testPinState.getNumericValue();
      TS_ASSERT(numericValue == 0);

      std::vector<int> pinVector{2, 3, 4};
      PinState testPinState2(pinVector);
      TS_ASSERT(testPinState2.getNumericValue() ==
                DEFAULT_STATE*4+
                DEFAULT_STATE*8+
                DEFAULT_STATE*16);

      PinState testPinState3(pinVector, false);
      TS_ASSERT(testPinState2.getNumericValue() == 0);
    }


    void testUpdate(void) {
      LOG_INFO("PinStateTestSuite::test Update");
      std::vector<int> pinVector {2, 3, 4};
      PinState testPinState(pinVector);
      testPinState.update(2, !DEFAULT_STATE);
      TS_ASSERT(testPinState.getNumericValue() ==
                !DEFAULT_STATE*4+
                DEFAULT_STATE*8+
                DEFAULT_STATE*16);
      std::vector<int> pinVector2 {5, 6, 7};
      PinState testPinState2(pinVector2);
      
      testPinState.update(testPinState2);
      TS_ASSERT(testPinState.getNumericValue() ==
                !DEFAULT_STATE*4 +
                DEFAULT_STATE*8 +
                DEFAULT_STATE*16 +
                DEFAULT_STATE*32 +
                DEFAULT_STATE*64 +
                DEFAULT_STATE*128);

      
      for (auto itr = pinVector2.begin();
           itr != pinVector2.end();
           itr++) {
        testPinState.update(*itr, !DEFAULT_STATE);
      }
      
      TS_ASSERT(testPinState.getNumericValue() ==
                !DEFAULT_STATE*4 +
                DEFAULT_STATE*8 +
                DEFAULT_STATE*16 +
                !DEFAULT_STATE*32 +
                !DEFAULT_STATE*64 +
                !DEFAULT_STATE*128);
    }
};

#endif  // MOTOR_UNIT_TEST_PINSTATEUNITTEST_H_
