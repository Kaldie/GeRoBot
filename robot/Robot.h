#ifndef Robot_h
#define Robot_h

#include <JointController.h> //has a joint controller
#include <Point2D.h>
class Trace;

class Robot
{
 private:
    //Speed of the robot
    GETSET(float,m_speed,Speed);

    //Position of the head of the robot
    GETSET(Point2D,m_currentPosition,Position);

    //Handles the joints and can update the position after a step
    GETSET_NO_CONST(JointController,m_jointController,JointController);
    
 public:
    // actuations
    void goToPosition(const Point2D&);
    void goTrace(const Trace&);
    void definePosition(Point2D&);
    void callibrate(Trace&);
    
    Robot();
  

};

#endif
