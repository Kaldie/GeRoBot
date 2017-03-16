// Copyright Ruud Cools [2017]

#include <macroHeader.h>
#include <thread>
#include <QLineEdit>
#include <QFormLayout>
#include <QTimer>
#include <BaseCalibration.h>
#include <MotorSpeedCalibration.h>
#include "./MotorSpeedCalibrationWidget.h"


MotorSpeedCalibrationWidget::MotorSpeedCalibrationWidget
(const std::shared_ptr<BaseCalibration>& i_calibration,
 QWidget* i_parent /*= 0*/)
  : CalibrationWidget(i_calibration, i_parent),
    m_isReadyInterval(1000),
    m_isCalibrating(false),
    m_searchRangeEdit(nullptr),
    m_delayEdit(nullptr),
    m_stepSizeEdit(nullptr),
    m_timer(new QTimer()) {
  // We want to grap ALL the keyboard stuff, dont care about focus....much
  initialise();
}


MotorSpeedCalibrationWidget::~MotorSpeedCalibrationWidget() {
  // release the grap keyboard "mutex", dont know how they implemented it...
  releaseKeyboard();
  m_timer->stop();
  delete m_timer;
}


void MotorSpeedCalibrationWidget::initialise() {
  createAdditionalUIElements();

  // Connect the timer so it updates the ready button
  connect(m_timer, &QTimer::timeout,
	  this, &MotorSpeedCalibrationWidget::updateIsReady);
  
  // connect the line edits to the slot that updates the calibration
  connect(m_searchRangeEdit, &QLineEdit::editingFinished,
	  this, &MotorSpeedCalibrationWidget::updateCalibrationFromEdits);
  connect(m_delayEdit, &QLineEdit::editingFinished,
	  this, &MotorSpeedCalibrationWidget::updateCalibrationFromEdits);
  connect(m_stepSizeEdit, &QLineEdit::editingFinished,
	  this, &MotorSpeedCalibrationWidget::updateCalibrationFromEdits);


  // connect the two methods to finish the calibration ->
  // via calibration itself, and user push button
  connect(this, &MotorSpeedCalibrationWidget::userFinishedCalibration,
	  this, &MotorSpeedCalibrationWidget::respondOnCalibrationFinished);
  connect(this, &MotorSpeedCalibrationWidget::calibrationIsFinshed,
	  this,  &MotorSpeedCalibrationWidget::respondOnCalibrationFinished);

  
  connect(isReadyBox, &QCheckBox::stateChanged,
	  this, &MotorSpeedCalibrationWidget::updateIsReady);

  m_timer->start(m_isReadyInterval);
  setFocusPolicy(Qt::StrongFocus);
  setFocus(Qt::OtherFocusReason);
  updateIsReady();
}


void MotorSpeedCalibrationWidget::createAdditionalUIElements() {
  QFormLayout* formLayout = new QFormLayout();
  std::shared_ptr<MotorSpeedCalibration> calibration =
    std::static_pointer_cast<MotorSpeedCalibration>(m_calibration);

  m_searchRangeEdit = new QLineEdit();
  m_searchRangeEdit->setText(QString::number(calibration->getSearchRange()));
  m_delayEdit = new QLineEdit();
  m_delayEdit->setText(QString::number(calibration->getDelay()));
  m_stepSizeEdit = new QLineEdit();
  m_stepSizeEdit->setText((QString::number(calibration->getStepSize())));

  formLayout->addRow("Search range", m_searchRangeEdit);
  formLayout->addRow("Delay in ms", m_delayEdit);
  formLayout->addRow("Step size", m_stepSizeEdit);
  calibrationLayout->addLayout(formLayout);
}


void MotorSpeedCalibrationWidget::keyPressEvent(QKeyEvent* i_event) {
  // make the motor stop at the correct key press...or all...
  if (m_isCalibrating) {
    LOG_DEBUG("Caught a keyboard event with: " << i_event->text().toStdString());
    if (i_event->key() == Qt::Key_Any) {
      m_calibration->setFinished(true);
      LOG_DEBUG("emit userFinishedCalibration");
      emit userFinishedCalibration();
      return;
    }
  }
  return QWidget::keyPressEvent(i_event);
}


void MotorSpeedCalibrationWidget::executeCalibration() {
  LOG_DEBUG("MotorSpeedCalibrationWidget::executeCalibration");
  setEditsEnabled(false);
  m_isCalibrating = true;
  m_calibration->setFinished(false);
  grabKeyboard();
  // connect the timer to the updateAfterExecution
  connect(m_timer, &QTimer::timeout,
	  this, &MotorSpeedCalibrationWidget::evaluateExecution);
  std::thread calibrationThread(&MotorSpeedCalibration::execute, m_calibration);
  calibrationThread.detach();
}


void MotorSpeedCalibrationWidget::respondOnCalibrationFinished() {
  releaseKeyboard();
  setEditsEnabled(true);
  m_isCalibrating = false;
  m_calibration->setFinished(true);
  disconnect(m_timer, &QTimer::timeout,
	     this, &MotorSpeedCalibrationWidget::evaluateExecution);
  emit updateCalibrationOutText();  
  LOG_DEBUG("MotorSpeedCalibrationWidget::respondOnCalibrationFinished");
}


void MotorSpeedCalibrationWidget::updateCalibrationFromEdits() {
  double value;
  bool hasWorked;
  std::shared_ptr<MotorSpeedCalibration> calibration =
    std::static_pointer_cast<MotorSpeedCalibration>(m_calibration);
  value = m_searchRangeEdit->text().toDouble(&hasWorked);
  if (hasWorked) {
    calibration->setSearchRange(value);
  } else {
    m_searchRangeEdit->setText(QString::number(calibration->getSearchRange()));
    LOG_DEBUG("Could not convert Search-Range to double!");
  }

  value = m_delayEdit->text().toDouble(&hasWorked);
  if (hasWorked) {
    calibration->setDelay(value);
  } else {
    m_delayEdit->setText(QString::number(calibration->getDelay()));
    LOG_DEBUG("Could not convert Delay to double!");
  }

  value = m_stepSizeEdit->text().toDouble(&hasWorked);
  if (hasWorked) {
    calibration->setStepSize(value);
  } else {
    m_stepSizeEdit->setText(QString::number(calibration->getStepSize()));
    LOG_DEBUG("Could not convert StepSize to double!");
  }
}


void MotorSpeedCalibrationWidget::updateIsReady() {
  bool isReady(m_calibration->isReady());
  executeButton->setEnabled(isReady);
  isReadyBox->setChecked(isReady);
}


void MotorSpeedCalibrationWidget::evaluateExecution() {
  if (!m_calibration->getFinished()) {
    return;
  }
  // if it has finished
  emit calibrationIsFinshed();
}


void MotorSpeedCalibrationWidget::setEditsEnabled(const bool& i_enable) {
  m_searchRangeEdit->setEnabled(i_enable);
  m_delayEdit->setEnabled(i_enable);
  m_stepSizeEdit->setEnabled(i_enable);
}
