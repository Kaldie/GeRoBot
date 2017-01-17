// Copyright Ruud Cools [2017]
#include "./ManualMovementWidget.h"
#include "./RobotMovementWidget.h"
#include "./RobotPositionWidget.h"


ManualMovementWidget::ManualMovementWidget(const std::shared_ptr<Robot>& i_robot,
             QWidget* i_parent)
  : QWidget(i_parent),
    m_robotPointer(i_robot) {
  initialise();
}



void ManualMovementWidget::initialise() {
  setupUi(this);
  RobotMovementWidget* robotMovementWidget = new RobotMovementWidget(m_robotPointer, this);
  RobotPositionWidget* robotPositionWidget = new RobotPositionWidget(m_robotPointer, this);
  layout()->addWidget(robotMovementWidget);
  layout()->addWidget(robotPositionWidget);

  connect(robotMovementWidget, SIGNAL(hasNewPosition()),
    robotPositionWidget, SLOT(update()));
}

