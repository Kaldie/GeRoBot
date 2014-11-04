/*
  Robot is the basis of the project.
  Robot is the root of a object which looks like: Robot->Joint->Actuator

  Robot will also be calculating the traces
  To calculating traces we need info on the joints
  These traces will be handled by the actuators of the joints    
*/

#include <macroHeader.h>
#include "Robot.h"
#include <BaseTraceCalculator.h>
#include <PinState.h>
#include <Trace.h>

Robot::Robot()
  : 
  m_speed(0), 
  m_currentPosition{0,50}
{}

void Robot::goToPosition(const Point2D &i_position)
{
  Trace thisTrace(m_currentPosition,i_position);
  BaseTraceCalculator baseTraceCalculator(&m_jointController);
  baseTraceCalculator.calculateTrace(&thisTrace,
				     m_currentPosition);
  getJointController().actuate();  
}

/*
void Robot::actuate()
{

  std::vector<pthread_t> treadIDs;
  int returnValue=0;

  // method to run the joints...with a bit of luck
  for(int i=0;i<m_numberOfJoints;i++)
    {
      pthread_t treadID;

      void* jointPointer=static_cast<void*>(&m_jointVector[i]);
      //Joint::threadedMoveSteps(jointPointer);     
      returnValue = pthread_create(&treadID, NULL, Joint::threadedMoveSteps, jointPointer);  
      treadIDs.push_back(treadID);      
      //check if the thread is created!
      if ( returnValue != 0 )  
	{  
	  std::cerr<<"Error in thread creation..."<<std::endl;  
	} 
      else
	std::cout<<"Created thread: "<<treadID<<std::endl;
       
    }
  
  //  sleep(100);
  
  for(std::vector<pthread_t>::iterator itr=treadIDs.begin();itr!=treadIDs.end();itr++)
    {
      //      std::cout<<*itr<<std::endl;
      returnValue = pthread_join(*itr, NULL);  
      if ( returnValue != 0 )  
	{  
	  std::cerr<<"Error in thread join...."<<std::endl;  
	} 
    }

}
*/
