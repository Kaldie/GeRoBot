// Copyright [2014] <Ruud Cools>

#include <macroHeader.h>
#include <Trace.h>
#include <Circle2D.h>
#include <RotationTrace.h>
#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include "../Point2DWidget.h"
#include "./TraceInfoWidget.h"

TraceInfoWidget::TraceInfoWidget(QWidget *parent /*=0*/,
                                 Trace::TracePointer pointer /*=nullptr*/)
  :QWidget(parent) ,m_trace(pointer) {
  initialise();
}


void TraceInfoWidget::initialise() {
  /**
   * creating points for the trace
   */
  /// Remove layout if installed
  if (this->layout()) {
    delete this->layout();
  }
  /// Set VBox layout
  this->setLayout(new QVBoxLayout);

  /// Add the start, stop and central point widget
  startPoint = new Point2DWidget(this);
  this->layout()->addWidget(startPoint);
  endPoint = new Point2DWidget(this);
  this->layout()->addWidget(endPoint);
  centralPoint = new Point2DWidget(this);
  this->layout()->addWidget(centralPoint);

  //set names of the points
  startPoint->setPointName(QString("StartP{oint"));
  endPoint->setPointName(QString("EndPoint"));
  centralPoint->setPointName(QString("CentralPoint"));

  /**
   * Creating label and combo box for the type
   */
  traceTypeLabel = new QLabel("Trace Type", this);
  traceTypeComboBox = new QComboBox(this);
  traceTypeComboBox->addItem("Line",QVariant(Trace::Line));
  traceTypeComboBox->addItem("Curve",QVariant(Trace::Curve));
  this->layout()->addWidget(traceTypeLabel);
  this->layout()->addWidget(traceTypeComboBox);

  // if the combo box is changed, check if the m_trace is the same type as the current
  // if not emit a signal that we need a new trace of the right type
  connect(traceTypeComboBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(shouldRequestTraceUpdate()));

  // If the points are changed they emit a hasNewPosition signal
  // This signal is connected to a slot which determins if it is a valid update()
  // If so, emit hasTraceUpdate()
  // if not, set the Point2DWidget to invalide and update to show it
  // Set the first trace
  connect(startPoint, SIGNAL(hasNewPosition()),
          this, SLOT(shouldSignalHasNewPosition()));
  connect(endPoint, SIGNAL(hasNewPosition()),
          this, SLOT(shouldSignalHasNewPosition()));
  connect(centralPoint, SIGNAL(hasNewPosition()),
          this, SLOT(shouldSignalHasNewPosition()));

  update();
}


void TraceInfoWidget::update() {
  LOG_DEBUG("Updating widget to show the info!");
  Trace::TracePointer tracePointer = m_trace.lock();
  // block the signals so no infinite loop occurs
  startPoint->blockSignals(true);
  endPoint->blockSignals(true);
  centralPoint->blockSignals(true);
  if (!tracePointer) {
    LOG_DEBUG("No pointer set. Set points to nullptr");
    traceTypeComboBox->setEnabled(false);

    startPoint->setPoint(nullptr);
    endPoint->setPoint(nullptr);
    centralPoint->setPoint(nullptr);

    startPoint->update();
    endPoint->update();
    centralPoint->update();
  } else {
    if (!traceTypeComboBox->isEnabled()) {
      traceTypeComboBox->setEnabled(true);
    }
    if (traceTypeComboBox->currentIndex() != tracePointer->getTraceType()) {
      traceTypeComboBox->setCurrentIndex(tracePointer->getTraceType());
    }
    // Get the point pointers from the new trace
    LOG_DEBUG("Getting points now!");
    std::vector<Point2D*> points(tracePointer->getPointPointers());

    if (points[0] != startPoint->getPoint()) {
      startPoint->setPoint(points[0]);
      LOG_DEBUG("Emit startPoint has new position");
    }
    if (points[1] != endPoint->getPoint()) {
      endPoint->setPoint(points[1]);
    }
    if (points.size() < 3) {
      centralPoint->setPoint(nullptr);
    } else {
      if (points[3] != endPoint->getPoint()) {
        centralPoint->setPoint(points[2]);
      }
    }
    startPoint->update();
    endPoint->update();
    centralPoint->update();
  }
  // re-enable the signals
  startPoint->blockSignals(false);
  endPoint->blockSignals(false);
  centralPoint->blockSignals(false);
}


void TraceInfoWidget::shouldRequestTraceUpdate() {
  Trace::TracePointer tracePointer = m_trace.lock();
  LOG_DEBUG("ShouldRequestTraceUpdate()");
  if (tracePointer) {
    LOG_DEBUG("Test if the new trace has the same type as the current.");
    if (traceTypeComboBox->currentIndex() !=
        tracePointer->getTraceType()) {
      LOG_DEBUG("traceTypeComboBox indicate a different type is needed!");
      emit requestTrace(static_cast<Trace::TraceType>(traceTypeComboBox->currentIndex()));
    }
  }
}


void TraceInfoWidget::setNewTracePointer(const Trace::TracePointer& i_tracePointer) {
  m_trace = i_tracePointer;
  update();
}


void TraceInfoWidget::setNewTracePointer(const Trace::WeakTracePointer& i_tracePointer) {
  m_trace = i_tracePointer;
  update();
}


void TraceInfoWidget::shouldSignalHasNewPosition() {
  // if the weak pointer points to something nice
  Trace::TracePointer trace = m_trace.lock();
  if (!trace) {
    return;
  }

  //if it is a line, it will work
  if (trace->getTraceType() == Trace::Line) {
    emit tracePositionChanged();
    return;
  }

  // if it is a curve
  if (trace->getTraceType() == Trace::Curve) {
    RotationTrace::RotationTracePointer rotationPointer =
      std::dynamic_pointer_cast<RotationTrace>(trace);
    assert(rotationPointer);
    // check if a circle can be made
    try {
      Arc2D arc(trace->getStartPoint(),
                trace->getEndPoint(),
                rotationPointer->getCentrePoint());
      if (!traceTypeComboBox->isEnabled()) {
          traceTypeComboBox->setEnabled(true);
      }
      emit tracePositionChanged();
    } catch(std::runtime_error) {
      traceTypeComboBox->setEnabled(false);
      return;
    }
  }
}
