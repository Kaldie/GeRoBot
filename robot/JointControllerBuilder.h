#ifndef JointControllerBuilder_H
#define JointControllerBuilder_H

#include "XMLBuilder.h"
#include <JointController.h>

class JointControllerBuilder: public XMLBuilder
{
  GETSET_NO_CONST(JointController,m_jointController,JointController);

 private:
  //no default contructor!
  JointControllerBuilder();
 
  void addJoints(const pugi::xml_node&);
  void parseJoint(const pugi::xml_node&);
  ArduinoMotorDriver parseActuator(const pugi::xml_node&);

 public:
  virtual void build();
  JointControllerBuilder(const pugi::xml_node&);
};

#endif // JointControllerBuilder
