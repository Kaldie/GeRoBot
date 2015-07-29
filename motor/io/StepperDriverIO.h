#ifndef StepperDriverIO_H
#define StepperDriverIO_H

#include <XMLBuilder.h>
#include <StepperDriver.h>

class StepperDriverIO: public XMLBuilder
{
  GETSET(StepperDriver,m_stepperDriver,StepperDriver);
  
 private:
  //No default construction!
  StepperDriverIO();
	bool updatePins(const PinVector& i_stepperDriver);

 public:
  virtual void build();
	bool update(const BaseMotor* i_stepperDriver);
  StepperDriverIO(const pugi::xml_node&);
};

#endif // StepperDriverIO
