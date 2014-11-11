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

	bool updateJointController(const JointController& i_jointController);

 public:
	RobotBuilder(const std::string&);
	
  virtual void build();
	
  virtual void displayTree();
	
	void setRobotPointer(Robot* i_robotPointer);
	
	bool update(const RobotPointer& i_newRobotPointer);

	bool store(const std::string& i_fileName);
};

#endif // RobotBuilder





