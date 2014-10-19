#ifndef RobotBuilder_H
#define RobotBuilder_H

#include "XMLBuilder.h"
#include <Robot.h>
class RobotBuilder:
public XMLBuilder
{
  //Properties
  GETSET(Robot,m_robot,Robot);

 private:
  JointController parseJointController(const pugi::xml_node&);
  RobotBuilder();

 public:
  virtual void build();

  virtual void displayTree();

  RobotBuilder(const std::string&);
};

#endif // RobotBuilder





