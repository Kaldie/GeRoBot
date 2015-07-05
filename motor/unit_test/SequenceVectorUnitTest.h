// Copyright [2015] Ruud Cools
#ifndef MOTOR_UNIT_TEST_SEQUENCEVECTORUNITTEST_H_
#define MOTOR_UNIT_TEST_SEQUENCEVECTORUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <SequenceVector.h>
#include <StateSequence.h>
#include <PinState.h>


class SequenceVectorUnitTest : public CxxTest::TestSuite {
 public:
  SequenceVector sequenceVector;
  StateSequence stateSequence;
  PinState pinState1;
  PinState pinState2;
  
  void setUp() {
    SequenceVector sequenceVector;
    StateSequence stateSequence;
    pinState1 = PinState(std::vector<int>({4,5,6}));
    pinState2 = PinState(std::vector<int>({1,2,3}));
  }
    
  void testCreation() {
    // Default creations no settings have to be set!
    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 0);
    TS_ASSERT_EQUALS(
        sequenceVector.getSequenceVector().size(), 0);
  }


  void testAppendPinState() {
    pinState1.displayPinState();
    TS_ASSERT(sequenceVector.addToSequence(pinState1));
    TS_ASSERT(sequenceVector.addToSequence(pinState2));
    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 1);
    TS_ASSERT(!sequenceVector.addToSequence(pinState1));

    PinState pinState(pinState1);
    pinState.update(pinState2);
    PinState resultingPinState =
        *sequenceVector.begin()->getPinStateVector().begin();

    TS_ASSERT_EQUALS(resultingPinState,
                     pinState);
  }


  void testCount() {
    sequenceVector.addToSequence(pinState1);
    sequenceVector.addToSequence(pinState2);
    pinState1.update(pinState2);
    for (auto pin = pinState1.getPinVector().begin();
         pin != pinState1.getPinVector().end();
         pin++) {
      pinState1.update(*pin, !pinState1.getPinState(*pin));
    }
    //    sequenceVector.begin()->setNumberOfRepetitions(10);
    
  }

  void testIsNormalised() {
  }

  void testNormalise() {
  }

  void testCondense() {
  }


  void testReduce() {
  }

  
};

#endif  // MOTOR_UNIT_TEST_SEQUENCEVECTORUNITTEST_H_
