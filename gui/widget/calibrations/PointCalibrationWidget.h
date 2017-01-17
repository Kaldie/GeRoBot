#ifndef GUI_WIDGET_CALIBRATION_POINTCALIBRATIONWIDGET_H_
#define GUI_WIDGET_CALIBRATION_POINTCALIBRATIONWIDGET_H_

#include "./CalibrationWidget.h"

class PointCalibratoinWidget : public CalibrationWidget {
 public:
  PointCalibrationWidget(const std::shared_ptr<BaseCalibration>& i_calibration,
			 QWidget* i_parent = 0);

 private:
  void initialise();
}

#endif  // GUI_WIDGET_CALIBRATION_POINTCALIBRATIONWIDGET_H_
