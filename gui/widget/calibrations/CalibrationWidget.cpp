// Copyright Ruud Cools [2017]
#include <macroHeader.h>
#include <BaseCalibration.h>
#include "./CalibrationWidget.h"


CalibrationWidget::CalibrationWidget(const std::shared_ptr<BaseCalibration>& i_calibration,
				     QWidget* i_parent /* = 0 */)
  : QWidget(i_parent),
    m_calibration(i_calibration) {
  setupUi(this);
}
