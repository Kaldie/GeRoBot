#ifndef GUI_WIDGET_CALIBRATION_MOTORSPEEDCALIBRATIONWIDGET_H_
#define GUI_WIDGET_CALIBRATION_MOTORSPEEDCALIBRATIONWIDGET_H_

#include "./CalibrationWidget.h"

class QKeyEvent;
class QTimer;


// Give the motorspeed calibrations a ui
class MotorSpeedCalibrationWidget : public CalibrationWidget {
  Q_OBJECT
  GETSET(int, m_isReadyInterval, IsReadyInterval);
  GET(bool, m_isCalibrating, Calibrating);
 private:
  QLineEdit* m_searchRangeEdit;
  QLineEdit* m_delayEdit;
  QLineEdit* m_stepSizeEdit;
  QTimer* m_timer;
  
 public:
  MotorSpeedCalibrationWidget(const std::shared_ptr<BaseCalibration>& i_calibration,
			      QWidget* i_parent = 0);

  ~MotorSpeedCalibrationWidget();
 signals:
  //will be emited when the calibration is finished by user input
  void userFinishedCalibration();
  // will be emited if the calibration has ran its course
  void calibrationIsFinshed();
 protected:
  virtual void keyPressEvent(QKeyEvent* i_event) override;
  
 private:
  void initialise();
  void createAdditionalUIElements();

 private slots:
  virtual void executeCalibration() override;
  virtual void respondOnCalibrationFinished();
  void updateCalibrationFromEdits();
  void updateIsReady();
  void evaluateExecution();
  void setEditsEnabled(const bool& i_enable);
};

#endif  // GUI_WIDGET_CALIBRATION_MOTORSPEEDCALIBRATIONWIDGET_H_
