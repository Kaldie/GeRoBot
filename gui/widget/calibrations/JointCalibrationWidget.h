// Copyright [2017] Ruud Cools
#ifndef GUI_WIDGET_CALIBRATION_JOINTCALIBRATIONWIDGET_H_
#define GUI_WIDGET_CALIBRATION_JOINTCALIBRATIONWIDGET_H_

#include <QWidget>
#include <macroHeader.h>
#include "./ui_JointCalibrationWidget.h"

class Robot;
class BaseJoint;
class CalibrationDirector;
class BaseCalibration;

class JointCalibrationWidget: public QWidget, private Ui::JointCalibrationWidget {
  Q_OBJECT
 private:
  typedef std::map<const std::shared_ptr<BaseJoint>,bool> JointMap;
  typedef std::vector<std::shared_ptr<BaseCalibration>> CalibrationVector;
  GETSET(std::shared_ptr<Robot>, m_robot, Robot);
  GET(JointMap, m_jointMap, JointMap);
  GETSET(std::shared_ptr<CalibrationDirector>, m_director, Director);
  GET(CalibrationVector, m_calibrationVector, CalibrationVector);
  GET(QButtonGroup, m_calibrationButtonGroup, CalibrationGroup);
 public:
  // constructor
  JointCalibrationWidget(const std::shared_ptr<Robot>& i_robot,
			 QWidget* i_parent = 0);

 private:
  // no default constructor
  JointCalibrationWidget();

  // populate the list box where the joint types are displayed
  void populateTypeListbox();

  // initialise all the connections etc.
  void initialisation();

  std::shared_ptr<BaseJoint> resolveJoint() const;

  void initialiseJointMap();

  void populateButtonGroup();

  void enableSelection(const bool& i_enable);

 private slots:
  // populate the list box where the joint number is displayed
  void populateNumberListbox();

  // update the inforamtion of the joint displayed in the widget
  void updateJointInforamtion();

  // remove all the calibration childeren
  void clearCalibrations();

  void toggleCalibration();

  // create calibration childer suitable for the selected widget
  void addCalibrationWidget();

  // update the joint map
  void updateJointMap(const bool& i_checked);

  // update the number of calibrations
  void updateNumberOfCalibrations();

  // update the director to match the gui
  void updateCalibrationSelection();

};

#endif  // GUI_WIDGET_CALIBRATION_JOINTCALIBRATIONWIDGET_H_
