/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_PinStateTestSuite_init = false;
#include "PinStateUnitTest.h"

static PinStateTestSuite suite_PinStateTestSuite;

static CxxTest::List Tests_PinStateTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_PinStateTestSuite( "PinStateUnitTest.h", 11, "PinStateTestSuite", suite_PinStateTestSuite, Tests_PinStateTestSuite );

static class TestDescription_suite_PinStateTestSuite_testCreation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateTestSuite_testCreation() : CxxTest::RealTestDescription( Tests_PinStateTestSuite, suiteDescription_PinStateTestSuite, 13, "testCreation" ) {}
 void runTest() { suite_PinStateTestSuite.testCreation(); }
} testDescription_suite_PinStateTestSuite_testCreation;

static class TestDescription_suite_PinStateTestSuite_testUpdate : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateTestSuite_testUpdate() : CxxTest::RealTestDescription( Tests_PinStateTestSuite, suiteDescription_PinStateTestSuite, 28, "testUpdate" ) {}
 void runTest() { suite_PinStateTestSuite.testUpdate(); }
} testDescription_suite_PinStateTestSuite_testUpdate;

#include "PinStateSequenceUnitTest.h"

static PinStateSequenceTestSuite suite_PinStateSequenceTestSuite;

static CxxTest::List Tests_PinStateSequenceTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_PinStateSequenceTestSuite( "PinStateSequenceUnitTest.h", 12, "PinStateSequenceTestSuite", suite_PinStateSequenceTestSuite, Tests_PinStateSequenceTestSuite );

static class TestDescription_suite_PinStateSequenceTestSuite_testDefaultCreation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateSequenceTestSuite_testDefaultCreation() : CxxTest::RealTestDescription( Tests_PinStateSequenceTestSuite, suiteDescription_PinStateSequenceTestSuite, 14, "testDefaultCreation" ) {}
 void runTest() { suite_PinStateSequenceTestSuite.testDefaultCreation(); }
} testDescription_suite_PinStateSequenceTestSuite_testDefaultCreation;

static class TestDescription_suite_PinStateSequenceTestSuite_testCreation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateSequenceTestSuite_testCreation() : CxxTest::RealTestDescription( Tests_PinStateSequenceTestSuite, suiteDescription_PinStateSequenceTestSuite, 22, "testCreation" ) {}
 void runTest() { suite_PinStateSequenceTestSuite.testCreation(); }
} testDescription_suite_PinStateSequenceTestSuite_testCreation;

static class TestDescription_suite_PinStateSequenceTestSuite_testDefectingCreation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateSequenceTestSuite_testDefectingCreation() : CxxTest::RealTestDescription( Tests_PinStateSequenceTestSuite, suiteDescription_PinStateSequenceTestSuite, 60, "testDefectingCreation" ) {}
 void runTest() { suite_PinStateSequenceTestSuite.testDefectingCreation(); }
} testDescription_suite_PinStateSequenceTestSuite_testDefectingCreation;

static class TestDescription_suite_PinStateSequenceTestSuite_testAddPinStateToSequence : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateSequenceTestSuite_testAddPinStateToSequence() : CxxTest::RealTestDescription( Tests_PinStateSequenceTestSuite, suiteDescription_PinStateSequenceTestSuite, 77, "testAddPinStateToSequence" ) {}
 void runTest() { suite_PinStateSequenceTestSuite.testAddPinStateToSequence(); }
} testDescription_suite_PinStateSequenceTestSuite_testAddPinStateToSequence;

static class TestDescription_suite_PinStateSequenceTestSuite_testAddPinStateVector : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateSequenceTestSuite_testAddPinStateVector() : CxxTest::RealTestDescription( Tests_PinStateSequenceTestSuite, suiteDescription_PinStateSequenceTestSuite, 93, "testAddPinStateVector" ) {}
 void runTest() { suite_PinStateSequenceTestSuite.testAddPinStateVector(); }
} testDescription_suite_PinStateSequenceTestSuite_testAddPinStateVector;

static class TestDescription_suite_PinStateSequenceTestSuite_testAddPinStateSequence : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateSequenceTestSuite_testAddPinStateSequence() : CxxTest::RealTestDescription( Tests_PinStateSequenceTestSuite, suiteDescription_PinStateSequenceTestSuite, 116, "testAddPinStateSequence" ) {}
 void runTest() { suite_PinStateSequenceTestSuite.testAddPinStateSequence(); }
} testDescription_suite_PinStateSequenceTestSuite_testAddPinStateSequence;

static class TestDescription_suite_PinStateSequenceTestSuite_testMergeSequenceWithEqualPinStateVector : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateSequenceTestSuite_testMergeSequenceWithEqualPinStateVector() : CxxTest::RealTestDescription( Tests_PinStateSequenceTestSuite, suiteDescription_PinStateSequenceTestSuite, 178, "testMergeSequenceWithEqualPinStateVector" ) {}
 void runTest() { suite_PinStateSequenceTestSuite.testMergeSequenceWithEqualPinStateVector(); }
} testDescription_suite_PinStateSequenceTestSuite_testMergeSequenceWithEqualPinStateVector;

static class TestDescription_suite_PinStateSequenceTestSuite_testMergeOfNoneEqualSequence : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateSequenceTestSuite_testMergeOfNoneEqualSequence() : CxxTest::RealTestDescription( Tests_PinStateSequenceTestSuite, suiteDescription_PinStateSequenceTestSuite, 216, "testMergeOfNoneEqualSequence" ) {}
 void runTest() { suite_PinStateSequenceTestSuite.testMergeOfNoneEqualSequence(); }
} testDescription_suite_PinStateSequenceTestSuite_testMergeOfNoneEqualSequence;

static class TestDescription_suite_PinStateSequenceTestSuite_testSequenceNormalisation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateSequenceTestSuite_testSequenceNormalisation() : CxxTest::RealTestDescription( Tests_PinStateSequenceTestSuite, suiteDescription_PinStateSequenceTestSuite, 301, "testSequenceNormalisation" ) {}
 void runTest() { suite_PinStateSequenceTestSuite.testSequenceNormalisation(); }
} testDescription_suite_PinStateSequenceTestSuite_testSequenceNormalisation;

static class TestDescription_suite_PinStateSequenceTestSuite_testPinStateSequenceMessage : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_PinStateSequenceTestSuite_testPinStateSequenceMessage() : CxxTest::RealTestDescription( Tests_PinStateSequenceTestSuite, suiteDescription_PinStateSequenceTestSuite, 338, "testPinStateSequenceMessage" ) {}
 void runTest() { suite_PinStateSequenceTestSuite.testPinStateSequenceMessage(); }
} testDescription_suite_PinStateSequenceTestSuite_testPinStateSequenceMessage;

#include "StepperDriverUnitTest.h"

static StepperDriverUnitTest suite_StepperDriverUnitTest;

static CxxTest::List Tests_StepperDriverUnitTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_StepperDriverUnitTest( "StepperDriverUnitTest.h", 12, "StepperDriverUnitTest", suite_StepperDriverUnitTest, Tests_StepperDriverUnitTest );

static class TestDescription_suite_StepperDriverUnitTest_testCreation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_StepperDriverUnitTest_testCreation() : CxxTest::RealTestDescription( Tests_StepperDriverUnitTest, suiteDescription_StepperDriverUnitTest, 14, "testCreation" ) {}
 void runTest() { suite_StepperDriverUnitTest.testCreation(); }
} testDescription_suite_StepperDriverUnitTest_testCreation;

static class TestDescription_suite_StepperDriverUnitTest_testMoveStep : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_StepperDriverUnitTest_testMoveStep() : CxxTest::RealTestDescription( Tests_StepperDriverUnitTest, suiteDescription_StepperDriverUnitTest, 33, "testMoveStep" ) {}
 void runTest() { suite_StepperDriverUnitTest.testMoveStep(); }
} testDescription_suite_StepperDriverUnitTest_testMoveStep;

static class TestDescription_suite_StepperDriverUnitTest_testMoveSteps : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_StepperDriverUnitTest_testMoveSteps() : CxxTest::RealTestDescription( Tests_StepperDriverUnitTest, suiteDescription_StepperDriverUnitTest, 71, "testMoveSteps" ) {}
 void runTest() { suite_StepperDriverUnitTest.testMoveSteps(); }
} testDescription_suite_StepperDriverUnitTest_testMoveSteps;

#include "RotationalJointUnitTest.h"

static RotationalJointUnitTest suite_RotationalJointUnitTest;

static CxxTest::List Tests_RotationalJointUnitTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_RotationalJointUnitTest( "RotationalJointUnitTest.h", 14, "RotationalJointUnitTest", suite_RotationalJointUnitTest, Tests_RotationalJointUnitTest );

static class TestDescription_suite_RotationalJointUnitTest_testCreation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_RotationalJointUnitTest_testCreation() : CxxTest::RealTestDescription( Tests_RotationalJointUnitTest, suiteDescription_RotationalJointUnitTest, 16, "testCreation" ) {}
 void runTest() { suite_RotationalJointUnitTest.testCreation(); }
} testDescription_suite_RotationalJointUnitTest_testCreation;

static class TestDescription_suite_RotationalJointUnitTest_testPredictStep : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_RotationalJointUnitTest_testPredictStep() : CxxTest::RealTestDescription( Tests_RotationalJointUnitTest, suiteDescription_RotationalJointUnitTest, 36, "testPredictStep" ) {}
 void runTest() { suite_RotationalJointUnitTest.testPredictStep(); }
} testDescription_suite_RotationalJointUnitTest_testPredictStep;

static class TestDescription_suite_RotationalJointUnitTest_testPredictSteps : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_RotationalJointUnitTest_testPredictSteps() : CxxTest::RealTestDescription( Tests_RotationalJointUnitTest, suiteDescription_RotationalJointUnitTest, 49, "testPredictSteps" ) {}
 void runTest() { suite_RotationalJointUnitTest.testPredictSteps(); }
} testDescription_suite_RotationalJointUnitTest_testPredictSteps;

static class TestDescription_suite_RotationalJointUnitTest_testClone : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_RotationalJointUnitTest_testClone() : CxxTest::RealTestDescription( Tests_RotationalJointUnitTest, suiteDescription_RotationalJointUnitTest, 64, "testClone" ) {}
 void runTest() { suite_RotationalJointUnitTest.testClone(); }
} testDescription_suite_RotationalJointUnitTest_testClone;

#include "TranslationalJointUnitTest.h"

static TranslationalJointUnitTest suite_TranslationalJointUnitTest;

static CxxTest::List Tests_TranslationalJointUnitTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TranslationalJointUnitTest( "TranslationalJointUnitTest.h", 14, "TranslationalJointUnitTest", suite_TranslationalJointUnitTest, Tests_TranslationalJointUnitTest );

static class TestDescription_suite_TranslationalJointUnitTest_testCreation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TranslationalJointUnitTest_testCreation() : CxxTest::RealTestDescription( Tests_TranslationalJointUnitTest, suiteDescription_TranslationalJointUnitTest, 16, "testCreation" ) {}
 void runTest() { suite_TranslationalJointUnitTest.testCreation(); }
} testDescription_suite_TranslationalJointUnitTest_testCreation;

static class TestDescription_suite_TranslationalJointUnitTest_testPredictStep : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TranslationalJointUnitTest_testPredictStep() : CxxTest::RealTestDescription( Tests_TranslationalJointUnitTest, suiteDescription_TranslationalJointUnitTest, 36, "testPredictStep" ) {}
 void runTest() { suite_TranslationalJointUnitTest.testPredictStep(); }
} testDescription_suite_TranslationalJointUnitTest_testPredictStep;

static class TestDescription_suite_TranslationalJointUnitTest_testPredictSteps : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TranslationalJointUnitTest_testPredictSteps() : CxxTest::RealTestDescription( Tests_TranslationalJointUnitTest, suiteDescription_TranslationalJointUnitTest, 49, "testPredictSteps" ) {}
 void runTest() { suite_TranslationalJointUnitTest.testPredictSteps(); }
} testDescription_suite_TranslationalJointUnitTest_testPredictSteps;

static class TestDescription_suite_TranslationalJointUnitTest_testClone : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TranslationalJointUnitTest_testClone() : CxxTest::RealTestDescription( Tests_TranslationalJointUnitTest, suiteDescription_TranslationalJointUnitTest, 63, "testClone" ) {}
 void runTest() { suite_TranslationalJointUnitTest.testClone(); }
} testDescription_suite_TranslationalJointUnitTest_testClone;

#include "JointControllerUnitTest.h"

static JointControllerTestSuite suite_JointControllerTestSuite;

static CxxTest::List Tests_JointControllerTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_JointControllerTestSuite( "JointControllerUnitTest.h", 15, "JointControllerTestSuite", suite_JointControllerTestSuite, Tests_JointControllerTestSuite );

static class TestDescription_suite_JointControllerTestSuite_testCreation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_JointControllerTestSuite_testCreation() : CxxTest::RealTestDescription( Tests_JointControllerTestSuite, suiteDescription_JointControllerTestSuite, 38, "testCreation" ) {}
 void runTest() { suite_JointControllerTestSuite.testCreation(); }
} testDescription_suite_JointControllerTestSuite_testCreation;

static class TestDescription_suite_JointControllerTestSuite_testAddJoint : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_JointControllerTestSuite_testAddJoint() : CxxTest::RealTestDescription( Tests_JointControllerTestSuite, suiteDescription_JointControllerTestSuite, 47, "testAddJoint" ) {}
 void runTest() { suite_JointControllerTestSuite.testAddJoint(); }
} testDescription_suite_JointControllerTestSuite_testAddJoint;

static class TestDescription_suite_JointControllerTestSuite_testResetPinStateSequence : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_JointControllerTestSuite_testResetPinStateSequence() : CxxTest::RealTestDescription( Tests_JointControllerTestSuite, suiteDescription_JointControllerTestSuite, 67, "testResetPinStateSequence" ) {}
 void runTest() { suite_JointControllerTestSuite.testResetPinStateSequence(); }
} testDescription_suite_JointControllerTestSuite_testResetPinStateSequence;

static class TestDescription_suite_JointControllerTestSuite_testMoveStep : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_JointControllerTestSuite_testMoveStep() : CxxTest::RealTestDescription( Tests_JointControllerTestSuite, suiteDescription_JointControllerTestSuite, 96, "testMoveStep" ) {}
 void runTest() { suite_JointControllerTestSuite.testMoveStep(); }
} testDescription_suite_JointControllerTestSuite_testMoveStep;

static class TestDescription_suite_JointControllerTestSuite_testMoveSteps : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_JointControllerTestSuite_testMoveSteps() : CxxTest::RealTestDescription( Tests_JointControllerTestSuite, suiteDescription_JointControllerTestSuite, 120, "testMoveSteps" ) {}
 void runTest() { suite_JointControllerTestSuite.testMoveSteps(); }
} testDescription_suite_JointControllerTestSuite_testMoveSteps;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
