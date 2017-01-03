// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_TRACESECTIONSTEST_H_
#define MATH_UNIT_TEST_TRACESECTIONSTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include "../TSA.h"
#include "../RotationTrace.h"

class TraceSectionsTest : public CxxTest::TestSuite {
 public:
  void testCircle() {
    RotationTrace fullCircle(Point2D(-40.0,0),
                             Point2D(-40.0,0),
                             Point2D(0,0), true);
    tsa::TraceSection section;
    section.push_back(std::make_shared<RotationTrace>(fullCircle));
    TS_ASSERT(tsa::isClosedSection(section));
    tsa::TraceSections sections = tsa::getSections(section);
    TS_ASSERT(tsa::isPartOfSections(section[0],sections));
    TS_ASSERT(tsa::isPartOfSection(section[0],section));
    TS_ASSERT_EQUALS(sections[0].size(), 1);
    LOG_DEBUG("Make Poly");
    Polygon2D polygon = tsa::createPolygon(section);
    for (const auto& point : polygon.getPointVector()) {
      LOG_DEBUG("Point: " << point.x << " , " << point.y);
    }
    TS_ASSERT_DELTA(polygon.getSurface(), (40.0 * 40.0 * PI), (40.0 * 40.0 * PI) * 0.01);
  }

  void testCircle2() {
    RotationTrace fullCircle(Point2D(-30,50),
                             Point2D(-30,50),
                             Point2D(0,50), true);
    RotationTrace innerCircle(Point2D(-20,50),
                              Point2D(-20,50),
                              Point2D(0,50), true);
    tsa::TraceSection section;
    section.push_back(std::make_shared<Trace>(fullCircle));
    section.push_back(std::make_shared<Trace>(innerCircle));
    tsa::TraceSections sections = tsa::getSections(section);
    TS_ASSERT_EQUALS(2, sections.size());
    TS_ASSERT_EQUALS(sections[0].size(), 1);
    TS_ASSERT_EQUALS(sections[1].size(), 1);
    //    calc.getOrderVector();
  }


  void testSquare() {
    LOG_DEBUG("testSquare()");
    Trace a(Point2D(-10, 10),
            Point2D(-10, 30));
    Trace b(Point2D(-10, 30),
            Point2D(-30, 30));
    Trace c(Point2D(-30, 30),
            Point2D(-30, 10));
    Trace d(Point2D(-30, 10),
            Point2D(-10, 10));
    tsa::TraceSection vector;
    vector.push_back(std::make_shared<Trace>(a));
    vector.push_back(std::make_shared<Trace>(b));
    vector.push_back(std::make_shared<Trace>(c));
    vector.push_back(std::make_shared<Trace>(d));
    Polygon2D polygon = tsa::createPolygon(vector);
    TS_ASSERT_DELTA(polygon.getSurface(), 20 * 20, 0.001);

    TS_ASSERT(tsa::isClosedSection(vector));
    tsa::TraceSections sections = tsa::getSections(vector);
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
                             Point2D(10, 20), true);
    tsa::TraceSection vector;
    vector.push_back(std::make_shared<Trace>(a));
    vector.push_back(std::make_shared<Trace>(b));
    vector.push_back(std::make_shared<Trace>(c));
    vector.push_back(std::make_shared<Trace>(d));
    vector.push_back(std::make_shared<RotationTrace>(fullCircle));
    tsa::TraceSections sections = tsa::getSections(vector);
    TS_ASSERT_EQUALS(sections.size(), 2);
    TS_ASSERT_EQUALS(sections[0].size(), 1);
    TS_ASSERT_EQUALS(sections[1].size(), 4);
    // sort the 2 in surface size, Cirlce is much bigger then square
    // first small then big
    std::sort(sections.begin(), sections.end(), tsa::sortSmallToBig);
    TS_ASSERT_EQUALS(*sections[1][0], fullCircle);
    // then Big then small
    std::sort(sections.begin(), sections.end(), tsa::sortBigToSmall);
    TS_ASSERT_EQUALS(*sections[0][0], fullCircle);
  }


  void testDriehoek() {
    LOG_DEBUG("testDriehoek()");
    Trace a(Point2D(-10, 9),
            Point2D(-10, 30));
    Trace b(Point2D(-10, 30),
	    Point2D(30, 30));
    Trace c(Point2D(30, 30),
            Point2D(-10, 9));
    tsa::TraceSection vector;
    vector.push_back(std::make_shared<Trace>(a));
    vector.push_back(std::make_shared<Trace>(b));
    vector.push_back(std::make_shared<Trace>(c));
    tsa::TraceSections sections = tsa::getSections(vector);
    TS_ASSERT_EQUALS(sections.size(), 1);
    TS_ASSERT_EQUALS(sections[0].size(), 3);
    Polygon2D poly = tsa::createPolygon(vector);
    TS_ASSERT_DELTA(poly.getSurface(), 40 * 21 * 0.5, 0.001);
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
                    Point2D(-8, 12), true);  // linker onder straal 2
    RotationTrace f(Point2D(-10, 28),
                    Point2D(-8, 30),
                    Point2D(-8, 28), true);  // linker boven straal 2
    RotationTrace g(Point2D(28, 30),
                    Point2D(30, 28),
                    Point2D(28, 28), true);  // rechts boven straal 2
    RotationTrace h(Point2D(30, 12),
                    Point2D(28, 10),
                    Point2D(28, 12), true);  // rechts onder straal 2

    tsa::TraceSection vector;
    vector.push_back(std::make_shared<RotationTrace>(e));
    vector.push_back(std::make_shared<Trace>(a));
    vector.push_back(std::make_shared<RotationTrace>(f));
    vector.push_back(std::make_shared<Trace>(b));
    vector.push_back(std::make_shared<RotationTrace>(g));
    vector.push_back(std::make_shared<Trace>(c));
    vector.push_back(std::make_shared<RotationTrace>(h));
    vector.push_back(std::make_shared<Trace>(d));
    TS_ASSERT(tsa::isClosedSection(vector));
    tsa::TraceSections sections =  tsa::getSections(vector);
    Polygon2D poly = tsa::createPolygon(sections[0]);
    poly.exportToFile();
    TS_ASSERT_DELTA(poly.getSurface(), 20 * 40 - ((4.0 * 4.0) - (2.0 * 2.0 * PI)) , 0.001);
    TS_ASSERT_EQUALS(sections.size(), 1);
    TS_ASSERT_EQUALS(sections[0].size(), 8);
  }


  void testMultipleSquarsInCircle() {
    tsa::TraceSection vector;
    vector.push_back(std::make_shared<Trace>(Trace(Point2D(-5, -5),
                                                   Point2D(-5, 5))));  // links omhoog
    vector.push_back(std::make_shared<Trace>(Trace(Point2D(-5, 5),
                                                   Point2D(5, 5)))); // links -> rechts hoog
    vector.push_back(std::make_shared<Trace>(Trace(Point2D(5, 5),
                                                   Point2D(5, -5))));  // rechts omlaag
    vector.push_back(std::make_shared<Trace>(Trace(Point2D(5, -5),
                                                   Point2D(-5, -5)))); // rechts -> links laag

    vector.push_back(std::make_shared<Trace>(Trace(Point2D(-10, -5),
                                                   Point2D(-10, 5))));  // links omhoog
    vector.push_back(std::make_shared<Trace>(Trace(Point2D(-10, 5),
                                                   Point2D(-6, 5)))); // links -> rechts hoog
    vector.push_back(std::make_shared<Trace>(Trace(Point2D(-6, 5),
                                                        Point2D(-6, -5))));  // rechts omlaag
    vector.push_back(std::make_shared<Trace>(Trace(Point2D(-6, -5),
                                                        Point2D(-10, -5)))); // rechts -> links laag

    vector.push_back(std::make_shared<RotationTrace>(RotationTrace(Point2D(-15, 0),
                                                                   Point2D(-15, 0),
                                                                   Point2D(0, 0), true)));  // linker onder straal 2
    tsa::TraceSections sections = tsa::getSections(vector);
    TS_ASSERT_EQUALS(sections.size(), 3);
    sort(sections.begin(), sections.end(), tsa::sortSmallToBig);
    TS_ASSERT_EQUALS(sections[0][0]->getTraceType(), Trace::Line);
    TS_ASSERT_EQUALS(sections[2][0]->getTraceType(), Trace::Curve);
    TS_ASSERT(tsa::isInside(sections[2], sections[0]));
    tsa::TraceSections::iterator itr = tsa::getIndependendSections(sections.begin(), sections.end());
    TS_ASSERT_EQUALS(sections.begin()+2, itr);
    itr = tsa::getIndependendSections(itr, sections.end());
    TS_ASSERT_EQUALS(sections.end(), itr);

    tsa::TraceSection allTracesInside = tsa::getInsideTraces(vector, sections.back());
    TS_ASSERT_EQUALS(allTracesInside.size(), 8);


    TS_ASSERT_EQUALS(tsa::getExtremeSection(Point2D(-16, 0), true, sections), sections[2]);
    TS_ASSERT_EQUALS(tsa::getExtremeSection(Point2D(-16, 0), false, sections), sections[1]);
    traceType x;
    TS_ASSERT_EQUALS(sections[2][0], tsa::getExtremeTrace(Point2D(-16, 0), true, sections[2], &x));
    TS_ASSERT_EQUALS(sections[1][1],tsa::getExtremeTrace(Point2D(-16, 0), false, sections[1], &x));
  }
};

#endif  // MATH_UNIT_TEST_TRACESECTIONSTEST_H_
