//Copyright [2015] Ruud Cools
#ifndef GUI_WIDGET_TRACEWIDGET_TRACEGRAPHPOINT_H_
#define GUI_WIDGET_TRACEWIDGET_TRACEGRAPHPOINT_H_

#include <QGraphicsItem>

// foward declarations
class TraceGraphItem;
class Trace;


class TraceGraphPoint : public QGraphicsItem {
 public:
  /// Enum defining if this object belongs to the start or the stop of the trace
  enum PointPosition {StartPoint, EndPoint, CenterPoint};

  /// Constructor
  explicit TraceGraphPoint(TraceGraphItem* parent,
			   TraceGraphPoint::PointPosition i_position);

  /// Paint the item to the scene
  virtual void paint(QPainter *painter,
		     const QStyleOptionGraphicsItem *option,
		     QWidget *widget) Q_DECL_OVERRIDE;

  /// Define the bounding box
  virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

  /**
   * Due to changes in the underlaying trace, update the position
   * on the scene and notify peeps if needed
   */
  void updatePositionOnScene();

  /**
   * Sometimes an invalid position of one of the points is set,
   * this function corrects it and returns if it has been corrected
   */
  bool correctTracePosition(Trace::TracePointer,
                            Point2D* i_possibleNewPoint);
 protected:
  /**
   * Be notified that the point has been moved
   * Update the TracePointer, update the TraceGraphitem,
   */
  virtual QVariant itemChange(GraphicsItemChange change,
                              const QVariant &value) Q_DECL_OVERRIDE;

  virtual void mousePressEvent(QGraphicsSceneMouseEvent* i_event);
 private:
  /// No default constructor
  TraceGraphPoint(){};

  /// Update the position of the widget based on the current Trace position
  void updatePositionOnScene(const Trace::TracePointer& i_trace);

  /// Update the position of the trace point based on the current position of the point
  void updateTracePosition(Trace::TracePointer& i_trace,
                           QPointF& i_newPosition);
  /// Method which determines if given the new point do we need to correct it.
  bool curveNeedsCorrection(const RotationTrace::RotationTracePointer&,
                            const Point2D& i_newPosition) const;
  /// variable define if this point is on the end or start position of the trace
  PointPosition m_positionOnTrace;

  /// variable storing the postion of the trace when the mouse went down first
  QPointF m_startPointAtMouseDown;
  /// size of the circle
  static const int size;
};

#endif  // GUI_WIDGET_TRACEWIDGET_TRACEGRAPHPOINT_H_
