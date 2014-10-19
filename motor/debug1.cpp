/*Stepper driver class
  Create the control the driver

  Dep WiringPi
*/
#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
#include <memory>

#include <PinState.h>
#include "BaseMotor.h"
//#include "StepperMotor.h"
#include "StepperDriver.h"

#include "BaseJoint.h"
#include "RotationalJoint.h"
#include "TranslationalJoint.h"

#include "JointController.h"

#include <Trace.h>
#include <LineTraceCalculator.h>
using namespace std;

bool DISPLAY(false);


bool verifyPoint(const Point2D& i_refPoint,
		 const Point2D& i_measPoint)
{
    float tolerance=0.0001;
    if(DISPLAY)
	cout<<i_refPoint.x<<", "<<i_measPoint.x<<endl;
	
    if(i_refPoint.x>(i_measPoint.x+tolerance)) return false;
    if(i_refPoint.x<(i_measPoint.x-tolerance)) return false;

    if(DISPLAY)
	cout<<i_refPoint.y<<", "<<i_measPoint.y<<endl;

    if(i_refPoint.y>(i_measPoint.y+tolerance)) return false;
    if(i_refPoint.y<(i_measPoint.y-tolerance)) return false;
    return true;
}

bool validateSequence(const PinStateSequence& i_ref,
		      const PinStateSequence& i_meas)
{
    if(i_ref.size()!=i_meas.size())
	{
	    cout<<"Size reference= "<<i_ref.size()<<endl;
	    cout<<"Size meas= "<<i_meas.size()<<endl;
	    return false;
	}
    
    
    if(DISPLAY)
	{
	    for(PinStateSequence::const_iterator itr=i_ref.begin();
		itr!=i_ref.end();
		itr++)
		cout<<*itr<<", ";
	    cout<<endl;
	    for(PinStateSequence::const_iterator itr=i_meas.begin();
		itr!=i_meas.end();
		itr++)
		cout<<*itr<<", ";
	    cout<<endl;
	}
    
    PinStateSequence::const_iterator refIter=i_ref.begin();
    PinStateSequence::const_iterator measIter=i_meas.begin();
    
    for(;
	refIter!=i_ref.end();
	refIter++,measIter++)
	{
	    if((*refIter)!=(*measIter))
		return false;
	}

    return true;
}

void testPinState()
{
    PinState pinState;
    assert(pinState.getNumericValue()==0);
}


StepperDriver testStepperDriver()
{
    cout<<"Start with testStepperDriver()"<<endl;
    StepperDriver stepperDriver(2,3,4);
    assert(stepperDriver.getCurrentPinState()==14);
    stepperDriver.updatePinState(2,0);
    assert(stepperDriver.getCurrentPinState()==12);
    stepperDriver.updatePinState(2,1);
    stepperDriver.updatePinState(3,0);
    assert(stepperDriver.getCurrentPinState()==10);

    PinStateSequence pinStateSequence;
    stepperDriver.setPins({2,3,4});
    pinStateSequence.push_back(stepperDriver.getCurrentPinState());
    stepperDriver.moveStep("CCW",pinStateSequence);
    PinStateSequence testSequence{8+4+2,8+4,8,0,8,8+2};
    assert(pinStateSequence==testSequence);

    if(DISPLAY)
	{
	    cout<<"Display low pin step sequence:"<<endl;
	    stepperDriver.displayPinStateSequence(pinStateSequence);
	    cout<<"Done!"<<endl;
	}

    stepperDriver.setPins({5,6,7});
    pinStateSequence={};
    pinStateSequence.push_back(stepperDriver.getCurrentPinState());
    stepperDriver.moveStep("CCW",pinStateSequence);
   
    if(DISPLAY)
	{
	    cout<<"Display high pin step sequence:"<<endl;
	    stepperDriver.displayPinStateSequence(pinStateSequence);
	    cout<<"Done!"<<endl;
	}

    testSequence={64+32+16,64+32,64,0,64,16+64};
    assert(pinStateSequence==testSequence);
    
    vector<std::string> y(10);
    for(int i=0;i<10;i++)
	{
	    y[i]="CW";
	}
    pinStateSequence={};
    stepperDriver.moveSteps(&y,pinStateSequence);
    if(DISPLAY)
	stepperDriver.displayPinStateSequence(pinStateSequence);

    testSequence={64,96,32,96,32,96,32,96,32,96,32,96,32,96,32,96,32,96,32,96,32,96,112};
    assert(pinStateSequence==testSequence);
    stepperDriver.setDefaultDirection("CCW");

    stepperDriver.setPins({2,3,4});
    cout<<"End with testStepperDriver()"<<endl;
    return stepperDriver;
}


std::vector<std::shared_ptr<BaseJoint>> testJoint(StepperDriver& i_driver)
{
    cout<<"start with Joint test"<<endl;
    
    float position,perStep;
    position=0.0;
    perStep=0.01;
	
    RotationalJoint<StepperDriver> joint(position,
					 perStep,
					 i_driver);
    Point2D point(0,50);
    joint.predictSteps(point,"CCW",100);
    if(DISPLAY)
	cout<<"Position of resulting point:"<<point.x<<", "<<point.y<<endl;

    assert(verifyPoint(Point2D(-0.87262, 49.9924),point));
    
    StepperDriver stepperDriver(5,6,7);

    DirectionConversionMap map{{"IN","CCW"},{"OUT","CW"}};
    TranslationalJoint<StepperDriver> joint2(position,
					     perStep,
					     map,
					     stepperDriver);


    joint2.predictSteps(point,"IN",100);
    if(DISPLAY)
    cout<<"Position of resulting point:"<<point.x<<", "<<point.y<<endl;


    assert(verifyPoint(Point2D(-0.855165, 48.9925),point));


    JointPointer jointPointer=joint.clone();

    assert(jointPointer->getMovementPerStep()==joint.getMovementPerStep());
    assert(jointPointer->getPosition()==joint.getPosition());
    assert(jointPointer->convertDirection("CCW")==joint.convertDirection("CCW"));
    assert(jointPointer->getMotor()->getPins()==joint.getMotor()->getPins());
    assert(joint2.clone()->getMotor()->getPins()==joint2.getMotor()->getPins());
    assert(joint2.clone()->getMotor()->getPins()!=jointPointer->getMotor()->getPins());
    
    std::vector<std::shared_ptr<BaseJoint>> jointVector{joint.clone(),joint2.clone()};
    
    cout<<"Done with Joint test"<<endl;
    return jointVector;
}

JointController testJointController(std::vector<std::shared_ptr<BaseJoint>> i_jointVector)
{
    cout<<"Start with JointController test"<<endl;
    JointController jointController;
    for(std::vector<std::shared_ptr<BaseJoint>>::iterator itr=i_jointVector.begin();
	itr!=i_jointVector.end();
	itr++)
	{
	    jointController.addJoint((*itr));
	}

    JointPointerVector rotationalJoints=jointController.getJoints(Rotational);
    assert(i_jointVector[0]->getMotor()->getPins()==rotationalJoints[0]->getMotor()->getPins());

    assert(jointController.getJoint(Translational)->getMovementPerStep()>0.0099999);
    assert(jointController.getJoint(Translational)->getMovementPerStep()<0.0100001);

    bool hasThrown(false);
    try 
	{
	    jointController.getJoint(Translational)->convertDirection("CCW");	
	} 
    catch (int e)
	{
	    hasThrown=true;	
	}
    
    //ment to throw an exception!
    assert(hasThrown==true);
	
    assert(jointController.getJoints(Translational)[0]->convertDirection("IN")=="CCW");

    JointPointer rotationalJoint=jointController.getJoints(Rotational)[0];
    JointPointer translationalJoint=jointController.getJoints(Translational)[0];

    //reset the pins of the joint
    rotationalJoint->getMotor()->setPins({2,3,4});
    translationalJoint->getMotor()->setPins({5,6,7});
    
    
    cout<<"start steps!"<<endl;
    jointController.moveStep(rotationalJoints[0],"CCW",true);
    PinStateSequence testSequence{138,130,138,140};
    assert(validateSequence(*(jointController.getPinStateSequence()),testSequence));
    jointController.resetPinStateSequence();
    testSequence={126};
    assert(validateSequence(*(jointController.getPinStateSequence()),testSequence));
    jointController.moveStep(rotationalJoints[0],"CW",true);
    testSequence={138,134,126,134,136};
    assert(validateSequence(*(jointController.getPinStateSequence()),testSequence));

    jointController.resetPinStateSequence();
    cout<<"first one is done!"<<endl;
    jointController.moveSteps(translationalJoint,"IN",10);

    jointController.moveStep(rotationalJoint,"CCW",true);
    jointController.getJoint(Translational)->getMotor()->displayPinStateSequence(*jointController.getPinStateSequence());
    jointController.getJoint(Rotational)->getMotor()->displayPinStateSequence(*jointController.getPinStateSequence());
    cout<<"moved steps!"<<endl;
    
    if(DISPLAY)
	{
	    for(PinStateSequence::iterator itr=(*jointController.getPinStateSequence()).begin();
		itr!=(*jointController.getPinStateSequence()).end();
		itr++)
		{
		    
		    std::cout<<"============================="<<std::endl;
		    cout<<"Rotational joint: "<<std::endl;
		    rotationalJoint->getMotor()->displayPinState(*itr);
		    cout<<"Translational Joint: "<<std::endl;
		    translationalJoint->getMotor()->displayPinState(*itr);
		    std::cout<<"============================="<<std::endl;
		}
	}
    
    cout<<"Done with JointController test"<<endl;
    return jointController;
}


void testCopyOfJointController(JointController& i_jointController)
{
    assert(i_jointController.getJoints(Rotational).size()==1);
    assert(i_jointController.getJoints(Translational).size()==1);

    assert(i_jointController.getJoint(Translational)->getMotor()->getPins()[0]==5);
    assert(i_jointController.getJoint(Rotational)->getMotor()->getPins()[0]==2);
}


void testTraceCalculation(JointController& i_jointController)
{
    Trace trace;
    Point2D startPoint(0,50);
    trace.setStartPosition(startPoint);
    trace.setEndPosition(Point2D(100,50));

    i_jointController.getJoint(Translational)->getMotor()->setHoldMotor(true);
    i_jointController.getJoint(Rotational)->getMotor()->setHoldMotor(true);
    
    LineTraceCalculator lineTraceCalculator(&i_jointController);
    lineTraceCalculator.calculateTrace(&trace,startPoint);
    cout<<i_jointController.getPinStateSequence()->size();
    cout<<i_jointController.getPinStateSequence()->size();
    assert(12363==i_jointController.getPinStateSequence()->size());

    JointPointer rotationalJoint=i_jointController.getJoints(Rotational)[0];
    JointPointer translationalJoint=i_jointController.getJoints(Translational)[0];

    assert((rotationalJoint->getMotor()->getPins()==std::vector<int>{2,3,4}));
    assert(translationalJoint->getDirectionConversionMap()->find("IN")->second=="CCW");
    
    for(PinStateSequence::iterator itr=(*i_jointController.getPinStateSequence()).begin()+200;
	itr!=(*i_jointController.getPinStateSequence()).begin()+250;
	itr++)
	{
	    std::cout<<"============================="<<std::endl;
	    std::cout<<"integer value: "<<*itr<<std::endl;
	    cout<<"Rotational joint: "<<std::endl;
	    rotationalJoint->getMotor()->displayPinState(*itr);
	    cout<<"Translational Joint: "<<std::endl;
	    translationalJoint->getMotor()->displayPinState(*itr);
	    std::cout<<"============================="<<std::endl;
	}
	
}

int main(){
    //  try{
    StepperDriver driver=testStepperDriver();
    std::vector<shared_ptr<BaseJoint>> x=testJoint(driver);
    JointController jointController=testJointController(x);

    testCopyOfJointController(jointController);
	
    //    testTraceCalculation(jointController);
}
