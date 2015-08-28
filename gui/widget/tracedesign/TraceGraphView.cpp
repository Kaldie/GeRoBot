//Copyright [2015] Ruud Cools

#include <macroHeader.h>
#include <Trace.h>
#include <RotationTrace.h>
#include <QPen>
#include "./TraceGraphView.h"
#include "./RotationTraceGraphItem.h"

const int TraceGraphView::MinimalSpaceBetweenGrid = 20;
const double TraceGraphView::LegendRatio = 0.2;
const int TraceGraphView::OptimalNumberOfLines = 8;

TraceGraphView::TraceGraphView(QWidget* i_parent)
  : QGraphicsView(i_parent) {
  initialise();
}


void TraceGraphView::initialise() {
  QGraphicsScene* scene = new QGraphicsScene;
  scene->setSceneRect(-100,-100,200,200);
  scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
  setScene(scene);
  setRenderHint(QPainter::Antialiasing);
  setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  setDragMode(QGraphicsView::RubberBandDrag);
  setCacheMode(QGraphicsView::CacheBackground);

}


void TraceGraphView::addTraceItem(const Trace::TracePointer& i_trace) {
  if (!i_trace) {
    setSelected(nullptr);
    return;
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
  scene()->addItem(newGraphItem);
}


bool TraceGraphView::removeTraceItem(const Trace::TracePointer& i_trace) {
  bool hasRemoved(false);
  TraceGraphItem* item(nullptr);
  findItem(i_trace,
	   item);
  if (item) {
    // remove item from the scene and receive ownership
    scene()->removeItem(item);
    // delete from heap
    delete item;
    // indicate that we have deleted an item
    hasRemoved = true;
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
}


void TraceGraphView::setSelected(Trace::TracePointer i_trace) {
  TraceGraphItem* item(nullptr);
  findItem(i_trace,
	   item);
  // see if the item is actually found
  if (!item) {
    return;
  }
  item->setSelected(true);
}


void TraceGraphView::drawBackground(QPainter *painter, const QRectF &rect) {
  // set the painter to make nice thingies
  QPen pen(Qt::DashLine);
  QColor color(Qt::black);
  color.setAlpha(100);
  pen.setColor(color);
  pen.setCapStyle(Qt::RoundCap);
  painter->setPen(pen);

  // draw lines of the grid
  int numberOfLines = drawVerticalGrid(painter, rect);
  drawHorizontalGrid(painter, rect);
  // get the scalling of the view
  QPointF ScalePoint(transform().map(QPointF(1,1)));
  // get the "real" size of the grid
  double gridSize = rect.width() / numberOfLines * ScalePoint.x();

  //  QRectF legendSize = QRectF(rect
  QPoint bottomLeft = rect.toRect().bottomLeft();
  int legendHeight = static_cast<int>(rect.height() * TraceGraphView::LegendRatio);
  int legendWidth = static_cast<int>(rect.width() * 1.25 * TraceGraphView::LegendRatio);
  bottomLeft.ry() -= legendHeight;
  bottomLeft.rx() -= legendWidth;

  QRect legendRect = QRect(bottomLeft,
                           QSize(legendWidth,
                                 legendHeight));
  // draw a legend to the view
  drawLegend(legendRect,
	     gridSize);

}


int TraceGraphView::drawVerticalGrid(QPainter *painter,
                                     const QRectF &rect) {
    // set the size of the axis that will be drawn
  int sizeOfRect(rect.width());
  int numberOfLines = TraceGraphView::OptimalNumberOfLines;
  int spacing(0);
  // set the spacing between the lines
  for (;
       numberOfLines > 0;
       --numberOfLines) {
    spacing = rect.width() / numberOfLines;
    if (spacing > TraceGraphView::MinimalSpaceBetweenGrid)
      break;
  }

  // draw lines
  for (int i = rect.left();
       i < rect.width();
       i += spacing) {
    painter->drawLine(QLine(QPoint(i,rect.top()),
                            QPoint(i,rect.height())));
  }
  return numberOfLines;
}


int TraceGraphView::drawHorizontalGrid(QPainter *painter,
                                       const QRectF &rect) {
    // set the size of the axis that will be drawn
  int sizeOfRect(rect.height());
  int numberOfLines = TraceGraphView::OptimalNumberOfLines;
  int spacing(0);
  // set the spacing between the lines
  for (;
       numberOfLines > 0;
       --numberOfLines) {
    spacing = rect.height() / numberOfLines;
    if (spacing > TraceGraphView::MinimalSpaceBetweenGrid)
      break;
  }

  // draw lines
  for (int i = rect.top();
       i < rect.height();
       i += spacing) {
    painter->drawLine(QLine(QPoint(rect.left(), i),
                            QPoint(rect.width(), i)));
  }
  return numberOfLines;
}


void TraceGraphView::findItem(const Trace::TracePointer& i_pointer,
			      TraceGraphItem* i_output) const {
  i_output = nullptr;
  for (auto& qGraphicsItem : items()) {
    // Cast the item to a trace graph item
    TraceGraphItem* item = dynamic_cast<TraceGraphItem*>(qGraphicsItem);
    // Check if it worked otherwise go on
    if (!item) {
      continue;
    }
    // Get the underlaying trace pointer
    Trace::TracePointer currentPointer(item->getTracePointer().lock());
    if ( currentPointer == i_pointer) {
      // return the item
      i_output = item;
      return;
    }
  }
}
