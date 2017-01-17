// Copyright [2017] Ruud Cools

#include "./PointCalibrationWidget.h"

PointCalibrationWidget::PointCalibrationWidget(const std::shared_ptr<BaseCalibration>& i_calibration,
					       QWidget* i_parent = 0) 
  : CalibrationWidget(i_calibration, i_parent) {
  initialise();
}


void PointCalibration::initialise() {
  // create robot movement widget
}
