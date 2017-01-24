#ifndef GUI_WIDGET_CALIBRATION_POINTCALIBRATIONWIDGET_H_
#define GUI_WIDGET_CALIBRATION_POINTCALIBRATIONWIDGET_H_

#include <macroHeader.h>
#include "./CalibrationWidget.h"

class QListWidget;
class QLineEdit;
class QPushButton;
class QListWidgetItem;
class Point2D;
class QWidget;

class PointCalibrationWidget : public CalibrationWidget {
  Q_OBJECT
  GETSETPOINTER(QListWidget, m_pointListBox, PointListBox);
  GETSETPOINTER(QLineEdit, m_xLineEdit, XLineEdit);
  GETSETPOINTER(QLineEdit, m_yLineEdit, YLineEdit);
  GETSETPOINTER(QPushButton, m_addPointButton, AddPointButton);
  GETSETPOINTER(QPushButton, m_removePointButton, RemovePointButton);
  GETSETPOINTER(QPushButton, m_registerPointButton, RegisterPointButton);
  GET(int, m_currentPointNumber, CurrentPointNumber);

 public:
  PointCalibrationWidget(const std::shared_ptr<BaseCalibration>& i_calibration,
			 QWidget* i_parent = 0);
 private:
  void initialise();
  void createAdditionalUIElements();
  void getPointFromListItem(QListWidgetItem* i_item, Point2D* o_point);
  void connectModelWithMovementWidget();
 private slots:
  virtual void executeCalibration() override;
  void addPointToList();
  void registerPoint();
  void updateAddPointButton();
  void removePointFromWidget();
  void updatePointIcons();
};

#endif  // GUI_WIDGET_CALIBRATION_POINTCALIBRATIONWIDGET_H_
