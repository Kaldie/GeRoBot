// Copyright Ruud Cools [2017]
#include <macroHeader.h>
#include <BaseCalibration.h>
#include "./CalibrationWidget.h"
#include <CalibrationOutput.h>

CalibrationWidget::CalibrationWidget(const std::shared_ptr<BaseCalibration>& i_calibration,
             QWidget* i_parent /* = 0 */)
  : QWidget(i_parent),
    m_calibration(i_calibration) {
  setupUi(this);
  initialise();
}


void CalibrationWidget::initialise() {
  nameLabel->setText(QString::fromStdString(m_calibration->name()));
  isReadyBox->setChecked(m_calibration->isReady());
  connect(executeButton, SIGNAL(clicked()), this, SLOT(executeCalibration()));
  connect(skipButton, SIGNAL(clicked()), this, SLOT(skipCalibration()));
  connect(this,&CalibrationWidget::updateCalibrationOutText,
          this,&CalibrationWidget::updateOutputText);
  connect(applyButton, &QPushButton::clicked,
	  this, &CalibrationWidget::applyCalibrationOutput);
}

void CalibrationWidget::executeCalibration() {
  /// execute the calibration and update the output thingy
  LOG_DEBUG("executeCalibration");
  emit calibrationFinished(false);
}


void CalibrationWidget::skipCalibration() {
  LOG_DEBUG("skipCalibration");
  emit calibrationFinished(false);
}


void CalibrationWidget::updateOutputText() {
  LOG_DEBUG("updateCalibrationText()");
  QString output;
  for (const auto& chapter : m_calibration->getOutput()->getAllChapters()) {
    output.append(QString::fromStdString(chapter));
    output.append("\n");
  }
  outputEdit->setPlainText(output);
}


void CalibrationWidget::applyCalibrationOutput() {
  LOG_DEBUG("applyCalibrationOutput");
  m_calibration->apply();
  emit calibrationFinished(true);
}


void CalibrationWidget::setEnabled(const bool& i_enable) {
  executeButton->setEnabled(i_enable);
  skipButton->setEnabled(i_enable);
  applyButton->setEnabled(i_enable);
}

