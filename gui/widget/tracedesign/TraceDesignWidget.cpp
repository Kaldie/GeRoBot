// Copyright [2014] <Ruud Cools>

#include <QtGui>
#include <QtWidgets>
#include <QVBoxLayout>
#include <macroHeader.h>
#include "../Point2DWidget.h"
#include "./TraceDesignWidget.h"
#include "./TraceGraphItem.h"
#include "./TraceGraphView.h"
#include "./RotationTraceGraphItem.h"


TraceDesignWidget::TraceDesignWidget(QWidget *parent)
    : QWidget(parent),
      m_index(0),
      m_vector(),
      m_traceInfoWidget(nullptr),
      m_traceGraphView(nullptr) {
    initialise();
}


void TraceDesignWidget::initialise() {
  setupUi(this);
  m_traceInfoWidget = new TraceInfoWidget(this);
  pointFrame->layout()->addWidget(m_traceInfoWidget);

  m_vector.push_back(std::make_shared<Trace>());
  m_index = 0;
  m_vector[m_index]->setEndPoint(Point2D(50,10));
  m_traceInfoWidget->setNewTracePointer(m_vector[m_index]);

  m_traceGraphView = new TraceGraphView(this);
  m_traceGraphView->addTraceItem(m_vector[m_index]);

  QLayout* layout = sceneFrame->layout();
  if (!layout) {
    sceneFrame->setLayout(new QVBoxLayout);
    layout = sceneFrame->layout();
  }
  layout->addWidget(m_traceGraphView);

  // Connect the request new trace to replace trace
  connect(m_traceInfoWidget, SIGNAL(requestTrace(Trace::TraceType)),
          this, SLOT(replaceTrace(Trace::TraceType)));

  // connect the scenes changed signal to info widget update slot
  connect(m_traceGraphView->scene(), SIGNAL(changed(const QList<QRectF>&)),
          m_traceInfoWidget, SLOT(update()));
}


void TraceDesignWidget::replaceTrace(Trace::TraceType i_type) {
  LOG_DEBUG("Replacing the trace");

  Trace::TracePointer tracePointer;
  if(i_type == Trace::Line) {
    tracePointer = std::make_shared<Trace>();
  } else if (i_type == Trace::Curve) {
    tracePointer = std::make_shared<RotationTrace>();
  } else {
    LOG_ERROR("Trace is bad");
  }

  tracePointer->setStartPoint(m_vector[m_index]->getStartPoint());
  tracePointer->setEndPoint(m_vector[m_index]->getEndPoint());
  TraceGraphItem* newGraphItem;
  if (i_type == Trace::Curve) {
    // this will be the new center point.
    Point2D centerPoint;
    RotationTrace::RotationTracePointer rotationTrace =
      std::dynamic_pointer_cast<RotationTrace>(tracePointer);
    assert(rotationTrace);
    if (auto currentRotationTrace = std::dynamic_pointer_cast<RotationTrace>(m_vector[m_index])) {
      // it is a rotation trace, use that point
      centerPoint = currentRotationTrace->getCentrePoint();
    } else {
      //Suggest one
      centerPoint = RotationTrace::suggestCentralPoint(tracePointer->getStartPoint(),
                                                       tracePointer->getEndPoint());
    }
    // For some reason we need to test the dynamic cast
    rotationTrace->setCentrePoint(centerPoint);
    newGraphItem = new RotationTraceGraphItem(rotationTrace);
  } else {
    newGraphItem = new TraceGraphItem(tracePointer);
  }
  // swap the old with the new
  m_vector[m_index].swap(tracePointer);

  // update the info widget to show the new info
  m_traceInfoWidget->setNewTracePointer(m_vector[m_index]);


  QList<QGraphicsItem*> list =  m_traceGraphView->scene()->selectedItems();
  TraceGraphItem* currentItem;
  if(list.size() != 1) {
    currentItem = dynamic_cast<TraceGraphItem*>(m_traceGraphView->scene()->items()[0]);
    LOG_INFO("BooBo!!");
  } else {
    currentItem = dynamic_cast<TraceGraphItem*>(list[0]);
  }
  m_traceGraphView->scene()->removeItem(currentItem);
  delete currentItem;
  m_traceGraphView->scene()->addItem(newGraphItem);

  LOG_DEBUG("Number of shared pointers to current trace: " << m_vector[m_index].use_count());
  LOG_DEBUG("The swaped is still hold by: " << tracePointer.use_count());
}


void TraceDesignWidget::setCurrentIndex(const int& i_newIndex) {
  m_index = i_newIndex;
}
