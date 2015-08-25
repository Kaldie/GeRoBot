// Copyright [2014] Ruud Cools

#include <macroHeader.h>
#include <QString>
#include "./Point2DWidget.h"

Point2DWidget::Point2DWidget(Point2D* i_point2D,
                             QWidget* parent/*=0*/)
        : QWidget(parent), m_pointPointer(i_point2D) {
  initialise();
}


Point2DWidget::Point2DWidget(QWidget* parent /* =0 */)
    : QWidget(parent) {
  m_pointPointer = NULL;
  initialise();
}


void Point2DWidget::initialise() {
  setupUi(this);

  /* custom signal to indicate the position is change
     is linked to update position widget
  */
  connect(this, SIGNAL(hasNewPosition()), this, SLOT(update()));
  connect(yPositionLineEdit,SIGNAL(editingFinished()), this , SLOT(updatePoint()));
  connect(xPositionLineEdit,SIGNAL(editingFinished()), this , SLOT(updatePoint()));
  update();
}


void Point2DWidget::update() {
  LOG_DEBUG("Update point view.");
  if (!m_pointPointer) {
    LOG_DEBUG("Pointer is invalid.");
    xPositionLineEdit->setEnabled(false);
    yPositionLineEdit->setEnabled(false);
    return;
  }
  xPositionLineEdit->setEnabled(true);
  yPositionLineEdit->setEnabled(true);

  QString textualRepresentation;
  textualRepresentation.setNum(m_pointPointer->x, 'g', Point2DWidget::m_precision);
  xPositionLineEdit->setText(textualRepresentation);
  textualRepresentation.setNum(m_pointPointer->y, 'g', Point2DWidget::m_precision);
  yPositionLineEdit->setText(textualRepresentation);
  QWidget::update();
}


void Point2DWidget::updatePoint() {
  if (!m_pointPointer)
    return;

  double x,y;
  bool hasWorked(false);
  x = xPositionLineEdit->text().toDouble(&hasWorked);
  if (hasWorked) {
    m_pointPointer->x = x;
  } else {
    QString textualRepresentation;
    textualRepresentation.setNum(m_pointPointer->x);
    xPositionLineEdit->setText(textualRepresentation);
  }

  y = yPositionLineEdit->text().toDouble(&hasWorked);
  if (hasWorked) {
    m_pointPointer->y = y;
  } else {
    QString textualRepresentation;
    textualRepresentation.setNum(m_pointPointer->y);
    yPositionLineEdit->setText(textualRepresentation);
  }
  emit hasNewPosition();
}


void Point2DWidget::setPointName(const QString& i_string) {
  pointName->setText(i_string);
}
