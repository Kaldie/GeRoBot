// Copyright [2015] Ruud Cools

#ifndef MOTOR_IO_SPEEDCONTROLLERIO_H_
#define MOTOR_IO_SPEEDCONTROLLERIO_H_

#include <XMLBuilder.h>
class SpeedController;

class SpeedControllerIO: public XMLBuilder {
  GETSET(std::shared_ptr<SpeedController>, m_speedController, SpeedController);
  
public:
  /// Build the stepperdiver from the xml node
  virtual void build();
  /// update the stepperdriver and remake the document
  bool update(const std::shared_ptr<SpeedController>& i_speedController);
  /// Constructor, the one and only
  explicit SpeedControllerIO(const pugi::xml_node&);
  /// Create a StepperDriver xml node in the given parent node
  static bool createNode(pugi::xml_node* i_parent);

private:
  /// No default construction!
  SpeedControllerIO();
  void resolveSpeedControllerType();
};

#endif  // MOTOR_IO_SPEEDCONTROLLERIO_H_
