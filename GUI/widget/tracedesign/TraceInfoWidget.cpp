// Copyright [2014] <Ruud Cools>

#include <macroHeader.h>
#include <Trace.h>
#include <QVBoxLayout>
#include "../Point2DWidget.h"
#include "./TraceInfoWidget.h"

TraceInfoWidget::TraceInfoWidget(QWidget *parent)
  : QWidget(parent), m_trace(NULL) {
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

void TraceInfoWidget::setTrace(TracePointer i_tracePointer) {
  m_trace = i_tracePointer;
  if (!m_trace) {
    LOG_DEBUG("No pointer set. return after disableing");
    traceTypeComboBox->setEnabled(false);
    startPoint->setPoint(NULL);
    endPoint->setPoint(NULL);
    centralPoint->setPoint(NULL);
  } else {
    LOG_DEBUG("Set a new Trace pointer");
    // enable trace combo box
    traceTypeComboBox->setEnabled(true);

    LOG_DEBUG("Set the trace type to: " << i_tracePointer->getTraceType());
    traceTypeComboBox->setCurrentIndex(m_trace->getTraceType());
    // Get the point pointers from the new trace
    std::vector<Point2D*> points;
    LOG_DEBUG("Getting points now!");
    points = m_trace->getPointPointers();

    startPoint->setPoint(points[0]);
    endPoint->setPoint(points[1]);
    LOG_DEBUG("Finished setting of start and end point.");
    if (points.size() < 2) {
      LOG_DEBUG("Cannot set center point, it is a line trace!");
      centralPoint->setEnabled(false);
    } else {
      centralPoint->setEnabled(true);
      centralPoint->setPoint(points[2]);
      centralPoint->updateView();
    }
  }
  LOG_DEBUG("Emit has new position");
  emit startPoint->hasNewPosition();
  emit endPoint->hasNewPosition();
  emit centralPoint->hasNewPosition();
}

void TraceInfoWidget::updateTrace() {
  LOG_DEBUG("Update trace!");
  if (traceTypeComboBox->currentIndex() == m_trace->getTraceType())
    LOG_DEBUG("Trace does not have to be reset!");

}
