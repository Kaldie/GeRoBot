#ifndef JointControllerBUilder_H
#define JointControllerBUilder_H

#include <XMLBuilder.h>
#include <BaseJoint.h>
//#include <StepperDriver.h>

class StepperDriver;

class JointIO: public XMLBuilder {
  GETSET(BaseJoint::JointPointer, m_jointPointer, JointPointer);

 public:
  /// Build the joint based on the given node
  virtual void build();
  /// Constructor
  explicit JointIO(const pugi::xml_node&);
  /// Update the node based on the given joint pointer
  bool update(const BaseJoint::JointPointer& i_jointPointer);
  /// Create a new joint node
  static bool createNode(pugi::xml_node*);
 private:
  // No default contructor!!
  JointIO();
  /// Constructor the stepper driver based on the child in the joint
  std::shared_ptr<StepperDriver> parseStepperDriver(const pugi::xml_node&);
  void makeSharedJoint();
  void handleConversionMap();
  bool isValidNode(const pugi::xml_node&) const;
};

#endif // JointControllerBUilder
