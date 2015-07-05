#ifndef JointControllerBuilder_H
#define JointControllerBuilder_H

#include <JointController.h>
#include "./XMLBuilder.h"

class JointControllerBuilder: public XMLBuilder {
  GETSET_NO_CONST(JointController,
                    m_jointController,
                    JointController);

 private:
  // no default contructor!
  JointControllerBuilder();
  void addJoints();
  void parseJoint(const pugi::xml_node&);
  ArduinoMotorDriver parseActuator(const pugi::xml_node&);
  bool updateActuatorNode();
  bool updateJointNodes();

 public:
  explicit JointControllerBuilder(const pugi::xml_node&);

  virtual void build();
  bool update(const JointController& i_newJointController);
};

#endif // JointControllerBuilder
