// Copyright [2015] Ruud Cools
#ifndef MOTOR_IO_ROBOTIO_H_
#define MOTOR_IO_ROBOTIO_H_

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

  std::shared_ptr<JointController> parseJointController(const pugi::xml_node&);
  std::shared_ptr<SpeedController> parseSpeedController(const pugi::xml_node&);

  bool updateJointController(const std::shared_ptr<JointController>& i_jointController);
  bool updateSpeedController(const std::shared_ptr<SpeedController>& i_speedController);
 public:
  explicit RobotIO(const std::string&);
#ifdef QT
  explicit RobotIO(QFile* i_QFile);
#endif

  virtual void build();

  Robot::RobotPointer buildRobot();

  virtual void displayTree();

  void setRobotPointer(Robot::RobotPointer* i_robotPointer);

  bool update(const Robot::RobotPointer& i_newRobotPointer);

  bool store(const std::string& i_fileName);
};

#endif  // MOTOR_IO_ROBOTIO_H_
