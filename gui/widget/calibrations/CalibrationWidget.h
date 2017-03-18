// Copyright Ruud Cools [2017]
#ifndef GUI_WIDGET_CALIBRATION_CALIBRATIONWIDGET_H_
#define GUI_WIDGET_CALIBRATION_CALIBRATIONWIDGET_H_

#include <QWidget>
#include <macroHeader.h>
#include "./ui_CalibrationWidget.h"

class BaseCalibration;

class CalibrationWidget: public QWidget, protected Ui::CalibrationWidget {
  Q_OBJECT

 private:
  GETSETPROTECTED(std::shared_ptr<BaseCalibration>, m_calibration, Calibration);
  void initialise();

 private slots:
  virtual void executeCalibration();
  void skipCalibration();
  void updateOutputText();
  void applyCalibrationOutput();

 protected slots:
  void setEnabled(const bool&);
  
 public:
  CalibrationWidget(const std::shared_ptr<BaseCalibration>& i_calibration,
		    QWidget* i_parent = 0);
 signals:
  void calibrationFinished(const bool&);
  void updateCalibrationOutText();
};

#endif  // GUI_WIDGET_CALIBRATION_CALIBRATIONWIDGET_H_
