//copyright [2015] Ruud Cools

#ifndef MATH_POLYGON2D_H_
#define MATH_POLYGON2D_H_

#include "./Point2D.h"
#include "./Line2D.h"

class Polygon2D {
  typedef std::vector<Point2D> Point2DVector;
  GETSET(Point2DVector, m_pointVector, PointVector);
  // the bounding values for polygon
  traceType xMin, xMax, yMin, yMax;
 public:
  /// easy constructor
  Polygon2D();
  /// Constructor with vector
  Polygon2D(const Point2DVector&);
  /// add an item to polygon
  void addPoint2D(const Point2D& i_point);
  /// add an Point2D to the Polygon2D at the index
  void addPoint2D(const Point2D& i_point, const size_t& i_index);
  /// returns the number of given points
  int getNumberOfPoints() const;
  /// Determines if a point is within the Polygon2D
  bool isInside(const Point2D i_point) const;
  /// Determines if a Polygon2D is within the Polygon2D
  bool isInside(const Polygon2D& i_polygon) const;
  /// Determines if the Polygon2D is closed
  bool isClosed() const;
  /// Determine if the polygon is empty
  bool isEmpty() const {return m_pointVector.size() == 0;}
  /**
   * Calculate the surface covered by the polygon
   * The algorithm is described in: http://geomalgorithms.com/a01-_area.html
   */
  traceType getSurface() const;
  bool operator==(const Polygon2D& i_polygon) const;
  bool operator!=(const Polygon2D& i_polygon) const {return !operator==(i_polygon);}
 private:
  /// get the Line2Ds for all the points
  std::vector<Line2D> getLines() const;
  /// If the polygon is not closed close it artificial by adding a extra point which is equal to the first
  bool closePolygon();
  // returns the x bounds of the polygon
  void getXBounds(traceType* i_lowerBound,
                  traceType* i_upperBound) const;
  /// retuns the y bounds of the polygon
  void getYBounds(traceType* i_lowerBound,
                  traceType* i_upperBound) const;
  // get the shortest line to a point outside the polygon
  Line2D getOutsideLine(Point2D i_point) const;
  };
#endif  // MATH_POLYGON2D_H_
