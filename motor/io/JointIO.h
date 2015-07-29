#ifndef JointControllerBUilder_H
#define JointControllerBUilder_H

#include <XMLBuilder.h>
#include <BaseJoint.h>
#include <StepperDriver.h>

typedef std::shared_ptr<BaseJoint> JointPointer;

class JointIO: public XMLBuilder {
  GETSET(JointPointer, m_jointPointer, JointPointer);

 private:
  // No default contructor!!
  JointIO();
  StepperDriver parseStepperDriver(const pugi::xml_node&);

 public:
  virtual void build();
  explicit JointIO(const pugi::xml_node&);
  bool update(const JointPointer& i_jointPointer);
};

#endif // JointControllerBUilder
