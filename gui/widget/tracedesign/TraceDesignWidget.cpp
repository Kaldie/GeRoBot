// Copyright [2014] <Ruud Cools>

#include <QtGui>
#include <QtWidgets>
#include <QVBoxLayout>
#include <macroHeader.h>
#include "../Point2DWidget.h"
#include "./TraceDesignWidget.h"
#include "./TraceGraphItem.h"


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
  connect(m_traceInfoWidget, SIGNAL(requestTrace(Trace::TraceType)),
          this, SLOT(replaceTrace(Trace::TraceType)));

  m_vector.push_back(std::make_shared<Trace>());
  m_index = 0;
  m_vector[m_index]->setEndPoint(Point2D(50,10));
  m_traceInfoWidget->setNewTracePointer(m_vector[m_index]);

  QGraphicsScene* scene = new QGraphicsScene;
  scene->setSceneRect(-100,-100,200,200);
  scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
  scene->addItem(new TraceGraphItem(m_vector[m_index]));
  m_traceGraphView = new QGraphicsView();
  m_traceGraphView->setScene(scene);
  m_traceGraphView->setRenderHint(QPainter::Antialiasing);
  //  m_traceGraphView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  //  m_traceGraphView->setDragMode(QGraphicsView::ScrollHandDrag);
  LOG_DEBUG("Been here!");
  QLayout* layout = sceneFrame->layout();
  if (!layout) {
    sceneFrame->setLayout(new QVBoxLayout);
    layout = sceneFrame->layout();
  }
  layout->addWidget(m_traceGraphView);

  LOG_DEBUG("Number of shared pointers to current trace: " << m_vector[m_index].use_count());
  QTimer* time = new QTimer();
  time->setInterval(500);
  connect(time,SIGNAL(timeout()),
          scene,SLOT(update()));
  time->start();
}


void TraceDesignWidget::replaceTrace(Trace::TraceType i_type) {
  LOG_DEBUG("Replacing the trace");

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
  tracePointer->setStartPoint(m_vector[m_index]->getStartPoint());
  tracePointer->setEndPoint(m_vector[m_index]->getEndPoint());

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
      std::dynamic_pointer_cast<RotationTrace>(m_vector[m_index]);
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


  m_vector[m_index].swap(tracePointer);
  m_traceInfoWidget->setNewTracePointer(m_vector[m_index]);
  QList<QGraphicsItem*> list =  m_traceGraphView->scene()->selectedItems();
  if(list.size() != 1) {
    dynamic_cast<TraceGraphItem*>(m_traceGraphView->scene()->items()[0])->setTrace(m_vector[m_index]);
    LOG_INFO("BooBo!!");
  } else {
  dynamic_cast<TraceGraphItem*>(list[0])->setTrace(m_vector[m_index]);
  }
  LOG_DEBUG("Number of shared pointers to current trace: " << m_vector[m_index].use_count());
  LOG_DEBUG("The swaped is still hold by: " << tracePointer.use_count());
}


void TraceDesignWidget::setCurrentIndex(const int& i_newIndex) {
  m_index = i_newIndex;
}
