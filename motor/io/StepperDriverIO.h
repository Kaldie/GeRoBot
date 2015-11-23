// Copyright [2015] Ruud Cools

#ifndef MOTOR_IO_STEPPERDRIVERIO_H_
#define MOTOR_IO_STEPPERDRIVERIO_H_

#include <XMLBuilder.h>
#include <StepperDriver.h>

class StepperDriverIO: public XMLBuilder {
  GETSET(StepperDriver, m_stepperDriver, StepperDriver);
  
public:
  /// Build the stepperdiver from the xml node
  virtual void build();
  /// update the stepperdriver and remake the document
  bool update(const BaseMotor* i_stepperDriver);
  /// Constructor, the one and only
  explicit StepperDriverIO(const pugi::xml_node&);
  /// Create a StepperDriver xml node in the given parent node
  static bool createNode(pugi::xml_node* i_parent);

private:
  /// No default construction!
  StepperDriverIO();
  /// update the pins given a stepper driver
  bool updatePins(const PinVector& i_stepperDriver);
};

#endif  // MOTOR_IO_STEPPERDRIVERIO_H_
