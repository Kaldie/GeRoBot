
#ifndef JointControllerIO_H
#define JointControllerIO_H

#include <JointController.h>
#include <BaseJoint.h>
#include <XMLBuilder.h>

class JointControllerIO: public XMLBuilder {
  GETSET_NO_CONST(JointController,
                  m_jointController,
                  JointController);

 private:
  typedef std::tuple<pugi::xml_node,
                     BaseJoint::JointPointer> JointTupple;
  typedef std::vector<JointTupple> JointTupleVector;

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
  bool update(const JointController& i_newJointController);

  virtual ~JointControllerIO(){};
};
#endif // JointControllerIO
