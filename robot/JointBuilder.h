#ifndef JointControllerBUilder_H
#define JointControllerBUilder_H

#include "XMLBuilder.h"
#include <BaseJoint.h>
#include <StepperDriver.h>

typedef std::shared_ptr<BaseJoint> JointPointer;

class JointBuilder: public XMLBuilder
{
  GETSET(JointPointer,m_jointPointer,JointPointer);

 private:
  //No default contructor!!
  JointBuilder();
  StepperDriver parseStepperDriver(const pugi::xml_node&);
  
 public:
  virtual void build();
  JointBuilder(const pugi::xml_node&);
};

#endif // JointControllerBUilder
