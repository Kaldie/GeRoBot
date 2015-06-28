// Copyright [2015] Ruud Cools

#ifndef MOTOR_UNIT_TEST_PINSTATESEQUENCEUNITTEST_H_
#define MOTOR_UNIT_TEST_PINSTATESEQUENCEUNITTEST_H_

#include <cxxtest/TestSuite.h>
#include <macroHeader.h>
#include <vector>
#include "./PinStateSequence.h"
#include "./PinState.h"

class PinStateSequenceTestSuite : public CxxTest::TestSuite {
 public:
  void testDefaultCreation(void) {
    LOG_INFO("PinStateSequenceTestSuite::testDefaultCreation");
    PinStateSequence pinStateSequence;
    // test if the integersequence is empty at creation
    TS_ASSERT_EQUALS(pinStateSequence.getIntegerSequence(), std::vector<int>());
  };


  void testCreation(void) {
    LOG_INFO("PinStateSequenceTestSuite::testCreation");
    PinStateVector pinStateVector;
    for (int i = 0;
         i < 2;
         i++) {
      PinState pinState(PinVector{2, 3, 4});

      int aPinState;
      if (i % 2 == 0)
        aPinState = !DEFAULT_STATE;
      else
        aPinState = DEFAULT_STATE;

      for (auto pin = pinState.getPinVector().begin();
           pin != pinState.getPinVector().end();
           pin++) {
        pinState.update(*pin, aPinState);
      }

      pinStateVector.push_back(pinState);
    }

    PinStateSequence pinStateSequence(5, 10, pinStateVector);
    // test if the integersequence is empty at creation
    std::vector<int> integerSequence = pinStateSequence.getIntegerSequence();

    std::vector<int> benchmarkVector {!DEFAULT_STATE*28,
          DEFAULT_STATE*28};
    for (uint i = 0;
         i < integerSequence.size();
         i++) {
      TS_ASSERT_EQUALS(integerSequence[i],
                       benchmarkVector[i]);
    }
  }


  void testDefectingCreation() {
    LOG_INFO("PinStateSequenceTestSuite::testDefectingCreation");
    PinStateVector pinStateVectorDiffPinNumber {
      PinState(PinVector {1, 2, 3}),
      PinState(PinVector {1, 2, 3, 4})};
    TS_ASSERT_THROWS(PinStateSequence pinStateSequence
                     (5, 10, pinStateVectorDiffPinNumber),
                     std::runtime_error);
    PinStateVector pinStateVectorDiffPins {
      PinState(PinVector {1, 2, 3}),
          PinState(PinVector {1, 2, 4})};
    TS_ASSERT_THROWS(PinStateSequence pinStateSequence
                     (5, 10, pinStateVectorDiffPins),
                     std::runtime_error);
  }


  void testAddPinStateToSequence() {
    LOG_INFO("PinStateSequenceTestSuite::testAddSequence");
    PinStateSequence pinStateSequence;
    PinState pinState1(PinVector{1,2,3});
    TS_ASSERT(pinStateSequence.addToSequence(pinState1));
    PinState pinState2(PinVector{4,5,6});
    TS_ASSERT(pinStateSequence.addToSequence(pinState2));
    PinState pinState3(PinVector{1,2,3});
    TS_ASSERT(!pinStateSequence.addToSequence(pinState3));
    TS_ASSERT_EQUALS(pinStateSequence.getNumberOfRepetitions(), 1);
    TS_ASSERT_EQUALS(pinStateSequence.getPinStateVector().size(), 1);
    TS_ASSERT_EQUALS(pinStateSequence.getIntegerSequence(),
                     std::vector<int>{DEFAULT_STATE*(2+4+8+16+32+64)});
  }


  void testAddPinStateVector() {
    PinStateSequence pinStateSequence;
    PinStateVector pinStateVector{
      PinState(PinVector{1, 2, 3}),
      PinState(PinVector{1, 2, 3})};
    TS_ASSERT(pinStateSequence.addToSequence(pinStateVector));
    TS_ASSERT(pinStateSequence.addToSequence(pinStateVector));
    TS_ASSERT(pinStateSequence.addToSequence(pinStateVector));
    TS_ASSERT_EQUALS(pinStateSequence.getNumberOfRepetitions(), 3);

    pinStateVector.push_back(PinState(PinVector{1, 2, 3, 4}));
    TS_ASSERT(!pinStateSequence.addToSequence(pinStateVector));

    PinStateVector pinStateVector2{
      PinState(PinVector{1, 2, 3}),
      PinState(PinVector{1, 2, 4})};
    TS_ASSERT(!pinStateSequence.addToSequence(pinStateVector2));

    // if this 1 fails, check if DEFAULT_STATE is 1!
    TS_ASSERT_EQUALS(pinStateSequence.getNumberOfRepetitions(), 3);
  }


  void testAddPinStateSequence() {
    LOG_INFO("PinStateSequenceTestSuite::testAddPinStateSequence");
    PinStateVector pinStateVector1{
      PinState(PinVector{1, 2, 3}),
      PinState(PinVector{1, 2, 3})};

    PinStateVector pinStateVector2{
      PinState(PinVector{4, 5, 6}),
      PinState(PinVector{4, 5, 6})};

    PinStateVector pinStateVector3{
      PinState(PinVector{6, 7, 8}),
          PinState(PinVector{6, 7, 8})};
    
    PinStateSequence pinStateSequence1;
    PinStateSequence pinStateSequence2(5, 2, pinStateVector1);
    PinStateSequence pinStateSequence3(5, 3, pinStateVector1);
    PinStateSequence pinStateSequence4(5, 1, pinStateVector2);
    PinStateSequence pinStateSequence5(5, 7, pinStateVector3);

    // Test if an empty sequence cannot be added
    TS_ASSERT(!pinStateSequence1.addToSequence(pinStateSequence1));

    // Test if an sequence can be added to an empty sequence
    TS_ASSERT(pinStateSequence1.addToSequence(pinStateSequence2));

    // Test if a sequence can be added to a non-empty sequence
    TS_ASSERT(pinStateSequence1.addToSequence(pinStateSequence2));

    // check if the number of repititions is calculated correctly
    TS_ASSERT_EQUALS(pinStateSequence1.getNumberOfRepetitions(), 4);

    // Test if an sequence can be added with a different number of repititions
    TS_ASSERT(pinStateSequence1.addToSequence(pinStateSequence3));

    /*
      Test again if the number of repetitions is
      calculated correctly in this case
    */
    TS_ASSERT_EQUALS(pinStateSequence1.getNumberOfRepetitions(), 7);

    /*
      Test if a sequence with none mutual pins
      but a different number of repititions can be added
    */
    TS_ASSERT(!pinStateSequence1.addToSequence(pinStateSequence4));


    /*
      Test if a sequence with none mutual pins
      and equal number of repititions can be added
    */
    pinStateSequence4.setNumberOfRepetitions(7);
    TS_ASSERT(pinStateSequence1.addToSequence(pinStateSequence4));

    // test if a sequence with mutual pins cannot be added
    TS_ASSERT_EQUALS(pinStateSequence1.getNumberOfRepetitions(),
                     pinStateSequence5.getNumberOfRepetitions());
    TS_ASSERT(!pinStateSequence1.addToSequence(pinStateSequence5));
  }


  void testMergeSequenceWithEqualPinStateVector() {
    LOG_INFO("PinStateSequenceTestSuite::" <<
             "testMergeSequenceWithEqualPinStateVector");

    PinVector pinVector{1, 2, 3};
    PinState defaultPinState(pinVector);
    PinState noneDefaultPinState(pinVector);

    for (auto pin = pinVector.begin();
         pin != pinVector.end();
         pin++) {
      noneDefaultPinState.update(*pin, !DEFAULT_STATE);
    }

    PinStateVector pinStateVector1{defaultPinState, noneDefaultPinState};

    PinStateSequence pinStateSequence1(5, 2, pinStateVector1);
    PinStateSequence pinStateSequence2(5, 2, pinStateVector1);

    LOG_DEBUG("Test if it is possible to merge 2 equal states");
    TS_ASSERT(pinStateSequence1.mergePinStateSequence(&pinStateSequence2));

    LOG_DEBUG("check if the merged sequence end up" <<
              "with the correct number of repitions");
    TS_ASSERT_EQUALS(pinStateSequence1.getNumberOfRepetitions(), 4);
    TS_ASSERT(pinStateSequence2.isEmpty());
    TS_ASSERT_EQUALS(pinStateSequence2.getNumberOfRepetitions(), 0);

    pinStateSequence2.addToSequence(pinStateVector1);
    pinStateSequence2.setNumberOfRepetitions(13);

    LOG_DEBUG("Test if it is possible to merge states:"
              "equal pin state sequence" <<
              "none equal number of repititions");
    TS_ASSERT(pinStateSequence1.mergePinStateSequence(&pinStateSequence2));
              }


  void testMergeOfNoneEqualSequence() {
    LOG_INFO("PinStateSequenceTestSuite::testMergeOfNoneEqualSequence");

    // Creating the necessary objects!
    PinVector pinVector1{1, 2, 3};

    PinState defaultPinState1(pinVector1);
    PinState noneDefaultPinState1(pinVector1);

    PinVector pinVector2{4, 5, 6};
    PinState defaultPinState2(pinVector2);
    PinState noneDefaultPinState2(pinVector2);

    TS_ASSERT_EQUALS(pinVector1.size(), pinVector2.size())
    PinVector::iterator pin2 = pinVector2.begin();
    for (auto pin1 = pinVector1.begin();
         pin1 != pinVector1.end();
         pin1++, pin2++) {
      noneDefaultPinState1.update(*pin1, !DEFAULT_STATE);
      noneDefaultPinState2.update(*pin2, !DEFAULT_STATE);
    }

    PinStateVector pinStateVector1{defaultPinState1, noneDefaultPinState1};
    PinStateVector pinStateVector2{noneDefaultPinState1, defaultPinState1};

    PinStateVector pinStateVector3{defaultPinState2, noneDefaultPinState2};
    PinStateVector pinStateVector4{noneDefaultPinState2, defaultPinState2};

    PinStateSequence pinStateSequence1(5, 2, pinStateVector1);
    PinStateSequence pinStateSequence2(5, 5, pinStateVector2);
    PinStateSequence pinStateSequence3(5, 2, pinStateVector3);
    PinStateSequence pinStateSequence4(5, 2, pinStateVector4);

    std::vector<int> sequenceBeforeMerge1 = pinStateSequence1.getIntegerSequence();
    // Done creating the necessary objects!

    /*
      Testing if merging 2 sequence with:
        none mutual pins and
        equal number of repetitions works
    */ 
    TS_ASSERT(pinStateSequence1.mergePinStateSequence(&pinStateSequence3));
    std::vector<int> sequenceAfterMerge1 = pinStateSequence1.getIntegerSequence();

    TS_ASSERT_EQUALS(sequenceBeforeMerge1.size(),
                     sequenceAfterMerge1.size());
    
    auto i = sequenceBeforeMerge1.begin();
    auto j = sequenceAfterMerge1.begin();
    for (;
         i!= sequenceBeforeMerge1.end();
         i++, j++) 
      if (*i != 0 && *j !=0)
        TS_ASSERT_DIFFERS(*i, *j);

    TS_ASSERT_EQUALS(pinStateSequence1.getNumberOfRepetitions(), 2);
    TS_ASSERT_EQUALS(pinStateSequence1.getIntegerSequence().size(), 2);
    TS_ASSERT(pinStateSequence3.isEmpty());

    /*
      Testing if merging 2 sequence with:
        none mutual pins and
        none equal number of repetitions works
    */ 
    std::vector<int> beforeMerge2 = pinStateSequence2.getIntegerSequence();
    std::vector<int> beforeMerge4 = pinStateSequence4.getIntegerSequence();

    TS_ASSERT(pinStateSequence2.mergePinStateSequence(&pinStateSequence4));

    std::vector<int> afterMerge2 = pinStateSequence2.getIntegerSequence();
    std::vector<int> afterMerge4 = pinStateSequence4.getIntegerSequence();

    TS_ASSERT_EQUALS(pinStateSequence2.getNumberOfRepetitions(), 3);
    TS_ASSERT_EQUALS(pinStateSequence4.getNumberOfRepetitions(), 2);
    TS_ASSERT_EQUALS(beforeMerge2, afterMerge2);
    TS_ASSERT_DIFFERS(beforeMerge4, afterMerge4);

    TS_ASSERT_EQUALS(pinStateSequence2.getIntegerSequence().size(), 2);
    TS_ASSERT_EQUALS(pinStateSequence4.getIntegerSequence().size(), 2);
    TS_ASSERT(!pinStateSequence4.isEmpty());

    // check if they cannot be merged for a second time!
    TS_ASSERT(!pinStateSequence2.mergePinStateSequence(&pinStateSequence4));
  }

  void testSequenceNormalisation() {
    LOG_INFO("testSequenceNormalisation");
    PinState pinState1({1, 2, 3});
    pinState1.update(1, 0);
    pinState1.update(2, 1);
    pinState1.update(3, 0);

    PinState pinState2({4, 5, 6});
    pinState2.update(4, 1);
    pinState2.update(5, 0);
    pinState2.update(6, 1);

    PinState pinState3({1, 2, 3});
    pinState3.update(1, 1);
    pinState3.update(2, 0);
    pinState3.update(3, 1);

    PinState pinState4({4, 5, 6});
    pinState4.update(4, 0);
    pinState4.update(5, 1);
    pinState4.update(6, 0);

    PinStateVector stateVector{pinState2,pinState3,pinState2};

    PinStateSequence sequence;
    sequence.addToSequence(stateVector);
    TS_ASSERT_EQUALS(sequence.getIntegerSequence(),
                     std::vector<int>({80, 10, 80}));
    
    sequence.setStateForSequence(pinState1, true);
    TS_ASSERT_EQUALS(sequence.getIntegerSequence(),
                     std::vector<int>({84, 10, 90}));
    sequence.setStateForSequence(pinState4, true);
    TS_ASSERT_EQUALS(sequence.getIntegerSequence(),
                     std::vector<int>({84, 90, 90}));
  }

  
  void testPinStateSequenceMessage() {
    PinStateSequence pinStateSequence;
    pinStateSequence.setSpeed(3);
    PinStateVector pinStateVector{
      PinState(PinVector{1, 2, 3}),
      PinState(PinVector{1, 2, 3})};
    for (auto i = 0;
         i < 5;
         i++)
      pinStateSequence.addToSequence(pinStateVector);

    std::vector<int> lala = pinStateSequence.createArduinoBuffer();
    pinStateSequence.displaySequence();
    TS_ASSERT_EQUALS(std::vector<int>({16, 3, 5, 14, 14}), lala);
  }


  void testCondenseSequence() {
    PinStateVector vector;
    PinStateSequence sequence;
    PinState pinState({1, 2, 3});
    for (int i = 0;
         i <= 7;
         i++) {
      pinState.update(1, i%2);
      vector.push_back(pinState);
    }
    sequence.addToSequence(vector);
    TS_ASSERT(sequence.condenseSequence());
    TS_ASSERT_EQUALS(sequence.getNumberOfRepetitions(), 4);
    TS_ASSERT_EQUALS(sequence.getPinStateVector().size(), 2);

    sequence = PinStateSequence();

    pinState.update(1, 0);
    vector.push_back(pinState);
    sequence.addToSequence(vector);
    TS_ASSERT(!sequence.condenseSequence());
  }
};
#endif  // MOTOR_UNIT_TEST_PINSTATESEQUENCEUNITTEST_H_
