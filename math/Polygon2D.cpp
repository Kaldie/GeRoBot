/// Copyright Ruud Cools [2015]
#include <macroHeader.h>
#include "./Polygon2D.h"
#include "./Point2D.h"

/// easy constructor
Polygon2D::Polygon2D() {}


/// Constructor with vector
Polygon2D::Polygon2D(const Point2DVector& i_vector)
  : m_pointVector(i_vector) {
}


/// add an item to polygon
void Polygon2D::addPoint2D(const Point2D& i_point) {
  m_pointVector.push_back(i_point);
}


/// add an Point2D to the Polygon2D at the index
void Polygon2D::addPoint2D(const Point2D& i_point, const size_t& i_index) {
  if (m_pointVector.size() <= i_index) {
    addPoint2D(i_point);
  }
  Point2DVector::iterator itr = m_pointVector.begin() + i_index;
  m_pointVector.insert(itr, i_point);
}


/// returns the number of given points
int Polygon2D::getNumberOfPoints() const {
  return m_pointVector.size();
}


/// Determines if a point is within the Polygon2D
bool Polygon2D::isInside(const Point2D i_point) const {
  LOG_DEBUG("Point: " << i_point.x << " , " << i_point.y);
  Line2D toOutSideLine = getOutsideLine(i_point);
  LOG_DEBUG("Outside line: start " << toOutSideLine.getStartPoint().x << " , " << toOutSideLine.getStartPoint().y <<
            " stop " << toOutSideLine.getEndPoint().x << " , " << toOutSideLine.getEndPoint().y);

  int interSections = 0;
  for (const auto& line : getLines()) {
    if (line.intersects(toOutSideLine)) {
      ++interSections;
      LOG_DEBUG("Found intersection!");
    }
  }
  if (interSections % 2) {
    return true;
  } else {
    return false;
  }
}


/// Determines if a Polygon2D is within the Polygon2D
bool Polygon2D::isInside(const Polygon2D& i_polygon) const {
  for (const Point2D& point : i_polygon.m_pointVector) {
    if (!isInside(point)) {
      return false;
    }
  }
  return true;
}


/// Determines if the Polygon2D is closed
bool Polygon2D::isClosed() const {
  if(m_pointVector.front() == m_pointVector.back())
    return true;
  else
    return false;
}


/// get the Line2Ds for all the points, closes the polygon if it is not closed
std::vector<Line2D> Polygon2D::getLines() const {
  Polygon2D newPoly(*this);
  newPoly.closePolygon();
  int numberOfLines = newPoly.getNumberOfPoints() - 2;
  std::vector<Line2D> lines;
  Point2DVector vector(newPoly.getPointVector());
  for (int i = 0;
       i != numberOfLines;
       ++i) {
    lines.push_back(Line2D(vector[i],
			   vector[i+1]));
  }
  return lines;
}


/// If the polygon is not closed close it artificial by adding a extra point which is equal to the first
bool Polygon2D::closePolygon() {
  if (isClosed()) {
    return false;
  }
  addPoint2D(m_pointVector.back());
  return true;
}


/// Get a Points which lays outside the Polygon2D
Line2D Polygon2D::getOutsideLine(Point2D i_point) const {
  traceType min,max;
  getXBounds(&min, &max);
  Line2D minLine(i_point, Point2D(min - 10 * TOLERANCE, i_point.y));
  Line2D maxLine(i_point, Point2D(max + 10 * TOLERANCE, i_point.y));
  if (minLine.getLength() < maxLine.getLength()) {
    return minLine;
  } else {
    return maxLine;
  }
}


void Polygon2D::getXBounds(traceType* i_lowerBound,
                           traceType* i_upperBound) const {
  for (const auto& point : m_pointVector) {
    if (*i_lowerBound > point.x) {
      *i_lowerBound = point.x;
    }
    if (*i_upperBound < point.x) {
      *i_upperBound = point.x;
    }
  }
}


void Polygon2D::getYBounds(traceType* i_lowerBound,
                           traceType* i_upperBound) const {
  for (const auto& point : m_pointVector) {
    if (*i_lowerBound > point.y) {
      *i_lowerBound = point.y;
    }
    if (*i_upperBound < point.y) {
      *i_upperBound = point.y;
    }
  }
}


bool Polygon2D::operator==(const Polygon2D& i_polygon) const {
  // if the size is not equal, they are not equal
  if (i_polygon.m_pointVector.size() !=
      m_pointVector.size()) {
    return false;
  }
  // do a point by point comparison
  auto rhsIterator = i_polygon.m_pointVector.begin();
  auto lhsIterator = m_pointVector.begin();
  for (;
       lhsIterator != m_pointVector.end();
       ++lhsIterator, ++rhsIterator) {
    // if one fails, they are not equal
    if (*lhsIterator != *rhsIterator) {
      return false;
    }
  }
  // if all is equal, they are equal
  return true;
}


traceType Polygon2D::getSurface() const {
  if (!isClosed()) {
    LOG_ERROR("Polygon should be closed");
  }
  /// Yes this code is gotten from a website
  /// http://geomalgorithms.com/a01-_area.html
  /// Works like a charm!
  /// if a polygon has 3 points then there is no surface
  if (getNumberOfPoints() < 3)
    return 0;
  traceType area = 0;
  int  i, j, k;   // indices
  int numberOfPoints = getNumberOfPoints();
  for (i = 1, j = 2, k = 0;
       i < numberOfPoints;
       i++, j++, k++) {
    area += m_pointVector[i].x * (m_pointVector[j].y - m_pointVector[k].y);
  }
  area += m_pointVector[numberOfPoints].x * (m_pointVector[1].y - m_pointVector[numberOfPoints-1].y);  // wrap-around term
  return area / 2.0;
}
