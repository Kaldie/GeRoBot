#include <macroHeader.h>
#include "RobotBuilder.h"
#include <Robot.h>
#include <BaseJoint.h>
#include <BaseMotor.h>
void testRobot()
{

}

void testRobotBuilder()
{
    std::cout<<"lol";
    RobotBuilder robotBuilder("/home/ruud/tmp.xml");
		robotBuilder.build();
		RobotPointer robotPointer = robotBuilder.getRobotPointer();
		LOG_INFO(robotPointer->getJointController().getNumberOfJoints());
		robotPointer->getJointController().getJoint(Rotational)->setPosition(44.3);
		robotPointer->getJointController().getJoint(Rotational)->getMotor()->setPins(std::vector<int>{1,2,3});
		robotPointer->getJointController().getJoint(Rotational)->getMotor()->setHoldMotor(false);
		robotPointer->getJointController().getJoint(Translational)->setMovementPerStep(0.12345);
		ArduinoMotorDriver arduinoMotorDriver = robotPointer->getJointController().getActuator();
		arduinoMotorDriver.setReducedSpeed(false);
		arduinoMotorDriver.setSerialRegularExpresion("lalalalalala");
		robotPointer->getJointController().setActuator(arduinoMotorDriver);
		robotBuilder.update(robotPointer);

		robotBuilder.store("./lala.xml");
}    

int main()
{
    testRobotBuilder();

}
