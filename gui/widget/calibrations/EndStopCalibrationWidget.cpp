// Copyright Ruud Cools [2017]
#include <macroHeader.h>
#include "./EndStopCalibrationWidget.h"
#include <QWidget>
#include <BaseCalibration.h>
#include <QTimer>
#include <QLineEdit>
#include <BaseJoint.h>
#include <EndStop.h>
#include <thread>
#include <EndStopCalibration.h>

EndStopCalibrationWidget::EndStopCalibrationWidget(const std::shared_ptr<BaseCalibration>& i_calibration,
						   QWidget* i_parent) 
  : CalibrationWidget(i_calibration, i_parent),
    m_timer(std::make_shared<QTimer>(this)) {
  if (i_calibration->getType() != BaseCalibration::EndStopCalibration) {
    LOG_ERROR("Did not recieve the proper calibration!");
  }
  initialise();
}


void EndStopCalibrationWidget::initialise() {
  connect(m_timer.get(), &QTimer::timeout,
	  this, &EndStopCalibrationWidget::updateJointInfo);
  createAdditionalUIElements();
}


void EndStopCalibrationWidget::createAdditionalUIElements() {
  // layout box for property label line edit combinations
  // add joint position info
  QHBoxLayout* hBox = new QHBoxLayout();
  hBox->addWidget(new QLabel("Estimated Joint Position: "));
  m_jointPosition = new QLineEdit();
  m_jointPosition->setReadOnly(true);
  hBox->addWidget(m_jointPosition);
  calibrationLayout->addLayout(hBox);
  // add joint movement per step info
  hBox = new QHBoxLayout();
  hBox->addWidget(new QLabel("Estimated Joint Movement per Step: "));
  m_jointMovementPerStep = new QLineEdit();
  m_jointMovementPerStep->setReadOnly(true);
  hBox->addWidget(m_jointMovementPerStep);
  // add the joint position
  calibrationLayout->addLayout(hBox);

  updateJointInfo();
  // add end stop indicators as we have some
  int i = 1;
  QLineEdit* tmpLineEdit;
  for (const auto& endStop : m_calibration->getJoint()->getEndStops()) {
    hBox = new QHBoxLayout();
    hBox->addWidget(new QLabel
		    (QString("EndStop %1 position:").arg(i)));
    tmpLineEdit = new QLineEdit(QString::number(endStop->getPosition()));
    tmpLineEdit->setReadOnly(true);
    hBox->addWidget(tmpLineEdit);
    calibrationLayout->addLayout(hBox);
    ++i;
  }
}


void EndStopCalibrationWidget::updateJointInfo() {
  LOG_DEBUG("Update joint info");
  std::shared_ptr<BaseJoint> joint(m_calibration->getJoint());
  m_jointPosition->setText(QString::number(joint->getPosition()));
  m_jointMovementPerStep->setText(QString::number(joint->getMovementPerStep()));
}


void EndStopCalibrationWidget::executeCalibration() {
  setEnabled(false);
  connect(m_timer.get(),&QTimer::timeout,
	  this, &EndStopCalibrationWidget::evaluateCurrentCalibration);
  std::thread endStopCalibrationThread(&EndStopCalibration::execute,
				       std::static_pointer_cast<EndStopCalibration>(m_calibration));
  endStopCalibrationThread.detach();
  m_timer->start(300);
}


void EndStopCalibrationWidget::evaluateCurrentCalibration() {
  notifyCurrentState();
  if (std::static_pointer_cast<EndStopCalibration>(m_calibration)->getState() == 
      EndStopCalibration::Finished) {
  LOG_DEBUG("Done execute calibration");
  m_timer->stop();
  setEnabled(true);
  emit updateCalibrationOutText();
  }
}


void EndStopCalibrationWidget::notifyCurrentState() {
  std::string currentStateString;
  switch (std::static_pointer_cast<EndStopCalibration>(m_calibration)->getState()) {
  case EndStopCalibration::Idle:
    currentStateString = "Idle";
    break;
  case EndStopCalibration::Moving:
    currentStateString = "Moving";
    break;
  case EndStopCalibration::Sending:
    currentStateString = "Sending";
    break;
  case EndStopCalibration::Calculating:
    currentStateString = "Calculating";
    break;
  case EndStopCalibration::Finished:
    currentStateString = "Finished";
  default:
    currentStateString = "Unknown";\
  }
  
  outputEdit->setPlainText(QString::fromStdString("Current state is: " + currentStateString));
}
