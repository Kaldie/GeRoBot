#ifndef JointControllerBuilder_H
#define JointControllerBuilder_H

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
  /// No default contructor!!
  JointIO();

  /// Constructor the stepper driver based on the child in the joint
  std::shared_ptr<StepperDriver> parseStepperDriver(const pugi::xml_node&);

  /// set the properties which are common for all joints
  void setBaseJointProperties();

  /// make the joint factory and make a proper shared pointer
  void makeSharedJoint();

  /// creating the movement conversion map
  void handleConversionMap();

  /// Converting the xml properties, which are in degree, to radians which are used in code
  void convertToRadians();

  /// check if the node is valid, and we can build it
  bool isValidNode(const pugi::xml_node&) const;

  /// Add end stops to the joint if they are available
  void addEndStops();
};

#endif // JointControllerBuilder
