// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_SEQUENTIALTRACECALCULATORTEST_H_
#define MATH_UNIT_TEST_SEQUENTIALTRACECALCULATORTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include "../SequentialTraceCalculator.h"
#include "../RotationTrace.h"

class SequentialTraceCalculatorTest : public CxxTest::TestSuite {
 public:
  void testCircle() {
    RotationTrace fullCircle(Point2D(-30,50),
			     Point2D(-30,50),
			     Point2D(0,50));
    Trace::TracePointerVector vector;
    vector.push_back(std::make_shared<Trace>(fullCircle));
    SequentialTraceCalculator calc(vector);
    TS_ASSERT(calc.isPartOfClosedSection(vector[0]));
    SequentialTraceCalculator::TraceSections sections;
    TS_ASSERT_EQUALS(1, calc.getClosedSections(&sections));
    TS_ASSERT_EQUALS(sections[0].size(), 1);
  }


  void testCircle2() {
    RotationTrace fullCircle(Point2D(-30,50),
                             Point2D(-30,50),
                             Point2D(0,50));
    RotationTrace fullCircle2(Point2D(-20,50),
                              Point2D(-20,50),
                              Point2D(0,50));
    Trace::TracePointerVector vector;
    vector.push_back(std::make_shared<Trace>(fullCircle));
    vector.push_back(std::make_shared<Trace>(fullCircle2));
    SequentialTraceCalculator calc(vector);
    SequentialTraceCalculator::TraceSections sections;
    TS_ASSERT_EQUALS(2, calc.getClosedSections(&sections));
    TS_ASSERT_EQUALS(sections[0].size(), 1);
    TS_ASSERT_EQUALS(sections[1].size(), 1);
    //    calc.getOrderVector();
  }


  void testSquare() {
    LOG_DEBUG("testSquare()");
    Trace a(Point2D(-10, 10),
	    Point2D(-10, 30));
    Trace b(Point2D(-10, 30),
	    Point2D(30, 30));
    Trace c(Point2D(30, 30),
	    Point2D(30, 10));
    Trace d(Point2D(30, 10),
	    Point2D(-10, 10));
    Trace::TracePointerVector vector;
    vector.push_back(std::make_shared<Trace>(a));
    vector.push_back(std::make_shared<Trace>(b));
    vector.push_back(std::make_shared<Trace>(c));
    vector.push_back(std::make_shared<Trace>(d));
    SequentialTraceCalculator calc(vector);
    TS_ASSERT(calc.isPartOfClosedSection(vector[0]));
    SequentialTraceCalculator::TraceSections sections;
    TS_ASSERT_EQUALS(1, calc.getClosedSections(&sections));
    TS_ASSERT_EQUALS(sections.size(), 1);
    TS_ASSERT_EQUALS(sections[0].size(), 4);
  }


  void testSquareAndCircle() {
    LOG_DEBUG("testSquareAndCircle()");
    Trace a(Point2D(-10, 10),
            Point2D(-10, 30));
    Trace b(Point2D(-10, 30),
            Point2D(30, 30));
    Trace c(Point2D(30, 30),
            Point2D(30, 10));
    Trace d(Point2D(30, 10),
            Point2D(-10, 10));
    RotationTrace fullCircle(Point2D(-15, 10),
                             Point2D(-15, 10),
                             Point2D(10, 20));
    Trace::TracePointerVector vector;
    vector.push_back(std::make_shared<Trace>(a));
    vector.push_back(std::make_shared<Trace>(b));
    vector.push_back(std::make_shared<Trace>(c));
    vector.push_back(std::make_shared<Trace>(d));
    vector.push_back(std::make_shared<RotationTrace>(fullCircle));
    SequentialTraceCalculator calc(vector);
    TS_ASSERT(calc.isPartOfClosedSection(vector[0]));
    SequentialTraceCalculator::TraceSections sections;
    TS_ASSERT_EQUALS(2, calc.getClosedSections(&sections));
    TS_ASSERT_EQUALS(sections.size(), 2);
    TS_ASSERT_EQUALS(sections[0].size(), 1);
    TS_ASSERT_EQUALS(sections[1].size(), 4);
  }

  void testDriehoek() {
    LOG_DEBUG("testDriehoek()");
    Trace a(Point2D(-10, 10),
	    Point2D(-10, 30));
    Trace b(Point2D(-10, 30),
	    Point2D(30, 30));
    Trace c(Point2D(30, 30),
	    Point2D(-10, 10));
    Trace::TracePointerVector vector;
    vector.push_back(std::make_shared<Trace>(a));
    vector.push_back(std::make_shared<Trace>(b));
    vector.push_back(std::make_shared<Trace>(c));
    SequentialTraceCalculator calc(vector);
    TS_ASSERT(calc.isPartOfClosedSection(vector[0]));
    SequentialTraceCalculator::TraceSections sections;
    TS_ASSERT_EQUALS(1, calc.getClosedSections(&sections));
    TS_ASSERT_EQUALS(sections.size(), 1);
    TS_ASSERT_EQUALS(sections[0].size(), 3);
  }

  void testRoundedSquare() {
    LOG_DEBUG("testRoundedSquare()");
    Trace a(Point2D(-10, 12),
            Point2D(-10, 28));  // links omhoog
    Trace b(Point2D(-8, 30),
            Point2D(28, 30)); // links -> rechts hoog
    Trace c(Point2D(30, 28),
            Point2D(30, 12));  // rechts omlaag
    Trace d(Point2D(28, 10),
            Point2D(-8, 10)); // rechts -> links laag

    RotationTrace e(Point2D(-8, 10),
                    Point2D(-10, 12),
                    Point2D(-8, 12));  // linker onder straal 2
    RotationTrace f(Point2D(-10, 28),
                    Point2D(-8, 30),
                    Point2D(-8, 28));  // linker boven straal 2
    RotationTrace g(Point2D(28, 30),
                    Point2D(30, 28),
                    Point2D(28, 28));  // rechts boven straal 2
    RotationTrace h(Point2D(30, 12),
                    Point2D(28, 10),
                    Point2D(28, 12));  // rechts onder straal 2

    Trace::TracePointerVector vector;
    vector.push_back(std::make_shared<Trace>(a));
    vector.push_back(std::make_shared<Trace>(b));
    vector.push_back(std::make_shared<Trace>(c));
    vector.push_back(std::make_shared<Trace>(d));
    vector.push_back(std::make_shared<Trace>(e));
    vector.push_back(std::make_shared<Trace>(f));
    vector.push_back(std::make_shared<Trace>(g));
    vector.push_back(std::make_shared<Trace>(h));
    SequentialTraceCalculator calc(vector);
    TS_ASSERT(calc.isPartOfClosedSection(vector[0]));
    SequentialTraceCalculator::TraceSections sections;
    TS_ASSERT_EQUALS(1, calc.getClosedSections(&sections));
    TS_ASSERT_EQUALS(sections.size(), 1);
    TS_ASSERT_EQUALS(sections[0].size(), 8);
  }
};

#endif  // SEQUENTIALTRACECALCULATORTEST
