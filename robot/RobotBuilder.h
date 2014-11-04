#ifndef RobotBuilder_H
#define RobotBuilder_H

#include "XMLBuilder.h"
#include <JointController.h>


class JointController;

class RobotBuilder: public XMLBuilder
{
  //Properties
  GET(RobotPointer,m_robotPointer,RobotPointer);

 private:
  RobotBuilder();

  JointController parseJointController(const pugi::xml_node&);

 public:
	RobotBuilder(const std::string&);
	
  virtual void build();
	
  virtual void displayTree();
	
	void setRobotPointer(Robot* i_robotPointer);
};

#endif // RobotBuilder





