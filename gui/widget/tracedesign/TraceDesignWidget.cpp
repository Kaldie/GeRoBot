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
      m_vector(),
      m_traceInfoWidget(nullptr),
      m_traceGraphView(nullptr) {
    initialise();
}


void TraceDesignWidget::initialise() {
  setupUi(this);
  // create info widget and add to the design widget
  m_traceInfoWidget = new TraceInfoWidget(this);
  pointFrame->layout()->addWidget(m_traceInfoWidget);
  // Create view widget and add to design widget
  m_traceGraphView = new TraceGraphView(this);
  // We do not know if there is a layout....
  if (QLayout* layout = sceneFrame->layout()) {
    layout->addWidget(m_traceGraphView);
  } else {
    sceneFrame->setLayout(new QVBoxLayout);
    sceneFrame->layout()->addWidget(m_traceGraphView);
  }

  // add a single trace for debug puposes
  addTrace(std::make_shared<Trace>());
  m_vector[0]->setEndPoint(Point2D(50,10));


  // Connect the request new trace to replace trace
  connect(m_traceInfoWidget, SIGNAL(requestTrace(Trace::TraceType)),
          this, SLOT(replaceTrace(Trace::TraceType)));

  // connect the scenes changed signal to info widget update slot
  connect(m_traceGraphView->scene(), SIGNAL(changed(const QList<QRectF>&)),
          m_traceInfoWidget, SLOT(update()));

  // connect the scene the changed selection to the update trace from the info widget
  connect(m_traceGraphView->scene(), SIGNAL(selectionChanged()),
           this, SLOT(setSelectedTrace()));

  // connect the InfoWidget changed signal to the View widget update slot
  connect(m_traceInfoWidget, SIGNAL(tracePositionChanged()),
          m_traceGraphView, SLOT(updateSelectedItem()));

  // connect the add trace button to the add trace slots
  connect(addCurveButton,SIGNAL(clicked()),
          this, SLOT(addTraceFromButton()));
  connect(addLineButton,SIGNAL(clicked()),
          this, SLOT(addTraceFromButton()));
}


void TraceDesignWidget::replaceTrace(Trace::TraceType i_type) {
  LOG_DEBUG("Replacing the trace");
  // this will be the new trace pointer
  Trace::TracePointer tracePointer;

  // Create one with the correct type
  if(i_type == Trace::Line) {
    tracePointer = std::make_shared<Trace>();
  } else if (i_type == Trace::Curve) {
    tracePointer = std::make_shared<RotationTrace>();
  } else {
    LOG_ERROR("Trace type is unknown!");
  }
  // get the Trace::TracePointer from the vector
  int currentIndex = getSelectedIndex();
  if (currentIndex == m_vector.size()) {
    LOG_ERROR("did not find a selected item!");
  }
  // set the right start and stop point
  tracePointer->setStartPoint(m_vector[currentIndex]->getStartPoint());
  tracePointer->setEndPoint(m_vector[currentIndex]->getEndPoint());
  // Add the center point if it is a RotationTrace
  if (i_type == Trace::Curve) {
    RotationTrace::RotationTracePointer rotationTrace =
      std::dynamic_pointer_cast<RotationTrace>(tracePointer);
    assert(rotationTrace);
    // this will be the new center point.
    Point2D centerPoint;
    if (auto currentRotationTrace =
        std::dynamic_pointer_cast<RotationTrace>(m_vector[currentIndex])) {
      // it is a rotation trace, use that point
      centerPoint = currentRotationTrace->getCentrePoint();
    } else {
      //Suggest one
      centerPoint =
        RotationTrace::suggestCentralPoint(tracePointer->getStartPoint(),
                                           tracePointer->getEndPoint());
    }
    rotationTrace->setCentrePoint(centerPoint);
  }
  LOG_DEBUG("Current index : " << currentIndex);
  removeTrace(m_vector[currentIndex]);
  // remove the item from the view
  addTrace(tracePointer);
  //  LOG_DEBUG("Number of shared pointers to current trace: " << m_vector[currentIndex].use_count());
  LOG_DEBUG("The swaped is still hold by: " << tracePointer.use_count());
  LOG_DEBUG("Length of the vector is: " << m_vector.size());
}


void TraceDesignWidget::setSelectedTrace() {
  TraceGraphItem* item = m_traceGraphView->getSelectedTraceGraphItem();
  if (item) {
    m_traceInfoWidget->setNewTracePointer(item->getTracePointer());
  } else {
    m_traceInfoWidget->setNewTracePointer(nullptr);
  }
}


int TraceDesignWidget::getSelectedIndex() const {
  // create an empty Trace:TracePointer
  Trace::TracePointer currentTracePointer(nullptr);
  // if we can get the current selected TraceGraphItem
  if (TraceGraphItem* currentTraceItem =
      m_traceGraphView->getSelectedTraceGraphItem()) {
    // get its Trace::TracePointer
    Trace::TracePointer itemPointer = currentTraceItem->
      getTracePointer().lock();
    // search that pointer in the vector
    auto vectorIterator = std::find(m_vector.begin(),
                                    m_vector.end(),
                                    itemPointer);
    // if it is not found at the end of the vector
    if (vectorIterator != m_vector.end()) {
      return std::distance(m_vector.begin(), vectorIterator);
    } else {
      return m_vector.size();
    }
  }
  return m_vector.size();
}


void TraceDesignWidget::addTraceFromButton() {
  Trace::TracePointer newTrace;

  // use sender to check which button was pressed
  if (sender() == addLineButton) {
    newTrace = std::make_shared<Trace>(Point2D(0,0),
                                       Point2D(10,10));
  } else if (sender() == addCurveButton) {
    newTrace = std::make_shared<RotationTrace>(Point2D(-10,0),
                                               Point2D(10,0),
                                               Point2D(0,0));
  } else {
    LOG_DEBUG("Sender did not return a known button!");
  }
  addTrace(newTrace);
}


void TraceDesignWidget::addTrace(Trace::TracePointer newTrace) {
 // push the new trace to the vector
  m_vector.push_back(newTrace);
  // make a new trace graph item and push it to the view
  TraceGraphItem* item = m_traceGraphView->addTraceItem(newTrace);
  setSelectedTrace();
  LOG_DEBUG("Bad thingy: " << item);
  //  connect(item, SIGNAL(removeThis(Trace::TracePointer),
  //                   this, SLOT(removeTrace(Trace::TracePointer);
}


void TraceDesignWidget::removeTrace(Trace::TracePointer i_pointer) {
  auto vectorIterator = std::find(m_vector.begin(),
                                  m_vector.end(),
                                  i_pointer);
  if (vectorIterator == m_vector.end()) {
    return;
  }
  if (i_pointer->getTraceType()==Trace::Curve)
    LOG_DEBUG("Trying to remove a curve");
  else
    LOG_DEBUG("Trying to remove a line");
  // remove the item from the view
  m_traceGraphView->removeTraceItem(i_pointer);
  // remove the item from the info widget
  m_traceInfoWidget->setNewTracePointer(nullptr);
  // remove the item from the vector
  m_vector.erase(vectorIterator);
}
