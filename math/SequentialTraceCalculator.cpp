// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <Robot.h>
#include "./BaseTraceCalculator.h"
#include "./RotationTraceCalculator.h"
#include "./LineTraceCalculator.h"
#include "./OTSA.h"
#include <RotationTrace.h>
//#include <ArduinoRobotTool.h> // will be necessary when we need to switch on the tool
#include "./SequentialTraceCalculator.h"


SequentialTraceCalculator::SequentialTraceCalculator()
  : SequentialTraceCalculator(nullptr, tsa::TraceSection()) {
}


SequentialTraceCalculator::SequentialTraceCalculator(const Robot::RobotPointer& i_robot)
  : SequentialTraceCalculator(i_robot, tsa::TraceSection()) {
}


/// Full Fledged Constructor
SequentialTraceCalculator::SequentialTraceCalculator(const Robot::RobotPointer& i_robot,
                                                     const tsa::TraceSection& i_section)
  : m_robot(i_robot), m_traceSection(i_section), m_stopAfterSection(true) {
}


/// Calculated Traces
void SequentialTraceCalculator::calculatedTraces() {
  // check if robot is alive
  if (!m_robot) {
    LOG_ERROR("Robot is not set yet!");
  }
  // check if there are sections
  if (m_traceSection.size() == 0) {
    return;
  }

  BaseTraceCalculator baseTraceCalculator(m_robot.get());
  RotationTraceCalculator rotationTraceCalculator(m_robot.get());
  LineTraceCalculator lineTraceCalculator(m_robot.get());

  tsa::otsa::orderVector(&m_traceSection,
			 m_robot->getVirtualPosition());
  for (const auto& trace : m_traceSection) {
    traceType distance  =
      Magnitude(trace->getStartPoint() - m_robot->getVirtualPosition());
    if (distance > baseTraceCalculator.getTolerance()) {
      Trace movementTrace(m_robot->getVirtualPosition(),
                          trace->getStartPoint());
      m_robot->switchTool(false);
      baseTraceCalculator.calculateTrace(movementTrace);
      m_robot->switchTool(true);
    }

    if (trace->getTraceType() == Trace::Line) {
      lineTraceCalculator.calculateTrace(*trace);
    } else if (trace->getTraceType() == Trace::Curve) {
      RotationTrace::RotationTracePointer rotationTrace =
        std::static_pointer_cast<RotationTrace>(trace);
      rotationTraceCalculator.calculateTrace(*rotationTrace);
    }
  }
}
