// Copyright Ruud Cools

#include <macroHeader.h>
#include <BaseCalibration.h>
#include "./CalibrationWidgetFactory.h"
#include "./CalibrationWidget.h"
#include "./EndStopCalibrationWidget.h" 
#include "./PointCalibrationWidget.h"
#include "./MotorSpeedCalibrationWidget.h"


typedef MotorSpeedCalibrationWidget MaximumSpeedCalibrationWidget;
typedef MotorSpeedCalibrationWidget AccelerationCalibrationWidget;
typedef MotorSpeedCalibrationWidget PullInCalibrationWidgtet;

CalibrationWidget* CalibrationWidgetFactory::getWidget
(const std::shared_ptr<BaseCalibration>& i_calibration,
 QWidget* i_parent /* = 0 */) {
  CalibrationWidget* widget(nullptr);
  if (!i_calibration) {
    LOG_DEBUG("Found an empty calibration pointer!!!!");
    return widget;
  }
  switch (i_calibration->getType()) {
  case BaseCalibration::PointCalibration: {
    widget = new PointCalibrationWidget(i_calibration, i_parent);
    break;
  }
  case BaseCalibration::EndStopCalibration: {
    widget = new EndStopCalibrationWidget(i_calibration, i_parent);
    break;
  }
  case BaseCalibration::PullInCalibration: {
    widget = new PullInCalibrationWidgtet(i_calibration, i_parent);
    break;
  }
  case BaseCalibration::MaximumCalibration: {
    widget = new MaximumSpeedCalibrationWidget(i_calibration, i_parent);
    break;
  }
  case BaseCalibration::AccelerationCalibration: {
    widget = new AccelerationCalibrationWidget(i_calibration, i_parent);
    break;
  }
  case BaseCalibration::Base: {
    LOG_ERROR("Base calibration is not a vailable calibration!");
  }
  }
  return widget;
}


/*
  #include "./PointCalibrationWidget.h"
  #include "./EndPointCalibrationWidget.h"
  #include "./PullInCalibrationWidgtet.h"
  #include "./MaximumSpeedCalibrationWidget.h"
  #include "./AccelerationCalibrationWidget.h"
*/

