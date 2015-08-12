// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "./Point2DIO.h"


void Point2DIO::build() {
  LOG_DEBUG("Building a Point2D");
  m_point2D.x = getNodeFromPath("./X").text().as_float();
  m_point2D.y = getNodeFromPath("./Y").text().as_float();
}


Point2DIO::Point2DIO(const pugi::xml_node& i_node) {
  setNode(i_node);
}


bool Point2DIO::update(const Point2D& i_point2D) {
  getNodeFromPath("./Y").text().set(m_point2D.y);
  getNodeFromPath("./X").text().set(m_point2D.x);
}
