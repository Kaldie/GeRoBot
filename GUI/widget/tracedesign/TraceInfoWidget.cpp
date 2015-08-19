// Copyright [2014] <Ruud Cools>

#include <macroHeader.h>
#include <Trace.h>
#include <QVBoxLayout>
#include "../Point2DWidget.h"
#include "./TraceInfoWidget.h"

TraceInfoWidget::TraceInfoWidget(QWidget *parent)
        : QWidget(parent) {
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
  startPoint = new Point2DWidget(this);
  this->layout()->addWidget(startPoint);
  endPoint = new Point2DWidget(this);
  this->layout()->addWidget(endPoint);
  centralPoint = new Point2DWidget(this);
  this->layout()->addWidget(centralPoint);

  startPoint->setPointName(QString("StartP{oint"));
  endPoint->setPointName(QString("EndPoint"));
  centralPoint->setPointName(QString("CentralPoint"));

  /**
   * Creating label and combo box for the type
   */
  traceTypeLabel = new QLabel("Trace Type", this);
  traceTypeComboBox = new QComboBox(this);
  traceTypeComboBox->addItem("Line",QVariant(Line));
  traceTypeComboBox->addItem("Curve",QVariant(Curve));
  this->layout()->addWidget(traceTypeLabel);
  this->layout()->addWidget(traceTypeComboBox);
  trace(std::shared_ptr<Trace>(NULL));
}

void TraceInfoWidget::trace(const TracePointer& i_tracePointer) {
  if (i_tracePointer) {
    LOG_DEBUG("Set a new Trace pointer");
    // enable trace combo box
    traceTypeComboBox->setEnabled(true);

    // set the trace
    m_trace = i_tracePointer;

    // update the combo box
    traceTypeComboBox->setCurrentIndex(m_trace->getTraceType());
    // Get the point pointers from the new trace
    std::vector<Point2D*> points;
    points = m_trace->getPointPointers();

    // Try to dynamicly cast it to a rotation trace
    RotationTracePointer rotationTracePointer =
      std::dynamic_pointer_cast<RotationTrace>(m_trace);
    startPoint->setPoint(points[0]);
    endPoint->setPoint(points[1]);
    if (points.size() < 2) {
      centralPoint->setEnabled(false);
    } else {
      centralPoint->setEnabled(true);
      centralPoint->setPoint(points[2]);
    }
  } else {
    traceTypeComboBox->setEnabled(false);
  }
}
