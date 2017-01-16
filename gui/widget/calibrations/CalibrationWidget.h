// Copyright Ruud Cools [2017]
#ifndef GUI_WIDGET_CALIBRATION_CALIBRATIONWIDGET_H_
#define GUI_WIDGET_CALIBRATION_CALIBRATIONWIDGET_H_

#include <QWidget>
#include <macroHeader.h>
#include "./ui_CalibrationWidget.h"

class BaseCalibration;

class CalibrationWidget: public QWidget, private Ui::CalibrationWidget {
  Q_OBJECT
 private:
  GETSET(std::shared_ptr<BaseCalibration>, m_calibration, Calibration);
 public:
  CalibrationWidget(const std::shared_ptr<BaseCalibration>& i_calibration,
		    QWidget* i_parent = 0);
};

#endif  // GUI_WIDGET_CALIBRATION_CALIBRATIONWIDGET_H_
