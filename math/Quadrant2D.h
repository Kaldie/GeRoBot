#ifndef MATH_QUADRANT2D_H_
#define MATH_QUADRANT2D_H_

#include <Point2D.h>
class Quadrant2D {
  GETSET(int, m_quadrant, QuadrantValue);
 private:
  int resolveQuadrant2D(const Point2D& i_point) const;
  void correctQuadrant(int* io_quadrant) const;
  void correctQuadrant() {correctQuadrant(&m_quadrant);};
 public:
  Quadrant2D(const Point2D& i_point);
  Quadrant2D(const int& i_quadrant);

  /// re
  void getQuardrantNeighbours(int* o_ccwQuardrant,
			      int* o_cwQuardrant) const;

  Point2D getBorderPoint(const bool& i_clockwise = false) const;
  float getBorderAngle(const bool& i_clockwise = false) const;

  /// increment this quadrant to represent the next quadrant
  void increment(const bool& i_isClockwise) {i_isClockwise?--*this:++*this;};
    
  /// operators of the quadrant.
  /// They make sure we will end up in a valid quadrant
  Quadrant2D& operator+=(const int& i_value) {m_quadrant += i_value; correctQuadrant(); return *this;};
  Quadrant2D& operator-=(const int& i_value) {m_quadrant -= i_value; correctQuadrant(); return *this;};
  Quadrant2D& operator--() {--m_quadrant; correctQuadrant(); return *this;};
  Quadrant2D& operator++() {++m_quadrant; correctQuadrant(); return *this;};
  bool operator<(const Quadrant2D& i_rh) {return m_quadrant < i_rh.m_quadrant;};
  bool operator>(const Quadrant2D& i_rh) {return m_quadrant > i_rh.m_quadrant;};
  bool operator==(const Quadrant2D& i_rh) {return this->m_quadrant == i_rh.m_quadrant;};
  bool operator!=(const Quadrant2D& i_rh) {return !(*this == i_rh);};
  
};
#endif  // MATH_QUADRANT2D_H_
