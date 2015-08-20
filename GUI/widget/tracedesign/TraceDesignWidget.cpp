// Copyright [2014] <Ruud Cools>

#include <QtGui>
#include <macroHeader.h>
#include "../Point2DWidget.h"
#include "./TraceDesignWidget.h"

TraceDesignWidget::TraceDesignWidget(QWidget *parent)
    : QWidget(parent),
      m_currentTrace(nullptr),
      m_vector(),
      m_traceInfoWidget(nullptr) {
    m_vector.push_back(std::make_shared<Trace>());
    initialise();
}


void TraceDesignWidget::initialise() {
    setupUi(this);
    pointFrame->layout()->addWidget(new TraceInfoWidget(this));

    m_traceInfoWidget = pointFrame->findChild<TraceInfoWidget*>();
    assert(m_traceInfoWidget);
    connect(m_traceInfoWidget, SIGNAL(requestTrace(Trace::TraceType)),
            this, SLOT(replaceTrace(Trace::TraceType)));
    // connect(trace);
    m_currentTrace = m_vector[0];
    m_traceInfoWidget->setNewTracePointer(m_vector[0]);
}


void TraceDesignWidget::replaceTrace(Trace::TraceType i_type) {
  LOG_DEBUG("Replacing the trace");
  assert(m_currentTrace);

  Trace::TracePointer tracePointer;
  if(i_type == Trace::Line) {
    LOG_DEBUG("Create new Line trace");
    tracePointer = std::make_shared<Trace>();
  } else if (i_type == Trace::Curve) {
    tracePointer = std::make_shared<RotationTrace>();
  } else {
      LOG_ERROR("Trace is bad");
  }

  LOG_DEBUG("Setting points.");
  tracePointer->setStartPoint(m_currentTrace->getStartPoint());
  tracePointer->setEndPoint(m_currentTrace->getEndPoint());

  if (i_type == Trace::Curve) {
      RotationTrace::RotationTracePointer rotationTrace =
          std::dynamic_pointer_cast<RotationTrace>(tracePointer);
      // For some reason we need to test the dynamic cast
      assert(rotationTrace);
      LOG_DEBUG("StartPoint: " << rotationTrace->getStartPoint().x << ", " << rotationTrace->getStartPoint().y);
      LOG_DEBUG("EndPoint: " << rotationTrace->getEndPoint().x << ", " << rotationTrace->getEndPoint().y);
      // this will be the new center point.
      Point2D centerPoint;
      RotationTrace::RotationTracePointer currentRotationTrace =
          std::dynamic_pointer_cast<RotationTrace>(m_currentTrace);
      if (currentRotationTrace) {
          // it is a rotation trace, use that point
          centerPoint = currentRotationTrace->getArc().getCentrePoint();
      } else {
          //Suggest one
          centerPoint = RotationTrace::suggestCentralPoint(tracePointer->getStartPoint(),
                                                           tracePointer->getEndPoint());
      }
      Arc2D arc(rotationTrace->getArc());
      arc.setCentrePoint(centerPoint);
      rotationTrace->setArc(arc);
  }
  m_currentTrace.swap(tracePointer);
  m_traceInfoWidget->setNewTracePointer(m_currentTrace);
}


void TraceDesignWidget::setCurrentTrace(Trace::WeakTracePointer i_currentTrace) {
    m_currentTrace = i_currentTrace.lock();
}
