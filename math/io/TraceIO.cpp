// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <TranslationalJoint.h>

#include <RotationTrace.h>
#include "./TraceIO.h"
#include "./Point2DIO.h"


void TraceIO::build() {
  LOG_DEBUG("Building a Trace");
  LOG_DEBUG("Buildiing a Trace!");
  std::string traceType = getNode().first_child().text().as_string();
  if (traceType == "Line") {
    m_tracePointer = std::make_shared<Trace>();
    m_tracePointer->setTraceType(Line);
  } else if (traceType == "Curve") {
    m_tracePointer = std::make_shared<RotationTrace>();
    m_tracePointer->setTraceType(Curve);
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


bool TraceIO::update(const TracePointer& i_trace) {
  LOG_DEBUG("Updateing the Trace");
  getNode().first_child().text().set("Line");
  if (i_trace->getTraceType() == Line) {
    getNode().first_child().text().set("Line");
  } else if (i_trace->getTraceType() == Curve) {
    getNode().first_child().text().set("Curve");
  } else {
    LOG_ERROR("Type is not resolved!");
  }
  bool hasUpdated = true;
  Point2DIO point2DIO(getNodeFromPath("./StartPoint"));
  hasUpdated &= point2DIO.update(i_trace->getStartPoint());
  point2DIO.setNode(getNodeFromPath("./EndPoint"));
  hasUpdated &= point2DIO.update(i_trace->getStartPoint());
  return true;
}
