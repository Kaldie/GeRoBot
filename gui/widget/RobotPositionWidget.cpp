// Copyright [2014] <Ruud Cools>

#include <QtGui>
#include <macroHeader.h>
#include "./RobotPositionWidget.h"

RobotPositionWidget::RobotPositionWidget(QWidget *parent)
    : QWidget(parent)
{}


RobotPositionWidget::RobotPositionWidget(Point2D* i_point,
                                         QWidget* parent /*= 0 */):
    QWidget(parent),
    m_point(i_point)
{}


QSize RobotPositionWidget::minimumSizeHint() const {
  return QSize(100, 100);
}

QSize RobotPositionWidget::sizeHint() const {
  return QSize(300, 300);
}

void RobotPositionWidget::paintEvent(QPaintEvent * /* event */) {
  if (!m_point) {
    LOG_DEBUG("Point2D pointer is void");
    return;
  }

  QPoint points[2] = {QPoint(0, 0), QPoint(m_point->x, -1 * m_point->y)};
  float scaleFactor = qMin(width(), height())/100.0;

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(width() / 2, height()-(5*scaleFactor));
  painter.scale(scaleFactor, scaleFactor);

  // Draw position of the robot base
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor("Red"));
  painter.drawEllipse(points[0], 1, 1);

  // Draw position of the robot head
  painter.setBrush(QColor("Green"));
  painter.drawEllipse(points[1], 1, 1);

  // Draw the stuf between it
  QPen pen;
  pen.setWidthF(0.5);
  pen.setColor(QColor("Black"));
  painter.setPen(pen);
  painter.drawLine(points[0], points[1]);

  // Definition of the start and end point of the axis
  QPoint startPoint(width() / (scaleFactor * -2), 0);
  QPoint endPoint(width() / (scaleFactor * 2), 0);

  // draw axis
  painter.drawLine(startPoint, endPoint);

  QFont font;
  font.setPixelSize(3);
  painter.setFont(font);
  
  for (int xPosition = startPoint.x()-(startPoint.x()%10);
       xPosition < endPoint.x();
       xPosition += 10) {
    painter.drawLine(QPoint(xPosition, 0),
                     QPoint(xPosition, -3));

    if (std::abs(xPosition) >= 10)
      painter.drawText(QPoint(xPosition-2, 3),
                       QVariant(xPosition).toString());
    else
      painter.drawText(QPoint(xPosition-1, 3),
                       QVariant(xPosition).toString());
  }
}
