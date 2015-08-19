// Copyright [2014] Ruud Cools

#include <macroHeader.h>
#include <Point2D.h>
#include <QtGui>
#include <QPainter>
#include <Robot.h>
#include "./RobotPositionWidget.h"
#include "./RobotMovementWidget.h"


RobotMovementWidget::RobotMovementWidget(RobotPointer i_robotPointer,
                                         QWidget* parent/*=0*/)
    : QWidget(parent) {
  m_robotPointer = i_robotPointer;
  m_point2DPointer= m_robotPointer->getPositionPointer();
  initialise();
}


RobotMovementWidget::RobotMovementWidget(QWidget* parent /* =0 */)
    : QWidget(parent) {
  m_point2DPointer = new Point2D(0, 50);
  initialise();
}


RobotMovementWidget::~RobotMovementWidget() {
  if (!m_robotPointer)
    delete m_point2DPointer;
}


void RobotMovementWidget::setRobotPointer(RobotPointer i_robotPointer) {
  if (i_robotPointer.get() == 0)
    return;

  delete m_point2DPointer;

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
  connect(moveRightButton, SIGNAL(clicked()), this, SLOT(movementRight()));
  connect(moveUpButton, SIGNAL(clicked()), this, SLOT(movementUp()));
  connect(moveDownButton, SIGNAL(clicked()), this, SLOT(movementDown()));

  if (m_point2DPointer) {
    // adding Robot Position widget
    robotCanvas->layout()->
        addWidget(new RobotPositionWidget(m_point2DPointer, this));
    updateMovementType(true);
  } else {
    LOG_DEBUG("m_point2DPointer is not a valid pointer!");
  }
  
  if (m_robotPointer.get()) {
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

  // update x position line edit
  textualRepresentation.setNum(m_point2DPointer->x);
  xPositionLineEdit->setText(textualRepresentation);

  // update y position line edit
  textualRepresentation.setNum(m_point2DPointer->y);
  yPositionLineEdit->setText(textualRepresentation);

  // update extension line edit
  textualRepresentation.setNum(Magnitude(*m_point2DPointer));
  extensionLineEdit->setText(textualRepresentation);

  // update angle line edit
  textualRepresentation.setNum(m_point2DPointer->getAlpha() * 180 / PI);
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
    moveLeftButton->setIcon(QIcon(":/icons/StraightLeft.png"));
    moveRightButton->setIcon(QIcon(":/icons/StraightRight.png"));

  } else if (m_mode == AxisMode) {
    moveLeftButton->setIcon(QIcon(":/icons/CCW.png"));
    moveRightButton->setIcon(QIcon(":/icons/CW.png"));

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

  if (m_robotPointer.get()) {
    hasRobotPointer = true;
    LOG_DEBUG("Found robot pointer!");
  }

  if (hasRobotPointer) {
    try {
      if (m_robotPointer->hasValidConnection()) {
        LOG_DEBUG("Found a connection!!!");
        hasArduinoConnection = true;
      } else {
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
  if (hasValidRobot() && actuateRadioButton->isChecked()) {
    LOG_DEBUG("Current position x,y "
              << m_point2DPointer->x  <<", "<< m_point2DPointer->y);

    Point2D newPosition = *m_point2DPointer;
    newPosition.y += stepSizeLineEdit->text().toDouble();

    LOG_DEBUG("New position x,y "
              << newPosition.x  <<", "<< newPosition.y);

    m_robotPointer->goToPosition(newPosition);
  } else {
    m_point2DPointer->y += stepSizeLineEdit->text().toDouble();
  }
    emit hasNewPosition();
}


void RobotMovementWidget::movementDown() {
  if (hasValidRobot()) {
    Point2D newPosition = *m_point2DPointer;
    newPosition.y -= stepSizeLineEdit->text().toDouble();

    LOG_DEBUG("New position x,y "
              << newPosition.x  <<", "<< newPosition.y);

    m_robotPointer->goToPosition(newPosition);
  } else {
    m_point2DPointer->y -= stepSizeLineEdit->text().toDouble();
  }
  emit hasNewPosition();
}


void RobotMovementWidget::movementLeft() {
  if (hasValidRobot()) {
    Point2D newPosition = *m_point2DPointer;
    newPosition.x -= stepSizeLineEdit->text().toDouble();

    LOG_DEBUG("New position x,y "
              << newPosition.x  <<", "<< newPosition.y);

    m_robotPointer->goToPosition(newPosition);
  } else {
    m_point2DPointer->x -= stepSizeLineEdit->text().toDouble();
  }
  emit hasNewPosition();
}

void RobotMovementWidget::movementRight() {
  if (hasValidRobot()) {
        Point2D newPosition = *m_point2DPointer;
    newPosition.x += stepSizeLineEdit->text().toDouble();

    LOG_DEBUG("New position x,y "
              << newPosition.x  <<", "<< newPosition.y);

    m_robotPointer->goToPosition(newPosition);
  } else {
    m_point2DPointer->x += stepSizeLineEdit->text().toDouble();
  }
  emit hasNewPosition();
}
