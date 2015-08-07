// Copyright [2015] Ruud Cools
#ifndef MOTOR_UNIT_TEST_SEQUENCEVECTORUNITTEST_H_
#define MOTOR_UNIT_TEST_SEQUENCEVECTORUNITTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include <SequenceVector.h>
#include <StateSequence.h>
#include <PinState.h>
#include <Condensor.h>

class SequenceVectorUnitTest : public CxxTest::TestSuite {
 public:
  SequenceVector sequenceVector;
  StateSequence stateSequence;
  PinState pinState1;
  PinState pinState2;

  void setUp() {
    sequenceVector = SequenceVector();
    stateSequence = StateSequence();
    pinState1 = PinState(std::vector<int>({4, 5, 6}));
    pinState2 = PinState(std::vector<int>({1, 2, 3}));
  }


  void testCreation() {
    // Default creations no settings have to be set!
    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 0);
    TS_ASSERT_EQUALS(
        sequenceVector.getSequenceVector().size(), 0);
  }


  void testAppendPinState() {
    LOG_DEBUG("Test: testAppendPinState");
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
    LOG_DEBUG("Test Count");
    sequenceVector.addToSequence(pinState1);
    sequenceVector.addToSequence(pinState2);
    TS_ASSERT_EQUALS(sequenceVector.numberOfSteps(),
                     1);
    pinState1.update(pinState2);
    for (int i = 1;
         i < 5;
         i++) {
      for (auto pin = pinState1.getPinVector().begin();
           pin != pinState1.getPinVector().end();
           pin++) {
        pinState1.update(*pin,
                         !pinState1.getPinState(*pin));
      }
      sequenceVector.addToSequence(pinState1,
                                   true);
      TS_ASSERT_EQUALS(sequenceVector.numberOfSteps(),
                       1+i);
    }
    StateSequence stateSequence2 = *sequenceVector.begin();
    LOG_DEBUG("adding to sequence");
    if (!stateSequence2.addToSequence(PinState({10, 6, 7}), true))
      LOG_ERROR("failed to add!");
    stateSequence2.setNumberOfRepetitions(10);
    sequenceVector.appendStateSequence(stateSequence2, false);
    TS_ASSERT_EQUALS(sequenceVector.numberOfSteps(),
                     5+6*10);
  }

  void testIsNormalised() {
    LOG_DEBUG("testAppendPinState");
    sequenceVector.addToSequence(PinState({1, 2, 3, 4, 5, 6}), true);
    sequenceVector.addToSequence(PinState({/*1,*/ 2, 3, 4, 5, 6}), true);
    TS_ASSERT(!sequenceVector.isNormilized());

    sequenceVector = SequenceVector();
    PinState pinState = PinState({1, 2, 3, 4, 5, 6});
    sequenceVector.addToSequence(pinState);
    for (auto pin = pinState.getPinVector().begin();
         pin != pinState.getPinVector().end();
         pin++) {
      pinState.update(*pin, !pinState.getPinState(*pin));
    }
    sequenceVector.addToSequence(pinState, true);
    TS_ASSERT(sequenceVector.isNormilized());
  }

  
  void testIsNormalisedAfterCondense() {
    LOG_DEBUG("testAppendPinState");
    sequenceVector.addToSequence(PinState({1, 2, 3, 4, 5, 6}), true);
    sequenceVector.addToSequence(PinState({/*1,*/ 2, 3, 4, 5, 6}), true);
    sequenceVector.condenseVector();
    for (auto& i :sequenceVector.getSequenceVector()) {
      i.displaySequence();
    }
    TS_ASSERT(!sequenceVector.isNormilized());

    sequenceVector = SequenceVector();
    PinState pinState = PinState({1, 2, 3, 4, 5, 6});
    sequenceVector.addToSequence(pinState);
    for (auto pin = pinState.getPinVector().begin();
         pin != pinState.getPinVector().end();
         pin++) {
      pinState.update(*pin, !pinState.getPinState(*pin));
    }
    sequenceVector.addToSequence(pinState, true);
    TS_ASSERT(sequenceVector.isNormilized());
  }

  void testNormalise() {
    LOG_DEBUG("Test normalise");
    sequenceVector.addToSequence(PinState({1, 2, 3, 4, 5, 6}), true);
    sequenceVector.addToSequence(PinState({/*1,*/ 2, 3, 4, 5, 6}), true);
    TS_ASSERT(!sequenceVector.isNormilized());
    sequenceVector.normalise();
    TS_ASSERT(sequenceVector.isNormilized());
    TS_ASSERT_EQUALS((*sequenceVector.begin()).getIntegerSequence(),
                     std::vector<int>({126, 126}));
  }


  void testInternalCondense() {
    LOG_DEBUG("Test internal condense!");
    sequenceVector.addToSequence(PinState({1, 2, 3, 4, 5, 6}), true);
    sequenceVector.addToSequence(PinState({1, 2, 3, 4, 5, 6}), true);
    TS_ASSERT_EQUALS(1, sequenceVector.begin()->getNumberOfRepetitions());
    TS_ASSERT_EQUALS(2, sequenceVector.begin()->getIntegerSequence().size());
    // test if  condensing is possible if there is only 1 sequence
    // internally condensing
    LOG_DEBUG("Test internal condensing");
    TS_ASSERT(sequenceVector.condenseVector());
    TS_ASSERT_EQUALS(2, sequenceVector.begin()->getNumberOfRepetitions());
    TS_ASSERT_EQUALS(1, sequenceVector.begin()->getIntegerSequence().size());
  }


  void testMergeCondenseCondense() {
    LOG_DEBUG("Test merge condense");
    StateSequence newStateSequence1(
        3000, 1,
        PinStateVector({PinState({1, 2, 3, 4, 5, 6})}));

    auto pin1 = pinState1.getPinVector().begin();
    auto pin2 = pinState2.getPinVector().begin();
    for (;
         pin1 != pinState1.getPinVector().end() &&
         pin2 != pinState2.getPinVector().end();
         pin1++, pin2++) {
      pinState1.update(*pin1, !pinState1.getPinState(*pin1));
      pinState2.update(*pin2, !pinState2.getPinState(*pin2));
    }

    newStateSequence1.addToSequence(pinState2, true);
    newStateSequence1.addToSequence(pinState1, true);

    sequenceVector = SequenceVector();
    newStateSequence1.setNumberOfRepetitions(23);
    PinStateSequenceVector  pinStatevector(
        {newStateSequence1, newStateSequence1,
         newStateSequence1, newStateSequence1,
         newStateSequence1, newStateSequence1,
         newStateSequence1, newStateSequence1,
         newStateSequence1});
    sequenceVector.setSequenceVector(pinStatevector);
    int oldNumberOfSteps = sequenceVector.numberOfSteps();
    int oldNumberOfSequences = sequenceVector.numberOfSequences();
    TS_ASSERT(sequenceVector.condenseVector());
    TS_ASSERT_EQUALS(sequenceVector.numberOfSteps(), oldNumberOfSteps);
    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), oldNumberOfSequences);

    sequenceVector.setSequenceVector(pinStatevector);
    TS_ASSERT(sequenceVector.condenseVector(true));
    TS_ASSERT_EQUALS(sequenceVector.numberOfSteps(), oldNumberOfSteps);
    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 1);

    for (auto stateSequenceItr = sequenceVector.begin();
         stateSequenceItr != sequenceVector.end();
         stateSequenceItr++) {
      stateSequenceItr->displaySequence();
    }
  }


  void testRecompileCondense_backwardCorrispondense() {
    LOG_INFO("testRecompileCondense_backwardCorrispondense");
    // Simulating 10 cw step
    PinStateVector pinStatevector;
    pinState2.update(1, 0);
    pinState2.update(2, 0);
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinStatevector.push_back(pinState2);
    stateSequence.setPinStateVector(pinStatevector);
    stateSequence.setNumberOfRepetitions(10);

    // simulating a cw step
    StateSequence stateSequence1;
    pinStatevector = PinStateVector();
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(1, 1);
    pinState2.update(2, 1);
    pinState2.update(3, 1);
    pinStatevector.push_back(pinState2);
    stateSequence1.setPinStateVector(pinStatevector);
    stateSequence1.setNumberOfRepetitions(1);

    // Simulating 10 cw steps
    pinStatevector = PinStateVector();
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinStatevector.push_back(pinState2);

    StateSequence stateSequence2;
    stateSequence2.setPinStateVector(pinStatevector);
    stateSequence2.setNumberOfRepetitions(10);

    sequenceVector.setSequenceVector(
        PinStateSequenceVector(
            {stateSequence, stateSequence1, stateSequence2}));
    LOG_DEBUG("Displaying sequences that need to be recompile condensed");
    stateSequence.displaySequence();
    stateSequence1.displaySequence();
    stateSequence2.displaySequence();

    int numberOfOriginalSteps = sequenceVector.numberOfSteps();

    TS_ASSERT(sequenceVector.condenseVector());
    TS_ASSERT_EQUALS(sequenceVector.numberOfSteps(),
                     numberOfOriginalSteps);
    for (auto& i : sequenceVector.getSequenceVector())
      i.displaySequence();
    
    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 4);
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[0].getIntegerSequence(),
                     std::vector<int>({0, 8}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[1].getIntegerSequence(),
                     std::vector<int>({}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[2].getIntegerSequence(),
                     std::vector<int>({14}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[3].getIntegerSequence(),
                     std::vector<int>({6, 14}));
  }


  void testRecompileCondense_forwardCorrispondense() {
    LOG_INFO("testRecompileCondense_forwardCorrispondense");
    // Simulating a ccw step
    PinStateVector pinStatevector;
    pinStatevector.push_back(pinState2);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinStatevector.push_back(pinState2);
    stateSequence.setPinStateVector(pinStatevector);
    stateSequence.setNumberOfRepetitions(1);

    // simulating a cw step
    StateSequence stateSequence1;
    pinStatevector = PinStateVector();
    pinState2.update(2, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinStatevector.push_back(pinState2);
    stateSequence1.setPinStateVector(pinStatevector);
    stateSequence1.setNumberOfRepetitions(1);

    // Simulating 10 cw steps
    pinStatevector = PinStateVector();
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinStatevector.push_back(pinState2);

    StateSequence stateSequence2;
    stateSequence2.setPinStateVector(pinStatevector);
    stateSequence2.setNumberOfRepetitions(10);

    sequenceVector.setSequenceVector(
        PinStateSequenceVector(
            {stateSequence, stateSequence1, stateSequence2}));

    /*
      LOG_DEBUG("Displaying sequences that need to be " <<
      "recompile condensed for foward condense");
      stateSequence.displaySequence();
      stateSequence1.displaySequence();
      stateSequence2.displaySequence();
    */
    int numberOfOriginalSteps = sequenceVector.numberOfSteps();

    TS_ASSERT(sequenceVector.condenseVector());
    TS_ASSERT_EQUALS(sequenceVector.numberOfSteps(),
                     numberOfOriginalSteps);

    for (auto& i:sequenceVector.getSequenceVector())
      i.displaySequence();
    
    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 4);

    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[0].getIntegerSequence(),
                     std::vector<int>({}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[1].getIntegerSequence(),
                     std::vector<int>({}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[2].getIntegerSequence(),
                     std::vector<int>({ 14, 12, 4, 12, 8 }));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[3].getIntegerSequence(),
                     std::vector<int>({0, 8}));    
  }

  void testRecompileCondense_internalRecompile_to_end() {
    LOG_INFO("testRecompileCondense_internalRecompile_to_end");
    /*
      Simulating a sequence which can be condensed
      by merging the last 2 steps and repeating
    */
    // Simulating a ccw step
    PinStateVector pinStatevector;
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(1, 0);
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    
    stateSequence.setPinStateVector(pinStatevector);
    stateSequence.setNumberOfRepetitions(1);

    sequenceVector.setSequenceVector(
        PinStateSequenceVector(
            {stateSequence}));

    std::vector<int> x = stateSequence.getIntegerSequence();
    for (auto& i : x)
      std::cout << i <<", ";
    std::cout << "." << std::endl;

    int numberOfOriginalSteps = sequenceVector.numberOfSteps();

    TS_ASSERT(sequenceVector.condenseVector());
    TS_ASSERT_EQUALS(sequenceVector.numberOfSteps(),
                     numberOfOriginalSteps);

    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 3);

    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[0].getIntegerSequence(),
                     std::vector<int>({}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[1].getIntegerSequence(),
                     std::vector<int>({6, 4}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[2].getIntegerSequence(),
                     std::vector<int>({14, 4}));
  }

  void testRecompileCondense_internalRecompile_to_begin() {
    LOG_INFO("testRecompileCondense_internalRecompile_to_begin");
    /*
      Simulating a sequence which can be condensed
      by merging the first 2 steps and repeating
    */
    PinStateVector pinStatevector;
    pinStatevector.push_back(pinState2);
    pinState2.update(1, 0);
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    stateSequence.setPinStateVector(pinStatevector);
    stateSequence.setNumberOfRepetitions(1);

    sequenceVector.setSequenceVector(
        PinStateSequenceVector(
            {stateSequence}));

    std::vector<int> x = stateSequence.getIntegerSequence();

    int numberOfOriginalSteps = sequenceVector.numberOfSteps();

    TS_ASSERT(sequenceVector.condenseVector());
    TS_ASSERT_EQUALS(sequenceVector.numberOfSteps(),
                     numberOfOriginalSteps);

    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 3);

    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[0].getIntegerSequence(),
                     std::vector<int>({}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[1].getIntegerSequence(),
                     std::vector<int>({14, 4}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[2].getIntegerSequence(),
                     std::vector<int>({4}));
  }


  void testRecompileCondense_internalRecompile_to_middle() {
    LOG_INFO("testRecompileCondense_internalRecompile_to_middle");
    // Simulating a ccw step
    PinStateVector pinStatevector;
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(1, 0);
    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 1);
    pinState2.update(1, 1);
    pinStatevector.push_back(pinState2);
    pinState2.update(3, 0);
    pinState2.update(1, 0);
    pinStatevector.push_back(pinState2);

    pinState2.update(3, 0);
    pinStatevector.push_back(pinState2);
    stateSequence.setPinStateVector(pinStatevector);
    stateSequence.setNumberOfRepetitions(1);

    sequenceVector.setSequenceVector(
        PinStateSequenceVector(
            {stateSequence}));

    std::vector<int> x = stateSequence.getIntegerSequence();
    int numberOfOriginalSteps = sequenceVector.numberOfSteps();

    TS_ASSERT(sequenceVector.condenseVector());
    TS_ASSERT_EQUALS(sequenceVector.numberOfSteps(),
                     numberOfOriginalSteps);

    TS_ASSERT_EQUALS(sequenceVector.numberOfSequences(), 4);

    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[0].getIntegerSequence(),
                     std::vector<int>({}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[1].getIntegerSequence(),
                     std::vector<int>({12, 4}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[2].getIntegerSequence(),
                     std::vector<int>({14, 4}));
    TS_ASSERT_EQUALS(sequenceVector.getSequenceVector()[3].getIntegerSequence(),
                     std::vector<int>({4}));
  }
};

#endif  // MOTOR_UNIT_TEST_SEQUENCEVECTORUNITTEST_H_
