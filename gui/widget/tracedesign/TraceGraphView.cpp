//Copyright [2015] Ruud Cools

#include <macroHeader.h>
#include <Trace.h>
#include <RotationTrace.h>
#include <QPen>
#include "./TraceGraphView.h"
#include "./RotationTraceGraphItem.h"
#include "./TraceGraphPoint.h"


const double TraceGraphView::LegendRatio = 0.2;
const int TraceGraphView::OptimalNumberOfLines = 1250;

TraceGraphView::TraceGraphView(QWidget* i_parent)
  : QGraphicsView(i_parent), m_gridSpacing(50) {
  initialise();
}


void TraceGraphView::initialise() {
  QGraphicsScene* scene = new QGraphicsScene;

  //  scene->setSceneRect(-100,-300,200,300);
  scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
  setScene(scene);
  setRenderHint(QPainter::Antialiasing);
  setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  setDragMode(QGraphicsView::RubberBandDrag);
  setCacheMode(QGraphicsView::CacheNone);
  //  connect(
}


TraceGraphItem* TraceGraphView::addTraceItem(const Trace::TracePointer& i_trace) {
  if (!i_trace) {
    setSelected(nullptr);
    return nullptr;
  }

  TraceGraphItem* newGraphItem;
  if (i_trace->getTraceType() == Trace::Curve) {
    RotationTrace::RotationTracePointer rotationTrace =
      std::dynamic_pointer_cast<RotationTrace>(i_trace);
    assert(rotationTrace);
    newGraphItem = new RotationTraceGraphItem(rotationTrace);
  } else {
    newGraphItem = new TraceGraphItem(i_trace);
  }
  newGraphItem->setSelected(true);
  scene()->addItem(newGraphItem);
  return newGraphItem;
}


bool TraceGraphView::removeTraceItem(const Trace::TracePointer& i_trace) {
  bool hasRemoved(false);
  TraceGraphItem* item(findItem(i_trace));
  if (item) {
    // remove item from the scene and receive ownership
    scene()->removeItem(item);
    if (item->getTracePointer().lock()->getTraceType() == Trace::Curve)
      LOG_DEBUG("Removed a curve");
    if (item->getTracePointer().lock()->getTraceType() == Trace::Line)
      LOG_DEBUG("Removed a line");
    // delete from heap
    delete item;
    // indicate that we have deleted an item
    hasRemoved = true;
  } else {
    LOG_DEBUG("Trace was not found and therfor not removed");
  }
  return hasRemoved;
}


bool TraceGraphView::replaceCurrentTrace(const Trace::TracePointer& i_newTrace) {
  QList<QGraphicsItem*> list =  scene()->selectedItems();
  TraceGraphItem* currentItem;
  if(list.size() != 1) {
    LOG_INFO("No item, or multiple items are selected not possible to replace them all!!");
    return false;
  } else {
    currentItem = dynamic_cast<TraceGraphItem*>(list[0]);
  }
  scene()->removeItem(currentItem);
  delete currentItem;
  addTraceItem(i_newTrace);
  return true;
}


void TraceGraphView::updateSelectedItem() {
  if (TraceGraphItem* item = getSelectedTraceGraphItem()) {
    item->updatePosition();
  } else {
    LOG_DEBUG("Could not find the item");
  }
}


void TraceGraphView::setSelected(const Trace::TracePointer& i_trace) {
  TraceGraphItem* item(nullptr);
  item = findItem(i_trace);
  // see if the item is actually found
  if (!item) {
    return;
  }
  scene()->clearSelection();
  item->setSelected(true);
}

void TraceGraphView::drawBackground(QPainter *painter, const QRectF &i_backgroundRect) {
  QPen pen(Qt::DashLine);
  QColor color(Qt::blue);
  pen.setColor(color.lighter(180));
  pen.setCapStyle(Qt::RoundCap);
  painter->setPen(pen);

  // check which is the biggest rect and use that 1
  LOG_DEBUG("Size of background rect: " << i_backgroundRect.width() << " , " <<
            i_backgroundRect.height());
  LOG_DEBUG("Size of scene rect: " << scene()->sceneRect().width() << " , " <<
        scene()->sceneRect().height());
    LOG_DEBUG("Size of scene rect: " << size().width() << " , " <<
        size().height());

  // draw lines of the grid
  drawGrid(painter, i_backgroundRect);
  painter->setBrush(Qt::red);
  painter->drawEllipse(-2, -2, 4, 4);
  painter->setBrush(Qt::NoBrush);
}


void TraceGraphView::drawGrid(QPainter *painter,
                              const QRectF &rect) {
  // draw lines
  QPointF start = rect.topLeft().toPoint();
  QPointF end = rect.bottomRight().toPoint();
  LOG_DEBUG("Draw grid!");
  LOG_DEBUG(rect.topLeft().x() << " , " << rect.topLeft().y());
  LOG_DEBUG(rect.bottomRight().x() << " , " << rect.bottomRight().y());
  LOG_DEBUG(start.x() << " , " << start.y());
  LOG_DEBUG(end.x() << " , " << end.y());

  // draw vertical grid
  for (int i = start.x();
       i < end.x();
       ++i) {
    if (i % m_gridSpacing == 0) {
      painter->drawLine(QLine(QPoint(i, rect.top()),
                              QPoint(i, rect.bottom())));
      i+= m_gridSpacing - 2;
    }
  }

  // draw horizontal grid
  for (int i = start.y();
       i < end.y();
       ++i) {
    if (i % m_gridSpacing == 0) {
      painter->drawLine(QLine(QPoint(rect.left(), i),
                              QPoint(rect.right(), i)));
      i+= m_gridSpacing - 2;
    }
  }
}


TraceGraphItem* TraceGraphView::getSelectedTraceGraphItem() {
  QList<TraceGraphItem*> traceItems;
  for (auto& item : scene()->selectedItems()) {
    if (TraceGraphItem* newTraceItem = dynamic_cast<TraceGraphItem*>(item)) {
      traceItems.append(newTraceItem);
    } else if (TraceGraphPoint* newTracePoint = dynamic_cast<TraceGraphPoint*>(item)) {
      TraceGraphItem* parent = static_cast<TraceGraphItem*>(newTracePoint->parentItem());
      if (!traceItems.contains(parent)) {
        traceItems.append(parent);
      }
    }
  }

  if (traceItems.size() == 1) {
    return traceItems[0];
  }
  return nullptr;
}


TraceGraphItem* TraceGraphView::
  findItem(const Trace::TracePointer& i_pointer) const {
  TraceGraphItem* output = nullptr;
  for (auto& qGraphicsItem : items()) {
    LOG_DEBUG("Found an item");
    // Cast the item to a trace graph item
    output = dynamic_cast<TraceGraphItem*>(qGraphicsItem);
    // Check if it worked otherwise go on
    if (!output) {
      LOG_DEBUG("Item could not be cast");
      continue;
    } else {
      LOG_DEBUG("Item could be cast, checking if it fits");
    }

    // Get the underlaying trace pointer
    Trace::TracePointer currentPointer(output->getTracePointer().lock());
    if ( currentPointer == i_pointer) {
      // return the item
      LOG_DEBUG("Found an item and returning it");
      return output;
    }
  }
  return nullptr;
}
