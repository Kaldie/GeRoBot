// Copyright Ruud Cools [2017]
#ifndef GUI_WIDGET_CALIBRATION_ENDSTOPCALIBRATIONWIDGET_H_
#define GUI_WIDGET_CALIBRATION_ENDSTOPCALIBRATIONWIDGET_H_

#include <macroHeader.h>
#include "./CalibrationWidget.h"


class QWidget;
class BaseCalibration;
class QLineEdit;
class QTimer;

class EndStopCalibrationWidget : public CalibrationWidget {
  Q_OBJECT

  GETSET(std::shared_ptr<QTimer>, m_timer, Timer);
 // all the added UI elements
 private:
  QLineEdit* m_jointPosition;
  QLineEdit* m_jointMovementPerStep;


 public:
  // Constructor
  EndStopCalibrationWidget(const std::shared_ptr<BaseCalibration>& i_calibration,
         QWidget* i_parent);

 private:
  // no default constructor
  EndStopCalibrationWidget();
  // initialise the ui and connect all the things
  void initialise();
  void createAdditionalUIElements();

 private slots:
   virtual void executeCalibration() override;
   void updateJointInfo();
};

#endif  // GUI_WIDGET_CALIBRATION_ENDSTOPCALIBRATIONWIDGET_H_
