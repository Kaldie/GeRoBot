// Copyright [2014] Ruud Cools
#include <macroHeader.h>
#include <Point2D.h>
#include <QtGui>
#include <QPainter>
#include <Robot.h>
#include <JointController.h>
#include <BaseJoint.h>
#include <RelativeSpeedController.h>
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
  connect(toolModeRadioButton, SIGNAL(toggled(bool)),
          this, SLOT(updateMovementType(bool)));
  connect(axisModeRadioButton, SIGNAL(toggled(bool)),
          this, SLOT(updateMovementType(bool)));
  // Connections to handle movement of the robot
  connect(moveLeftButton, SIGNAL(clicked()), this, SLOT(movementLeft()));
  connect(moveLeftButton, SIGNAL(clicked()), this, SLOT(movementCounterClockWise()));

  connect(moveRightButton, SIGNAL(clicked()), this, SLOT(movementRight()));
  connect(moveRightButton, SIGNAL(clicked()), this, SLOT(movementClockWise()));

  connect(moveUpButton, SIGNAL(clicked()), this, SLOT(movementUp()));
  connect(moveUpButton, SIGNAL(clicked()), this, SLOT(movementExtent()));

  connect(moveDownButton, SIGNAL(clicked()), this, SLOT(movementRetract()));
  connect(moveDownButton, SIGNAL(clicked()), this, SLOT(movementDown()));

  robotCanvas->layout()->addWidget(new RobotPositionWidget(m_robotPointer, this));

  connect(simulateRadioButton,SIGNAL(toggled(bool)),
          this, SLOT(updateSimulateRadioButtons()));
  connect(actuateRadioButton, SIGNAL(toggled(bool)),
          this, SLOT(updateSimulateRadioButtons()));

  connect(speedSlider, SIGNAL(sliderReleased()),
          this, SLOT(setSpeedOnSpeedController()));

  updateFromNewPosition();
  updateMovementType(true);
  updateFromConfiguration();
}


void RobotMovementWidget::updateFromConfiguration() {
  LOG_DEBUG("Update from configuration");
  updateSpeedSlider();
  updateSimulateRadioButtons();
  updateTextEdit();
}


void RobotMovementWidget::updateFromNewPosition() {
  updatePositionWidget();
  updateTextEdit();
}


void RobotMovementWidget::updateSpeedSlider() {
  bool hasRelativeSpeedController =
    (m_robotPointer->getSpeedController()->getType() == SpeedController::Type::Relative);
  speedSlider->setEnabled(hasRelativeSpeedController);
  QString qstring;
  if (hasRelativeSpeedController) {
    RelativeSpeedController* controller =
      static_cast<RelativeSpeedController*>(m_robotPointer->getSpeedController().get());
    speedSlider->setToolTip(getRelativeToolTip(controller));
    speedSlider->setStatusTip(getRelativeStatusTip(controller));
    speedSlider->setValue(controller->getRobotSpeed());
  } else {
    qstring = "Speed controlling only valid with a relative speed controller!";
    speedSlider->setToolTip(qstring);
    speedSlider->setStatusTip(qstring);
  }
}


QString RobotMovementWidget::getRelativeToolTip(RelativeSpeedController* i_controller) const {
  QString string("");
  QTextStream stringStream(&string);
  stringStream << "Speed controlling at " << i_controller->getRobotSpeed() <<
    "% of the maximum!";
  return *stringStream.string();
}


QString RobotMovementWidget::getRelativeStatusTip(RelativeSpeedController* i_controller) const {
  QString string("");
  QString dimension;
  QTextStream stringStream(&string);
  int i = 1;
  stringStream << "Estimate speed at this position is: ";
  for (const auto& joint : m_robotPointer->getJointController()->getJointPointerVector()) {
    if (joint->getMovementType() == BaseJoint::MovementType::Rotational) {
      dimension = "deg/sec:";
    } else if (joint->getMovementType() == BaseJoint::MovementType::Translational) {
      dimension = "mm/sec:";
    }
    stringStream << i_controller->determineRobotSpeed(joint) << " " << dimension << " for joint " << i << ". ";
    ++i;
  }
  return *stringStream.string();
}
     


void RobotMovementWidget::setSpeedOnSpeedController() {
  m_robotPointer->getSpeedController()->setRobotSpeed(speedSlider->value());
  updateSpeedSlider();
}


void RobotMovementWidget::updatePositionWidget() {
  RobotPositionWidget* widget = robotCanvas->
    findChild<RobotPositionWidget*>();
  if (widget) {
    widget->update();
  } else {
    LOG_ERROR("Could not find item!");
  }
}


void RobotMovementWidget::updateTextEdit() {
  LOG_DEBUG("Update RobotPosition!");
  Point2D robotPosition = m_robotPointer->getPosition();
  QString textualRepresentation;
  textualRepresentation.setNum(robotPosition.x);
  xPositionLineEdit->setText(textualRepresentation);

  textualRepresentation.setNum(robotPosition.y);
  yPositionLineEdit->setText(textualRepresentation);

  textualRepresentation.setNum(magnitude(robotPosition));
  extensionLineEdit->setText(textualRepresentation);

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
}


bool RobotMovementWidget::hasValidRobot() {
  LOG_DEBUG("Updating Simulation and Actuation Radio buttons!");
  /*
    This is a slot for the hasNewRobotPointer signal.
    It will update the simulation/actuation radio buttons,
    based on the availability of the RobotPointer and
    if a connection can be established to an arduino motor driver
  */
  if (m_robotPointer) {
    LOG_DEBUG("Found proper set robot pointer!");
    try {
      m_robotPointer->hasValidConnection();
      LOG_DEBUG("Found a connection!!!");
      return true;
    } catch (std::runtime_error) {
      LOG_DEBUG("Dit not find a connection!");
    }
  } else {
    LOG_DEBUG("Dit not find a robot pointer");
  }
  return false;
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
  movementToolMode(BaseJoint::MovementType::Translational, "OUT");
}


void RobotMovementWidget::movementRetract() {
  movementToolMode(BaseJoint::MovementType::Translational, "IN");
}


void RobotMovementWidget::movementClockWise() {
  movementToolMode(BaseJoint::MovementType::Rotational, "CW");
}


void RobotMovementWidget::movementCounterClockWise() {
  movementToolMode(BaseJoint::MovementType::Rotational, "CCW");
}


void RobotMovementWidget::movementToolMode(const BaseJoint::MovementType& i_type,
                                           const std::string& i_direction) {
  if (m_mode != ToolMode)
    return;
  double stepSize = stepSizeLineEdit->text().toDouble();
  // convert to radians
  if (i_type == BaseJoint::MovementType::Rotational) {
    stepSize *= (PI/180.0);
  }
  double robotStepSize = m_robotPointer->getMovementPerStep(i_type);
  int numberOfSteps = (stepSize) / robotStepSize;
  if (numberOfSteps == 0) {
    numberOfSteps = 1;
  }
  // Calculate the new position and prepare to actual take the steps
  m_robotPointer->getSpeedController()->prepareSpeedController
      (m_robotPointer->getJointController()->resolveJoint(i_type));
  m_robotPointer->prepareSteps(i_direction, numberOfSteps);
  if (hasValidRobot() && actuateRadioButton->isChecked()) {
    m_robotPointer->actuate();
  } else {
    m_robotPointer->setPosition(m_robotPointer->getVirtualPosition());
  }
  emit hasNewPosition();
}
