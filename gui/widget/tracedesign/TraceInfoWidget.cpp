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
                                 Trace::TracePointer pointer /*nullptr*/)
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

  // Set the first trace, which is a null ptr
  showTraceInfo();

  connect(traceTypeComboBox,SIGNAL(currentIndexChanged(int)),
          this, SLOT(update()));
  /*
    connect(startPoint,SIGNAL(hasNewPosition()),
          this, SLOT(update()));
  connect(endPoint,SIGNAL(hasNewPosition()),
          this, SLOT(update()));
  connect(centralPoint,SIGNAL(hasNewPosition()),
          this, SLOT(update()));
  */
}


void TraceInfoWidget::showTraceInfo() {
  LOG_DEBUG("Show trace info");
  Trace::TracePointer tracePointer = m_trace.lock();
  if (!tracePointer) {
    LOG_DEBUG("No pointer set. Set points to nullptr");
    traceTypeComboBox->setEnabled(false);
    startPoint->setPoint(nullptr);
    endPoint->setPoint(nullptr);
    centralPoint->setPoint(nullptr);
  } else {
    // enable trace combo box
    traceTypeComboBox->setEnabled(true);

    LOG_DEBUG("Set the trace type to: " << tracePointer->getTraceType());
    traceTypeComboBox->setCurrentIndex(tracePointer->getTraceType());
    // Get the point pointers from the new trace
    LOG_DEBUG("Getting points now!");
    std::vector<Point2D*> points(tracePointer->getPointPointers());

    startPoint->setPoint(points[0]);
    endPoint->setPoint(points[1]);
    if (points.size() <= 2) {
      centralPoint->setEnabled(false);
      centralPoint->setPoint(nullptr);
    } else {
      centralPoint->setEnabled(true);
      centralPoint->setPoint(points[2]);
    }
  }
  LOG_DEBUG("Emit has new position");
  emit startPoint->hasNewPosition();
  emit endPoint->hasNewPosition();
  emit centralPoint->hasNewPosition();
}


void TraceInfoWidget::update() {
  LOG_DEBUG("Update");
  Trace::TracePointer tracePointer = m_trace.lock();

  if (tracePointer) {
    updateCurrentTraceFromWidget(tracePointer);

    LOG_DEBUG("Test if the new trace has the same type as the current.");
    if (traceTypeComboBox->currentIndex() !=
        tracePointer->getTraceType()) {
      LOG_DEBUG("traceTypeComboBox indicate a different type is needed!");
      LOG_DEBUG("Current start point" <<
                tracePointer->getStartPoint().x << ", " << tracePointer->getStartPoint().y);
      LOG_DEBUG("Current end point" <<
                tracePointer->getEndPoint().x << ", " << tracePointer->getEndPoint().y);
      emit requestTrace(static_cast<Trace::TraceType>(traceTypeComboBox->currentIndex()));
      return;
    }
  }
  showTraceInfo();
  QWidget::update();
}


void TraceInfoWidget::setNewTracePointer(Trace::TracePointer& i_tracePointer) {
  m_trace = i_tracePointer;
  showTraceInfo();
}


void TraceInfoWidget::updateCurrentTraceFromWidget(Trace::TracePointer& i_tracePointer) {
  if (!i_tracePointer)
    return;

  i_tracePointer->setStartPoint(*startPoint->getPoint());
  i_tracePointer->setEndPoint(*endPoint->getPoint());

  RotationTrace::RotationTracePointer rotationPointer =
    std::dynamic_pointer_cast<RotationTrace>(i_tracePointer);
  if(rotationPointer) {
    if (centralPoint->getPoint()) {
      Arc2D arc(rotationPointer->getArc());
      arc.setCentrePoint(*centralPoint->getPoint());
      rotationPointer->setArc(arc);
    }
  }
}
