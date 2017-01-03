// Copyright [2016] Ruud Cools
#include <macroHeader.h>
#include <Point2D.h>
#include <QPainter>
#include <QPointF>
#include <QtWidgets>
#include <QTimer>
#include <Robot.h>
#include "./RobotLocator.h"

const int RobotLocator::m_size = 5;

RobotLocator::RobotLocator(const Robot::RobotPointer& i_robot,
			   QGraphicsItem* i_parent  /*=0*/)
  :QGraphicsItem(i_parent), m_robot(i_robot) {
  setCacheMode(QGraphicsItem::DeviceCoordinateCache);
  LOG_DEBUG("Constructing a RobotLocator");
}


void RobotLocator::paint(QPainter *painter,
			 const QStyleOptionGraphicsItem *option,
			 QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  LOG_DEBUG("Paint an Point");
  QPen pen = painter->pen();
  pen.setCosmetic(true);
  pen.setWidth(1);
  pen.setColor(QColor(Qt::blue));
  painter->setPen(pen);
  QBrush brush = painter->brush();
  brush.setStyle(Qt::SolidPattern);
  painter->setBrush(brush);
  painter->drawEllipse(-RobotLocator::m_size * 0.5,
		       -RobotLocator::m_size * 0.5,
		       RobotLocator::m_size,
		       RobotLocator::m_size);
}


QRectF RobotLocator::boundingRect() const {
  LOG_DEBUG("Create bouding box!");
  QRectF rect(-RobotLocator::m_size * 0.5,
	      -RobotLocator::m_size * 0.5,
	      RobotLocator::m_size,
	      RobotLocator::m_size);
  rect.adjust(-2,-2,2,2);
  return rect;
}


void RobotLocator::updatePosition() {
  Point2D x = m_robot->getVirtualPosition();
  LOG_DEBUG("Position is: " << x.x << " , " << x.y);
  setPos(x);
}
