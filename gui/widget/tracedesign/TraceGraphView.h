#ifndef GUI_WIDGET_TRACEWIDGET_TRACEGRAPHSCENE_H_
#define GUI_WIDGET_TRACEWIDGET_TRACEGRAPHSCENE_H_

#include <macroHeader.h>
#include <QGraphicsView>
#include <QList>
#include <Trace.h>

class TraceGraphItem;
class QGraphicsScene;
class QRectF;

/**
 * A view to visualize and edit Traces.
 * Added several functions to add/ remove/ replace current TraceGraphitems.
 * The changed(const QList<QRectF> & region) signal of the scene should be connected
 * to make other bits of the interface respond to changes in this view.
 */
class TraceGraphView : public QGraphicsView {
  Q_OBJECT

 public:
  /**
   * Default constructor
   * Creates the view and scene which will be able to add items
   * The class is responsible for sending signals if a trace is changed
   * The signal will hold a single Trace::TracePointer or a QList of pointers
   */
  TraceGraphView(QWidget *parent = 0);

  /**
   * Add a new TraceGraphItem to the scene
   * A new TraceGraphItem will be created from the Pointer
   * This item will be added to the scene will be set as selected
   * This function signals traceChanged(Trace::TracePointer)
   */
  TraceGraphItem* addTraceItem(const Trace::TracePointer& i_trace);

  /**
   * Removes a TraceGraphItem from the scene.
   * Based on the given Trace::TracePointer remove
   * the asociated TraceGraphItem
   */
  bool removeTraceItem(const Trace::TracePointer& i_trace);

  /**
   * Replace the current trace item with a new one defined by the trace
   * The current selected trace item is removed from the scene
   * A newly created trace item is added and set as selected
   * @param[in] i_newTrace a shared_ptr to added trace
   * if there are 0 or more then 1 items selected this will not work
   * false is returned
   */
  bool replaceCurrentTrace(const Trace::TracePointer& i_newTrace);

  /**
   * Set the corrisponding TraceGraphitem as selected and update the scene
   */
  void setSelected(const Trace::TracePointer& i_trace);


  /**
   * Get the selected TraceItem if there is one, otherwise return nullptr
   * Simple function to make my life easier, life easy is important
   */
  TraceGraphItem* getSelectedTraceGraphItem();

 public slots:
  void updateSelectedItem();

 protected:
  virtual void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

 private:
  /**
   * Draws the a legend based on the size of the scene rect
   * Based on the size of the exposed scene a legend indicating the real world size
   * needs to be plotted to give the user an indication on the size of it all.
   */
  //  void drawLegend(const QRectF& i_legendSize,
  //		  const double& i_gridSize){};

  void initialise();

  /**
   * Given a pointer find the corrisponding item
   * This function will search all the items in the scene to find a TraceGraphItem
   * which shares the managed object indicated by i_pointer
   * @param[in] i_pointer shared pointer that indicates the wanted item
   * @param[out] TraceGraphitem which displays the same managed object as the shaped pointer.
   * Outputs a nullptr if none can be found
   */
  TraceGraphItem* findItem(const Trace::TracePointer& i_pointer) const;

  /**
   * Draws a grid to the view
   */
  void drawGrid(QPainter *painter,
                const QRectF &rect);
  /**
   * Size of the legend relative to the whole view
   */
  int m_gridSpacing;
  static const double LegendRatio;
  static const int OptimalNumberOfLines;
};

#endif // GUI_WIDGET_TRACEWIDGET_TRACEGRAPHSCENE_H_
