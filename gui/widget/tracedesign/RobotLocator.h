//Copyright [2015] Ruud Cools
#ifndef GUI_WIDGET_TRACEWIDGET_ROBOTLOCATOR_H_
#define GUI_WIDGET_TRACEWIDGET_ROBOTLOCATOR_H_

#include <QGraphicsItem>

// foward declarations
class TraceGraphView;
class Robot;

class RobotLocator : public QGraphicsItem {
  GETSET(std::shared_ptr<Robot>, m_robot, Robot);
  static const int m_size;
 public:
  /// Constructor
  explicit RobotLocator(const std::shared_ptr<Robot>& i_Robot,
			QGraphicsItem* parent = 0);
  
  /// Paint the item to the scene
  virtual void paint(QPainter *painter,
		     const QStyleOptionGraphicsItem *option,
		     QWidget *widget) Q_DECL_OVERRIDE;

  /// Define the bounding box
  virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

  void updatePosition();
    
 private:
  /// No default constructor
  RobotLocator();
};

#endif  // GUI_WIDGET_TRACEWIDGET_ROBOTLOCATOR_H_
