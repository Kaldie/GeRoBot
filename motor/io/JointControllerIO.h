
#ifndef JointControllerIO_H
#define JointControllerIO_H

#include <BaseJoint.h>
#include <XMLBuilder.h>
#include <ArduinoMotorDriver.h>

class JointController;

class JointControllerIO: public XMLBuilder {
  GETSET_NO_CONST(std::shared_ptr<JointController>,
		  m_jointController,
		  JointController);

 private:
  typedef std::tuple<pugi::xml_node,
                     BaseJoint::JointPointer> JointTuple;
  typedef std::vector<JointTuple> JointTupleVector;

  /// Build all the joints and store with the id and node
  JointTupleVector buildAllJoints() const;

  /// For each joint, find its child in the vector
  void resolveDependencies(const JointTupleVector& i_vector) const;

  // no default contructor!
  JointControllerIO();

  void addJoints();

  BaseJoint::JointPointer parseJoint(const pugi::xml_node&);

  ArduinoMotorDriver parseActuator(const pugi::xml_node&);

  bool updateActuatorNode();

  JointTupleVector updateJointNodes();

  bool updateDependancies(const JointTupleVector&);

  void updateNumberOfJointNode();

  bool removeLastJointNode();

  bool addJointNode();

  void getNextJointNode(pugi::xml_node* i_node) const;
 public:

  explicit JointControllerIO(const pugi::xml_node&);

  virtual void build();
  bool update(const std::shared_ptr<JointController>& i_newJointController);

  virtual ~JointControllerIO(){};
};
#endif // JointControllerIO
