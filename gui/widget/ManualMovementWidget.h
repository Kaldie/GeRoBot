#ifndef GUI_WIDGET_MANUALMOVEMENTWIDGET_H_
#define GUI_WIDGET_MANUALMOVEMENTWIDGET_H_


class Robot;
#include <macroHeader.h>
#include <QWidget>
#include <ui_ManualMovementWidget.h>

// class is more a container then an actual widget. It combines the RobotMovementWidget with
// the RobotPositionWidget, which is fine
class ManualMovementWidget: public QWidget, private Ui::ManualMovementWidget {
  Q_OBJECT
  GETSET(std::shared_ptr<Robot>, m_robotPointer, Robot);
  void initialise();
 public:
  ManualMovementWidget(const std::shared_ptr<Robot>& i_robot,
                       QWidget* i_parent);


};

#endif  // GUI_WIDGET_MANUALMOVEMENTWIDGET_H_
