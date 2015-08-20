// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <RotationTrace.h>
#include "./TraceIO.h"
#include "./Point2DIO.h"


void TraceIO::build() {
  LOG_DEBUG("Building a Trace");
  std::string traceType = getNode().first_child().text().as_string();
  if (isLineTrace()) {
    m_tracePointer = std::make_shared<Trace>();
    m_tracePointer->setTraceType(Trace::Line);
  } else if (traceType == "Curve") {
    m_tracePointer = std::make_shared<RotationTrace>();
    m_tracePointer->setTraceType(Trace::Curve);
  } else {
    LOG_ERROR("Unknown trace type: " << traceType);
  }

  Point2DIO point2DIO(getNodeFromPath("./StartPoint"));
  point2DIO.build();
  m_tracePointer->setStartPoint(point2DIO.getPoint2D());
  point2DIO.setNode(getNodeFromPath("./EndPoint"));
  point2DIO.build();
  m_tracePointer->setEndPoint(point2DIO.getPoint2D());
}


TraceIO::TraceIO(const pugi::xml_node& i_node) {
  setNode(i_node);
}


TraceIO::TraceIO(const std::string& i_fileName)
    : XMLBuilder(i_fileName) {
  XMLBuilder::build();
  setNode(getNodeFromPath("./Trace"));
}


bool TraceIO::update(const Trace::TracePointer& i_trace) {
  LOG_DEBUG("Updateing the Trace");
  if (i_trace->getTraceType() == Trace::Line) {
    getNode().first_child().text().set("Line");
  } else if (i_trace->getTraceType() == Trace::Curve) {
    getNode().first_child().text().set("Curve");
  } else {
    LOG_ERROR("Type is not resolved!");
  }
  bool hasUpdated = true;
  Point2DIO point2DIO(getNodeFromPath("./StartPoint"));
  LOG_DEBUG("Startpoint x,y: " << i_trace->getStartPoint().x << "," << i_trace->getStartPoint().y);
  hasUpdated &= point2DIO.update(i_trace->getStartPoint());
  LOG_DEBUG("Endpoint x,y: " << i_trace->getEndPoint().x << "," << i_trace->getEndPoint().y);
  point2DIO.setNode(getNodeFromPath("./EndPoint"));
  hasUpdated &= point2DIO.update(i_trace->getEndPoint());
  return true;
}


bool TraceIO::isLineTrace() const {
  std::string traceType = getNode().first_child().text().as_string();
  if (traceType == "Line") {
    return true;
  } else if (traceType == "Curve") {
    return false;
  } else {
    LOG_ERROR("Unknown trace type: " << traceType);
  }
}
