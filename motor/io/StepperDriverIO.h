// Copyright [2015] Ruud Cools

#ifndef MOTOR_IO_STEPPERDRIVERIO_H_
#define MOTOR_IO_STEPPERDRIVERIO_H_

#include <XMLBuilder.h>
#include <StepperDriver.h>

class StepperDriverIO: public XMLBuilder {
  GETSET(StepperDriver, m_stepperDriver, StepperDriver);
  
private:
  // No default construction!
  StepperDriverIO();
  bool updatePins(const PinVector& i_stepperDriver);

public:
  /// Build the stepperdiver from the xml node
  virtual void build();
  /// update the stepperdriver and remake the document
  bool update(const BaseMotor* i_stepperDriver);
  /// Constructor, the one and only
  explicit StepperDriverIO(const pugi::xml_node&);
};

#endif  // MOTOR_IO_STEPPERDRIVERIO_H_
