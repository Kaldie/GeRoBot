// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <RotationTrace.h>
#include "./RotationTraceIO.h"
#include "./Point2DIO.h"
#include <Arc2D.h>

void RotationTraceIO::build() {
  LOG_DEBUG("Building a RotationTrace");
  TraceIO::build();
  Point2DIO point2DIO(getNodeFromPath("./CenterPoint"));
  point2DIO.build();
  RotationTrace::RotationTracePointer pointer =
    std::dynamic_pointer_cast<RotationTrace>(getTracePointer());
  if (!pointer) {
    LOG_ERROR("Dynamic cast did not go well!");
  }
  pointer->setCentrePoint(point2DIO.getPoint2D());
  setTracePointer(pointer);
}



RotationTraceIO::RotationTraceIO(const pugi::xml_node& i_node)
  : TraceIO(i_node)
{}


bool RotationTraceIO::update(const Trace::TracePointer& i_trace) {
  LOG_DEBUG("Updateing Rotation Trace");
  bool hasUpdated = TraceIO::update(i_trace);
  RotationTrace::RotationTracePointer pointer =
    std::dynamic_pointer_cast<RotationTrace>(i_trace);
  if (!pointer) {
    LOG_ERROR("Dynamic cast went wrong. Which is very unexpected!");
  }
  //Point2D centerPoint = pointer->getArc().getCentrePoint();
  Point2DIO point2DIO((getNodeFromPath("./CenterPoint")));
  hasUpdated &= point2DIO.update(pointer->getArc().getCentrePoint());
  return hasUpdated;
}
