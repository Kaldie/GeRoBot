#ifndef StepperDriverBuilder_H
#define StepperDriverBuilder_H

#include "XMLBuilder.h"
#include <StepperDriver.h>

class StepperDriverBuilder: public XMLBuilder
{
  GETSET(StepperDriver,m_stepperDriver,StepperDriver);
  
 private:
  //No default construction!
  StepperDriverBuilder();
	bool updatePins(const PinVector& i_stepperDriver);

 public:
  virtual void build();
	bool update(const BaseMotor* i_stepperDriver);
  StepperDriverBuilder(const pugi::xml_node&);
};

#endif // StepperDriverBuilder
