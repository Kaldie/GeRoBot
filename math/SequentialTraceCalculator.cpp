// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <Robot.h>
#include "./BaseTraceCalculator.h"
#include "./RotationTraceCalculator.h"
#include "./LineTraceCalculator.h"
#include "./OTSA.h"
#include <RotationTrace.h>
#include <Robot.h>
//#include <ArduinoRobotTool.h> // will be necessary when we need to switch on the tool
#include "./SequentialTraceCalculator.h"


SequentialTraceCalculator::SequentialTraceCalculator()
  : SequentialTraceCalculator(nullptr, tsa::TraceSection(), true, false) {
}


SequentialTraceCalculator::SequentialTraceCalculator(const Robot::RobotPointer& i_robot)
  : SequentialTraceCalculator(i_robot, tsa::TraceSection(), true, false) {
}


/// Full Fledged Constructor
SequentialTraceCalculator::SequentialTraceCalculator(const Robot::RobotPointer& i_robot,
                                                     const tsa::TraceSection& i_section)
  : SequentialTraceCalculator(i_robot, i_section, true, false) {
}

SequentialTraceCalculator::SequentialTraceCalculator(const Robot::RobotPointer& i_robot,
                                                     const tsa::TraceSection& i_section,
                                                     const bool& i_stopAfterSection,
                                                     const bool& i_orderTraces)
  : m_robot(i_robot),
    m_traceSection(i_section),
    m_stopAfterSection(i_stopAfterSection),
    m_orderTraces(i_orderTraces) {
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

  //TODO: fix it so the settings are passed to the other calculators
  baseTraceCalculator.setWriteLog(true);
  rotationTraceCalculator.setWriteLog(true);
  lineTraceCalculator.setWriteLog(true);

  if (m_orderTraces) {
    tsa::otsa::orderVector(&m_traceSection,
                           m_robot->getPosition());
  }

  for (const auto& trace : m_traceSection) {
    LOG_DEBUG("Calculating a trace!");
    traceType distance  =
      magnitude(trace->getStartPoint() - m_robot->getVirtualPosition());
    LOG_DEBUG("Distance from the current position and the start point: " << distance);
    if (distance > baseTraceCalculator.getTolerance()) {
      Trace movementTrace(m_robot->getVirtualPosition(),
                          trace->getStartPoint());
      //m_robot->switchTool(false);
      baseTraceCalculator.calculateTrace(movementTrace);
      //m_robot->switchTool(true);
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
