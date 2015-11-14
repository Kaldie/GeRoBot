#ifndef JointControllerBUilder_H
#define JointControllerBUilder_H

#include <XMLBuilder.h>
#include <BaseJoint.h>
#include <StepperDriver.h>

class JointIO: public XMLBuilder {
  GETSET(BaseJoint::JointPointer, m_jointPointer, JointPointer);

 public:
  virtual void build();
  explicit JointIO(const pugi::xml_node&);
  bool update(const BaseJoint::JointPointer& i_jointPointer);

 private:
  // No default contructor!!
  JointIO();
  StepperDriver parseStepperDriver(const pugi::xml_node&);
  void makeSharedJoint();
  void handleConversionMap();
  bool isValidNode(const pugi::xml_node&) const;
};

#endif // JointControllerBUilder
