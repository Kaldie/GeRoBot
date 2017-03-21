//Copyright [2015] Ruud Cools
#ifndef GUI_WIDGET_TRACEWIDGET_TRACEGRAPHPOINT_H_
#define GUI_WIDGET_TRACEWIDGET_TRACEGRAPHPOINT_H_

#include <QGraphicsItem>

// foward declarations
class TraceGraphItem;
class Trace;
class RotationTrace;

class TraceGraphPoint : public QGraphicsItem {
 public:
  /// Enum defining if this object belongs to the start or the stop of the trace
  enum PointPosition {StartPoint, EndPoint, CenterPoint};
  
  /// variable define if this point is on the end or start position of the trace
  GETSET(PointPosition, m_positionOnTrace, PositionOnTrace);

 public:
  /// Constructor
  explicit TraceGraphPoint(TraceGraphItem* parent,
			   TraceGraphPoint::PointPosition i_position);

   enum { Type = UserType + 2};
   virtual int type() const Q_DECL_OVERRIDE { return Type; }


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

  /**
   * update the points so that the will or will not snap to other points
   */
  static void updateSnapToOthers(const bool&);
 protected:
  /**
   * Be notified that the point has been moved
   * Update the TracePointer, update the TraceGraphitem,
   */
  virtual QVariant itemChange(GraphicsItemChange change,
                              const QVariant &value) Q_DECL_OVERRIDE;

  virtual void mousePressEvent(QGraphicsSceneMouseEvent* i_event) Q_DECL_OVERRIDE;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* i_event) Q_DECL_OVERRIDE;

 private:
  /// No default constructor
  TraceGraphPoint(){};

  /// Update the position of the widget based on the current Trace position
  void updatePositionOnScene(const Trace::TracePointer& i_trace);

  /// Update the position of the trace point based on the current position of the point
  void updateTracePosition(Trace::TracePointer& i_trace,
                           QPointF& i_newPosition);

  /// Method which determines if given the new point do we need to correct it.
  bool curveNeedsCorrection(const std::shared_ptr<RotationTrace>&,
                            const Point2D& i_newPosition) const;

  bool snapPointToOthers(QPointF* i_newPoint) const;

  /// defines if the point is being moved, otherwise will just set them to the end position
  bool m_isBeingMoved;

  /// variable storing the postion of the trace when the mouse went down first
  QPointF m_startPointAtMouseDown;

  /// variable defining if it should snap to other points in the scene
  static bool m_snapToOthers;

  /// size of the circle
  static const int m_size;

  /// distance to search for different points to snap to
  static const float m_searchDistance;
};

#endif  // GUI_WIDGET_TRACEWIDGET_TRACEGRAPHPOINT_H_
