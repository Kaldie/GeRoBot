// Copyright [2015] Ruud Cools

#ifndef MATH_UNIT_TEST_ORDEREDTRACESECTIONSTEST_H_
#define MATH_UNIT_TEST_ORDEREDTRACESECTIONSTEST_H_

#include <macroHeader.h>
#include <cxxtest/TestSuite.h>
#include "../OTSA.h"
#include "../RotationTrace.h"

class OrderedTraceSectionsTest : public CxxTest::TestSuite {
 public:

  void testCircle() {
    RotationTrace fullCircle(Point2D(-40.0,0),
                             Point2D(-40.0,0),
                             Point2D(0,0));
    tsa::TraceSection section;
    section.push_back(std::make_shared<RotationTrace>(fullCircle));
    tsa::otsa::orderVector(&section, Point2D(-40,0));
    TS_ASSERT_EQUALS(section.size(), 1);
    TS_ASSERT_EQUALS(*section[0], fullCircle);
  }


  void testTwoCircles() {
    RotationTrace fullCircle(Point2D(-40.0,0),
                             Point2D(-40.0,0),
                             Point2D(0,0));
    RotationTrace smallCircle(Point2D(-20.0,0),
                              Point2D(-20.0,0),
                              Point2D(0,0));
    tsa::TraceSection section;
    section.push_back(std::make_shared<RotationTrace>(fullCircle));
    section.push_back(std::make_shared<RotationTrace>(smallCircle));
    tsa::otsa::orderVector(&section, Point2D(-40,0));
    TS_ASSERT_EQUALS(section.size(), 2);
    TS_ASSERT_EQUALS(*section[0], smallCircle);
    TS_ASSERT_EQUALS(*section[1], fullCircle);
  }


  void testTwoCirclesAndLine() {
    RotationTrace fullCircle(Point2D(-40.0,0),
                             Point2D(-40.0,0),
                             Point2D(0,0));
    RotationTrace smallCircle(Point2D(-20.0,0),
                              Point2D(-20.0,0),
                              Point2D(0,0));
    Trace line(Point2D(-35, -5),
               Point2D(-35, 5));

    tsa::TraceSection section;
    section.push_back(std::make_shared<RotationTrace>(fullCircle));
    section.push_back(std::make_shared<RotationTrace>(smallCircle));
    section.push_back(std::make_shared<Trace>(line));

    tsa::createPolygon(section).exportToFile();
    tsa::otsa::orderVector(&section, Point2D(-35 , 5));
    TS_ASSERT_EQUALS(section.size(), 3);
    TS_ASSERT_EQUALS(*section[1], smallCircle);
    TS_ASSERT_EQUALS(*section[0], line);
    LOG_DEBUG((*section[0]).getStartPoint().x << " , " << (*section[0]).getStartPoint().y <<
              (*section[0]).getEndPoint().x << " , " << (*section[0]).getEndPoint().y);
    TS_ASSERT_EQUALS(*section[2], fullCircle);

    LOG_DEBUG("Here!!");
    tsa::otsa::orderVector(&section, Point2D(-35 , -5));
    Trace reversedLine(Point2D(-35, 5),
                       Point2D(-35, -5));
    TS_ASSERT_EQUALS(*section[0], reversedLine);
  }


  void testThreeCircles() {
    RotationTrace fullCircle(Point2D(-40.0,0),
                             Point2D(-40.0,0),
                             Point2D(0,0));
    RotationTrace smallCircle(Point2D(-20.0,0),
                              Point2D(-20.0,0),
                              Point2D(0,0));
    RotationTrace smallestCircle(Point2D(-20.0,0),
                                 Point2D(-20.0,0),
                                 Point2D(0,0));
    tsa::TraceSection section;
    section.push_back(std::make_shared<RotationTrace>(smallestCircle));
    section.push_back(std::make_shared<RotationTrace>(fullCircle));
    section.push_back(std::make_shared<RotationTrace>(smallCircle));
    tsa::otsa::orderVector(&section, Point2D(-40,0));
    TS_ASSERT_EQUALS(section.size(), 3);
    TS_ASSERT_EQUALS(*section[0], smallestCircle);
    TS_ASSERT_EQUALS(*section[1], smallCircle);
    TS_ASSERT_EQUALS(*section[2], fullCircle);
  }


  void testSquare() {
    Trace a(Point2D(50, 10),
            Point2D(50, 30));
    Trace b(Point2D(50, 30),
            Point2D(90, 30));
    Trace c(Point2D(90, 30),
            Point2D(90, 10));
    Trace d(Point2D(90, 10),
            Point2D(50, 10));

    tsa::TraceSection square;
    square.push_back(std::make_shared<Trace>(a));
    square.push_back(std::make_shared<Trace>(b));
    square.push_back(std::make_shared<Trace>(c));
    square.push_back(std::make_shared<Trace>(d));
    tsa::otsa::orderVector(&square, Point2D(90,10));
    std::vector<Point2D>
      pointVector({Point2D(90, 10),Point2D(90, 30), Point2D(50, 30),Point2D(50, 10)});
    auto pointItr = pointVector.begin();
    for (const auto& x : square) {
      TS_ASSERT_EQUALS(x->getStartPoint(), *pointItr);
      ++pointItr;
    }

    tsa::otsa::orderVector(&square, Point2D(40, 30));
    std::vector<Point2D>
      pointVector2({Point2D(50, 30), Point2D(90, 30), Point2D(90, 10), Point2D(50, 10)});
    auto pointItr2 = pointVector2.begin();
    for (const auto& x : square) {
      TS_ASSERT_EQUALS(x->getStartPoint(), *pointItr2);
      ++pointItr2;
      LOG_DEBUG("StartPoint: " << x->getStartPoint().x << " , " << x->getStartPoint().y);
      LOG_DEBUG("EndPoint: " << x->getEndPoint().x << " , " << x->getEndPoint().y);
      LOG_DEBUG("-------------");
    }
  }


  void testTwoIndependendSections() {
    RotationTrace fullCircle(Point2D(-40.0,0),
                             Point2D(-40.0,0),
                             Point2D(0,0));
    RotationTrace smallCircle(Point2D(-20.0,0),
                              Point2D(-20.0,0),
                              Point2D(0,0));
    RotationTrace smallestCircle(Point2D(-12.0,0),
                                 Point2D(-12.0,0),
                                 Point2D(0,0));

    Trace a(Point2D(50, 10),
            Point2D(50, 30));
    Trace b(Point2D(50, 30),
            Point2D(90, 30));
    Trace c(Point2D(90, 30),
            Point2D(90, 10));
    Trace d(Point2D(90, 10),
            Point2D(50, 10));

    tsa::TraceSection circles;
    circles.push_back(std::make_shared<RotationTrace>(smallestCircle));
    circles.push_back(std::make_shared<RotationTrace>(fullCircle));
    circles.push_back(std::make_shared<RotationTrace>(smallCircle));

    tsa::TraceSection square;
    square.push_back(std::make_shared<Trace>(a));
    square.push_back(std::make_shared<Trace>(b));
    square.push_back(std::make_shared<Trace>(c));
    square.push_back(std::make_shared<Trace>(d));

    tsa::TraceSection total;
    total.insert(total.end(), circles.begin(), circles.end());
    total.insert(total.end(), square.begin(), square.end());
    tsa::otsa::orderVector(&total, Point2D(-41,0));

    std::vector<Point2D> points
      ({Point2D(-12, 0), Point2D(-20, 0), Point2D(-40, 0),
        Point2D(50, 10), Point2D(50, 30), Point2D(90, 30), Point2D(90, 10)});
    auto pointItr = points.begin();
    for (const auto& x : total) {
      TS_ASSERT_EQUALS(*pointItr, x->getStartPoint());
      ++pointItr;
      LOG_DEBUG("Point2D(" << x->getStartPoint().x << ", " << x->getStartPoint().y << ")");
      LOG_DEBUG("EndPoint: " << x->getEndPoint().x << " , " << x->getEndPoint().y);
      LOG_DEBUG("-------------");
    }
    /*
      TS_ASSERT_EQUALS(total.size(), 7);
    TS_ASSERT_EQUALS(*total[0], smallestCircle);
    TS_ASSERT_EQUALS(*total[1], smallCircle);
    TS_ASSERT_EQUALS(*total[2], fullCircle);
    */
  }

};

#endif  // MATH_UNIT_TEST_ORDEREDTRACESECTIONSTEST_H_
