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
  RotationTrace::RotationTracePointer trace = std::make_shared<RotationTrace>();
  trace->setStartPoint(Point2D(-100,0));
  trace->setEndPoint(Point2D(100,0));
  trace->setCentrePoint(Point2D(0,0));
  addTrace(trace);
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


void TraceDesignWidget::setSelectedTrace() {
  TraceGraphItem* item = m_traceGraphView->getSelectedTraceGraphItem();
  if (item) {
    m_traceInfoWidget->setNewTracePointer(item->getTracePointer());
  } else {
    m_traceInfoWidget->setNewTracePointer(nullptr);
  }
}


int TraceDesignWidget::getSelectedIndex() const {
  // if we can get the current selected TraceGraphItem
  if (TraceGraphItem* currentTraceItem =
      m_traceGraphView->getSelectedTraceGraphItem()) {
    // get its Trace::TracePointer
    Trace::TracePointer itemPointer = currentTraceItem->
      getTracePointer().lock();
    return getIndex(itemPointer);
  } else {
    return m_vector.size();
  }
}


int TraceDesignWidget::getIndex(const Trace::TracePointer& i_pointer) const {
  // check if the pointer is valid
  assert(i_pointer);
  // search that pointer in the vector
  auto vectorIterator = std::find(m_vector.begin(),
                                  m_vector.end(),
                                  i_pointer);
  // if it is not found at the end of the vector
  if (vectorIterator != m_vector.end()) {
    return std::distance(m_vector.begin(), vectorIterator);
  } else {
    return m_vector.size();
  }
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
  // connect the remove this trace signal to remove trace from this widget
  connect(item, SIGNAL(removeThisTrace(Trace::TracePointer)),
          this, SLOT(removeTrace(Trace::TracePointer)));
  // connect the convert to something signal to the replaceTrace slot
  connect(item, SIGNAL(convertThisTrace(Trace::TracePointer, Trace::TraceType)),
          this, SLOT(replaceTrace(Trace::TracePointer,Trace::TraceType)));
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


// replace the currenty selected trace
void TraceDesignWidget::replaceTrace(Trace::TraceType i_type) {
  LOG_DEBUG("Replace trace based on type. Type: " << i_type);
  replaceTrace(getSelectedIndex(), i_type);
}


// replace the trace which is indicated by the pointer
void TraceDesignWidget::replaceTrace(Trace::TracePointer i_pointer,
                                     Trace::TraceType i_type) {
  LOG_DEBUG("Replace trace based on pointer and type");
  replaceTrace(getIndex(i_pointer), i_type);
}


// Replace the trace based on the index and type
void TraceDesignWidget::replaceTrace(const int& i_index,
                                     const Trace::TraceType& i_type) {
  LOG_DEBUG("Replacing the trace");
  if (i_index == m_vector.size()) {
    LOG_ERROR("did not find a selected item!");
  }
  // this will be the new trace pointer
  Trace::TracePointer tracePointer;
  // Create one with the correct type
  if(i_type == Trace::Line) {
    LOG_DEBUG("Make a Line");
    tracePointer = std::make_shared<Trace>();
  } else if (i_type == Trace::Curve) {
    LOG_DEBUG("Make a Curve");
    tracePointer = std::make_shared<RotationTrace>();
  } else {
    LOG_ERROR("Trace type is unknown: " << i_type);
  }
  // create the perfect replacement
  setupReplacementPointer(tracePointer, i_index);
  // remove the old
  removeTrace(m_vector[i_index]);
  // add the new
  addTrace(tracePointer);
  LOG_DEBUG("Current index : " << i_index);
  LOG_DEBUG("New type: " << m_vector[i_index]->getTraceType());
  //  LOG_DEBUG("Number of shared pointers to current trace: " << m_vector[currentIndex].use_count());
  LOG_DEBUG("The swaped is still hold by: " << tracePointer.use_count());
  LOG_DEBUG("Length of the vector is: " << m_vector.size());

}

void TraceDesignWidget::setupReplacementPointer(Trace::TracePointer i_replacement,
                                                const int& i_index) {
  // set the right start and stop point
  i_replacement->setStartPoint(m_vector[i_index]->getStartPoint());
  i_replacement->setEndPoint(m_vector[i_index]->getEndPoint());
  // if it is a line: we are done
  if (i_replacement->getTraceType() == Trace::Line) {
    return;
  }

  // if it is a RotationTrace we need to set the center point
  if (i_replacement->getTraceType() == Trace::Curve) {
    RotationTrace::RotationTracePointer rotationTrace =
      std::dynamic_pointer_cast<RotationTrace>(i_replacement);
    assert(rotationTrace);
    // this will be the new center point.
    Point2D centerPoint;
    if (auto currentRotationTrace =
        std::dynamic_pointer_cast<RotationTrace>(m_vector[i_index])) {
      // it is a rotation trace, use that point
      centerPoint = currentRotationTrace->getCentrePoint();
      // if we can get the arc we are done.
      try {
        rotationTrace->getArc();
        return;
      } catch (std::runtime_error) {
        // pass
      }
    }
    // Could not make the arc, suggest a central point and finally finish
    centerPoint =
      RotationTrace::suggestCentralPoint(i_replacement->getStartPoint(),
                                         i_replacement->getEndPoint());
    rotationTrace->setCentrePoint(centerPoint);
  }
}
