// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <Point2D.h>
#include <QtGui>
#include <QPainter>
#include <Robot.h>
#include "./RobotPositionWidget.h"
#include "./RobotMovementWidget.h"


RobotMovementWidget::RobotMovementWidget(Robot::RobotPointer i_robotPointer,
                                         QWidget* parent/*=0*/)
  : QWidget(parent),
    m_robotPointer(i_robotPointer) {
  initialise();
}


RobotMovementWidget::RobotMovementWidget(QWidget* parent /* =0 */)
    : QWidget(parent) {
  initialise();
}


void RobotMovementWidget::setRobotPointer(Robot::RobotPointer i_robotPointer) {
  if (i_robotPointer)
    return;
  m_robotPointer = i_robotPointer;

}


void RobotMovementWidget::initialise() {
  setupUi(this);

  /* custom signal to indicate the position is change
     is linked to update position widget
  */
  connect(this, SIGNAL(hasNewPosition()), this, SLOT(updatePositionWidget()));
  connect(toolModeRadioButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(updateMovementType(bool)));
  connect(axisModeRadioButton,
          SIGNAL(toggled(bool)),
          this,
          SLOT(updateMovementType(bool)));
  connect(moveLeftButton, SIGNAL(clicked()), this, SLOT(movementLeft()));
  connect(moveLeftButton, SIGNAL(clicked()), this, SLOT(movementCounterClockWise()));
  connect(moveRightButton, SIGNAL(clicked()), this, SLOT(movementRight()));
  connect(moveRightButton, SIGNAL(clicked()), this, SLOT(movementClockWise()));
  connect(moveUpButton, SIGNAL(clicked()), this, SLOT(movementUp()));
  connect(moveUpButton, SIGNAL(clicked()), this, SLOT(movementExtent()));
  connect(moveDownButton, SIGNAL(clicked()), this, SLOT(movementRetract()));

  robotCanvas->layout()->addWidget(new RobotPositionWidget(m_robotPointer, this));
  if (m_robotPointer) {
    updateMovementType(true);
    connect(simulateRadioButton,
            SIGNAL(toggled(bool)),
            this,
            SLOT(updateSimulateRadioButtons()));
    connect(actuateRadioButton,
            SIGNAL(toggled(bool)),
            this,
            SLOT(updateSimulateRadioButtons()));
    updateSimulateRadioButtons();
  } else {
    LOG_DEBUG("m_RobotPointer is not a valid pointer!");
  }
}


void RobotMovementWidget::updatePositionWidget() {
  QString textualRepresentation;

  // Update grafical widget
  QWidget* robotPositionWidget = robotCanvas->
      findChild<RobotPositionWidget*>();

  if (robotPositionWidget)
    robotPositionWidget->update();
  else
    LOG_ERROR("could not find robot position widget!");
  LOG_DEBUG("Update RobotPosition!");
  Point2D robotPosition = m_robotPointer->getPosition();
  // update x position line edit
  textualRepresentation.setNum(robotPosition.x);
  xPositionLineEdit->setText(textualRepresentation);

  // update y position line edit
  textualRepresentation.setNum(robotPosition.y);
  yPositionLineEdit->setText(textualRepresentation);

  // update extension line edit
  textualRepresentation.setNum(Magnitude(robotPosition));
  extensionLineEdit->setText(textualRepresentation);

  // update angle line edit
  textualRepresentation.setNum(robotPosition.getAlpha() * 180 / PI);
  angleLineEdit->setText(textualRepresentation);
}


void RobotMovementWidget::updateMovementType(bool i_isChecked) {
  //  update line edits
  if (!m_robotPointer && !i_isChecked)
    return;

  // store movement mode in dialog
  if (toolModeRadioButton->isChecked())
    m_mode = ToolMode;
  else if (axisModeRadioButton->isChecked())
    m_mode = AxisMode;
  else
    LOG_ERROR("Mode is not set correctly!");

  if (m_mode == ToolMode) {
    // set icons of movement button to tool mode which is straight pointers
    moveLeftButton->setIcon(QIcon(":/icons/CCW.png"));
    moveRightButton->setIcon(QIcon(":/icons/CW.png"));
  } else if (m_mode == AxisMode) {
    moveLeftButton->setIcon(QIcon(":/icons/StraightLeft.png"));
    moveRightButton->setIcon(QIcon(":/icons/StraightRight.png"));
  } else {
    LOG_ERROR("Mode is not resolved correctly!");
  }

  moveUpButton->setIcon(QIcon(":/icons/StraightUp.png"));
  moveDownButton->setIcon(QIcon(":/icons/StraightDown.png"));

  RobotPositionWidget* widget = robotCanvas->
      findChild<RobotPositionWidget*>();
  if (widget)
    widget->update();
  else
    LOG_ERROR("Could not find item!");
}


bool RobotMovementWidget::hasValidRobot() {
  LOG_DEBUG("Updating Simulation and Actuation Radio buttons!");
  /*
    This is a slot for the hasNewRobotPointer signal.
    It will update the simulation/actuation radio buttons,
    based on the availability of the RobotPointer and
    if a connection can be established to an arduino motor driver    
  */

  bool hasRobotPointer(false);
  bool hasArduinoConnection(false);

  if (m_robotPointer) {
    hasRobotPointer = true;
    LOG_DEBUG("Found robot pointer!");
  }

  if (hasRobotPointer) {
    try {
      if (m_robotPointer->hasValidConnection()) {
        LOG_DEBUG("Found a connection!!!");
        hasArduinoConnection = true;
      } else {
        LOG_DEBUG("Has a robot pointer but no connection!!!");
        hasArduinoConnection = false;
      }
    } catch(std::runtime_error) {
      LOG_DEBUG("Did not find a connection!!!");
      hasArduinoConnection = false;
    }
  }
  return hasRobotPointer && hasArduinoConnection;
}

void RobotMovementWidget::updateSimulateRadioButtons() {
  if (hasValidRobot()) {
    actuateRadioButton->setEnabled(true);
  } else {
    actuateRadioButton->setEnabled(false);
    actuateRadioButton->setChecked(false);
    simulateRadioButton->setChecked(true);
  }
}


void RobotMovementWidget::movementUp() {
  if (m_mode != AxisMode)
    return;

  Point2D newPosition = m_robotPointer->getPosition();
  newPosition.y += stepSizeLineEdit->text().toDouble();
  if (hasValidRobot() && actuateRadioButton->isChecked()) {
    m_robotPointer->goToPosition(newPosition);
  } else {
    m_robotPointer->setPosition(newPosition);
  }
  emit hasNewPosition();
}


void RobotMovementWidget::movementDown() {
  if (m_mode != AxisMode)
    return;

  Point2D newPosition = m_robotPointer->getPosition();
  newPosition.y -= stepSizeLineEdit->text().toDouble();
  if (hasValidRobot() && actuateRadioButton->isChecked()) {
    m_robotPointer->goToPosition(newPosition);
  } else {
    m_robotPointer->setPosition(newPosition);
  }
  emit hasNewPosition();
}


void RobotMovementWidget::movementLeft() {
  if (m_mode != AxisMode)
    return;
  Point2D newPosition = m_robotPointer->getPosition();
  newPosition.x -= stepSizeLineEdit->text().toDouble();
  if (hasValidRobot() && actuateRadioButton->isChecked()) {
    m_robotPointer->goToPosition(newPosition);
  } else {
    m_robotPointer->setPosition(newPosition);
  }
  emit hasNewPosition();
}

void RobotMovementWidget::movementRight() {
  if (m_mode != AxisMode)
    return;

  Point2D newPosition = m_robotPointer->getPosition();
  newPosition.x += stepSizeLineEdit->text().toDouble();
  if (hasValidRobot() && actuateRadioButton->isChecked()) {
    m_robotPointer->goToPosition(newPosition);
  } else {
    m_robotPointer->setPosition(newPosition);
  }
  emit hasNewPosition();
}


void RobotMovementWidget::movementExtent() {
  if (m_mode != ToolMode)
    return;
  double stepSize = stepSizeLineEdit->text().toDouble();
  double robotStepSize = m_robotPointer->getMovementPerStep(Translational);
  int numberOfSteps = stepSize / robotStepSize;
  if (numberOfSteps == 0) {
    numberOfSteps = 1;
  }
  // Calculate the new position and prepare to actual take the steps
  m_robotPointer->prepareSteps("OUT",numberOfSteps);
  if (hasValidRobot() && actuateRadioButton->isChecked()) {
    m_robotPointer->actuate();
  } else {
    m_robotPointer->setPosition(m_robotPointer->getVirtualPosition());
  }
  emit hasNewPosition();
}


void RobotMovementWidget::movementRetract() {
  if (m_mode != ToolMode)
    return;
  double stepSize = stepSizeLineEdit->text().toDouble();
  double robotStepSize = m_robotPointer->getMovementPerStep(Translational);
  int numberOfSteps = stepSize / robotStepSize;
  if (numberOfSteps == 0) {
    numberOfSteps = 1;
  }
  // Calculate the new position and prepare to actual take the steps
  m_robotPointer->prepareSteps("IN",numberOfSteps);
  if (hasValidRobot() && actuateRadioButton->isChecked()) {
    m_robotPointer->actuate();
  } else {
    m_robotPointer->setPosition(m_robotPointer->getVirtualPosition());
  }
  emit hasNewPosition();
}


void RobotMovementWidget::movementClockWise() {
  if (m_mode != ToolMode)
    return;
  double stepSize = stepSizeLineEdit->text().toDouble();
  double robotStepSize = m_robotPointer->getMovementPerStep(Rotational);
  int numberOfSteps = stepSize / robotStepSize;
  if (numberOfSteps == 0) {
    numberOfSteps = 1;
  }
  // Calculate the new position and prepare to actual take the steps
  m_robotPointer->prepareSteps("CW",numberOfSteps);
  if (hasValidRobot() && actuateRadioButton->isChecked()) {
    m_robotPointer->actuate();
  } else {
    m_robotPointer->setPosition(m_robotPointer->getVirtualPosition());
  }
  emit hasNewPosition();
}


void RobotMovementWidget::movementCounterClockWise() {
  if (m_mode != ToolMode)
    return;
  double stepSize = stepSizeLineEdit->text().toDouble();
  double robotStepSize = m_robotPointer->getMovementPerStep(Rotational);
  int numberOfSteps = stepSize / robotStepSize;
  if (numberOfSteps == 0) {
    numberOfSteps = 1;
  }
  // Calculate the new position and prepare to actual take the steps
  m_robotPointer->prepareSteps("CCW",numberOfSteps);
  if (hasValidRobot() && actuateRadioButton->isChecked()) {
    m_robotPointer->actuate();
  } else {
    m_robotPointer->setPosition(m_robotPointer->getVirtualPosition());
  }
  emit hasNewPosition();
}
