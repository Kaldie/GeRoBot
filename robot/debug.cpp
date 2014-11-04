#include <macroHeader.h>
#include "RobotBuilder.h"

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
}    

int main()
{
    testRobotBuilder();

}
