// Copyright [2015] Ruud Cools

#ifndef MATH_IO_POINT2DIO_H_
#define MATH_IO_POINT2DIO_H_

#include <XMLBuilder.h>
#include <Point2D.h>

class Point2DIO: public XMLBuilder {
  GETSET(Point2D, m_point2D, Point2D);

 private:
  // No default contructor!!
  Point2DIO();

 public:
  virtual void build();
  explicit Point2DIO(const pugi::xml_node&);
  bool update(const Point2D& i_point2D);
};

#endif  // MATH_IO_POINT2DIO_H_
