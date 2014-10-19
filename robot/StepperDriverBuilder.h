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

 public:
  virtual void build();
  StepperDriverBuilder(const pugi::xml_node&);
};

#endif // StepperDriverBuilder
