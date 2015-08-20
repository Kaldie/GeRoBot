// Copyright [2015] Ruud Cools
#ifndef MOTOR_IO_ROBOTIO_H_
#define MOTOR_IO_ROBOTIO_H_

#include <JointController.h>
#include <XMLBuilder.h>
#include <Robot.h>

class JointController;

class RobotIO: public XMLBuilder {
  // Properties
  GET(Robot::RobotPointer,
      m_robotPointer,
      RobotPointer);

 private:
  RobotIO();
  JointController parseJointController(const pugi::xml_node&);
  bool updateJointController(const JointController& i_jointController);

 public:
  explicit RobotIO(const std::string&);
  virtual void build();
  virtual void displayTree();
  void setRobotPointer(Robot* i_robotPointer);
  bool update(const Robot::RobotPointer& i_newRobotPointer);
  bool store(const std::string& i_fileName);
};

#endif  // MOTOR_IO_ROBOTIO_H_
