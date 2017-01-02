// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <RotationTrace.h>
#include <RotationTraceIO.h>
#include <Point2DIO.h>
#include <Arc2D.h>


void RotationTraceIO::build() {
  LOG_DEBUG("Building a RotationTrace");
  TraceIO::build();
  RotationTrace::RotationTracePointer pointer =
    std::static_pointer_cast<RotationTrace>(m_tracePointer);
  // Build centre point
  Point2DIO point2DIO(getNodeFromPath("./CenterPoint"));
  point2DIO.build();
  pointer->setCentrePoint(point2DIO.getPoint2D());
  // set direction
  std::string rotationDirection =
    getNodeFromPath("./RotationDirection").text().as_string();
  if (rotationDirection.compare("CCW") == 0) {
    pointer->setIsClockwise(false);
  } else if (rotationDirection.compare("CW") == 0) {
    pointer->setIsClockwise(true);
  } else {
    LOG_ERROR("Unknown rotation direction");
  }  
}



RotationTraceIO::RotationTraceIO(const pugi::xml_node& i_node)
  : TraceIO(i_node)
{}


bool RotationTraceIO::update(const Trace::TracePointer& i_trace) {
  LOG_DEBUG("Updateing Rotation Trace");
  bool hasUpdated = TraceIO::update(i_trace);
  
  RotationTrace::RotationTracePointer pointer =
    std::static_pointer_cast<RotationTrace>(i_trace);

  Point2DIO point2DIO((getNodeFromPath("./CenterPoint")));
  hasUpdated &= point2DIO.update(pointer->getCentrePoint());

  pugi::xml_node rotationDirectionNode = getNodeFromPath("./RotationDirection");
  if (pointer->getIsClockwise()) {
    rotationDirectionNode.text().set("CW");
  } else {
    rotationDirectionNode.text().set("CCW");
  }  
  return hasUpdated;
}
