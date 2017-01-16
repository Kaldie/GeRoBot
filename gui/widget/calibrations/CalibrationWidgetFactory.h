/// Copyright Ruud Cools [2017]

#ifndef GUI_WIDGET_CALIBRATION_CALIBRATIONWIDGETFACTORY_H_
#define GUI_WIDGET_CALIBRATION_CALIBRATIONWIDGETFACTORY_H_

class BaseCalibation;
class CalibrationWidget;
class QWidget;

// Factory is responsible for creating valid calibration widgets
// AFTER CREATION, IT IS NOT RESPONSIBLE FOR DESTRUCTING IT PROPERLY
// ALL WIDGETS WILL BE CONSTRUCTED IN THE HEAP
class CalibrationWidgetFactory {
 private:
  //  TODO: check if the commented parameters are necessary and if they need to be constructed
  //  GET(std::shared_ptr<Robot>, m_robot, Robot);
  //  GET(std::shared_ptr<BaseJoint>, m_joint, Joint);
  
  // create the different widgets
  static CalibrationWidget* createMotorCalibrationWidget(const std::shared_ptr<BaseCalibation>&);
  static CalibrationWidget* createPointCalibrationWidget(const std::shared_ptr<BaseCalibation>&);
  static CalibrationWidget* createEndPointCalibrationWidget(const std::shared_ptr<BaseCalibation>&);
  
  // no public constructor
  CalibrationWidgetFactory();
 public:
  
  //  CalibrationWidgetFactory(const std::shared_ptr<Robot>& i_robot,
  //			   const std::shared_ptr<BaseJoint>& i_joint);
  static CalibrationWidget* getWidget(const std::shared_ptr<BaseCalibration>&, QWidget* = 0);
};
#endif  // GUI_WIDGET_CALIBRATION_CALIBRATIONWIDGETFACTORY_H_
