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

  // Set the first trace
  update();

}


void TraceInfoWidget::update() {
  LOG_DEBUG("Updating widget to show the info!");
  Trace::TracePointer tracePointer = m_trace.lock();
  if (!tracePointer) {
    LOG_DEBUG("No pointer set. Set points to nullptr");
    traceTypeComboBox->setEnabled(false);

    startPoint->setPoint(nullptr);
    emit startPoint->hasNewPosition();

    endPoint->setPoint(nullptr);
    emit endPoint->hasNewPosition();

    centralPoint->setPoint(nullptr);
    emit centralPoint->hasNewPosition();
  } else {

    LOG_DEBUG("Enable TraceType combobox if necessary");
    if (!traceTypeComboBox->isEnabled()) {
      traceTypeComboBox->setEnabled(true);
    }


    LOG_DEBUG("Update TraceType combobox if necessary");
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
    emit startPoint->hasNewPosition();

    if (points[1] != endPoint->getPoint()) {
      endPoint->setPoint(points[1]);
    }
    emit endPoint->hasNewPosition();

    if (points.size() < 3) {
      centralPoint->setPoint(nullptr);
    } else {
      centralPoint->setEnabled(true);
      centralPoint->setPoint(points[2]);
    }
    emit centralPoint->hasNewPosition();
  }
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
