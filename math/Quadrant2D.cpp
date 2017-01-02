#include <macroHeader.h>
#include <Quadrant2D.h>


Quadrant2D::Quadrant2D(const Point2D& i_point) {
  m_quadrant = resolveQuadrant2D(i_point);
}


Quadrant2D::Quadrant2D(const int& i_quadrant)
  : m_quadrant(i_quadrant) {
}


int Quadrant2D::resolveQuadrant2D(const Point2D& i_point) const {
  if (i_point.y >= 0) {
    if(i_point.x >= 0) {
      return 0;
    } else{
      return 1;
    }
  } else {
    if (i_point.x >= 0) {
      return 3;
    } else {
      return 2;
    }
  }
}


Point2D Quadrant2D::getBorderPoint(const bool& i_clockwise /*= false*/) const {

  int border = m_quadrant;
  if (!i_clockwise) {
    if (border < 3) {
      ++border;
    } else {
      border = 0;
    }
  }
  /* the following list is made. Therefor if i_clockwise ++m_quadrant
     0 : 1, 0; even
     1 : 0, 1; uneven
     2 : -1, 0; even
     3 : 0, -1; uneven
  */
  Point2D boundry((border + 1) % 2, border % 2);  
  if (border >= 2) {
    boundry *= -1;
  }
  LOG_DEBUG("Quadrant: " << m_quadrant << ". Border point: " << boundry);
  return boundry;
}


void Quadrant2D::getQuardrantNeighbours(int* o_ccwQuardrant,
					int* o_cwQuardrant) const {
  *o_ccwQuardrant = m_quadrant + 1;
  correctQuadrant(o_ccwQuardrant);
  *o_cwQuardrant = m_quadrant - 1;
  correctQuadrant(o_cwQuardrant);
 }


void Quadrant2D::correctQuadrant(int* io_quadrant) const {
  if (*io_quadrant < 0) {
    *io_quadrant = 4 + *io_quadrant;
  }
  if (*io_quadrant > 3) {
    *io_quadrant -= 4;
  }
}
