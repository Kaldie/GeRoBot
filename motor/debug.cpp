/*Stepper driver class
  Create the control the driver

  Dep WiringPi
*/
#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
#include <memory>

#include <macroHeader.h>
#include <PinState.h>
#include "BaseMotor.h"
//#include "StepperMotor.h"
#include "StepperDriver.h"

#include "BaseJoint.h"
#include "RotationalJoint.h"
#include "TranslationalJoint.h"

//#include "JointController.h"

#include <Trace.h>
#include <LineTraceCalculator.h>
using namespace std;

bool DISPLAY(true);


bool verifyPoint(const Point2D& i_refPoint,
		 const Point2D& i_measPoint)
{
    float tolerance=0.0001;


    if(DISPLAY)
	{
	    cout<<"Position of measurement point:"<<i_measPoint.x <<", "<<i_measPoint.y<<endl;
	    cout<<"Position of resulting point:"<<i_refPoint.x<<", "<<i_refPoint.y<<endl;
	}

	
    if(i_refPoint.x>(i_measPoint.x+tolerance)) return false;
    if(i_refPoint.x<(i_measPoint.x-tolerance)) return false;

    if(i_refPoint.y>(i_measPoint.y+tolerance)) return false;
    if(i_refPoint.y<(i_measPoint.y-tolerance)) return false;
    return true;
}


bool validateSequence(const PinStateSequence& i_toTest,
		      const vector<int>& i_benchmark)
{
    bool hasPassed=true;

    if(i_toTest.size()!=i_benchmark.size())
	{
	    if(DISPLAY)
		{
		    cout<<"Size test= "<<i_toTest.size()<<endl;
		    cout<<"Size benchmark= "<<i_benchmark.size()<<endl;
		}
	    hasPassed=false;
	}
    
    
    if(DISPLAY)
	{
	    for(PinStateSequence::const_iterator itr=i_toTest.begin();
		itr!=i_toTest.end();
		itr++)
		cout<<itr->getNumericValue()<<", ";
	    cout<<endl;

	    for(std::vector<int>::const_iterator itr=i_benchmark.begin();
		itr!=i_benchmark.end();
		itr++)
		cout<<*itr<<", ";
	    cout<<endl;
	}
    
    PinStateSequence::const_iterator toTestIter=i_toTest.begin();
    std::vector<int>::const_iterator benchmarkIter=i_benchmark.begin();
    
    for(;
	toTestIter!=i_toTest.end();
	toTestIter++,benchmarkIter++)
	{
	    if(toTestIter->getNumericValue()!=*benchmarkIter)
		hasPassed=false;
	}

    return hasPassed;
}


void testPinState()
{
    PinState pinState;

    assert(pinState.getNumericValue()==0);

    std::vector<int> pins{1,2,3};
    pinState.setPins(pins);

    assert(pinState.getNumericValue()==14);
    pinState.setPinsToDefault();

    assert(pinState.getPinState(3)==1);
    //assert pin state map update
    pinState.update(1,0);
    pinState.update(2,0);
    pinState.update(3,0);

    assert(pinState.getNumericValue()==0);

    //assert pin update via pin number and value

    pinState.update(2,1);
    assert(pinState.getNumericValue()==4);
   
    //assert pin update via pin state and appending multiple pins with it
    PinState testState;

    testState.setPins(std::vector<int>{4,5,6});
    if(DISPLAY)
	testState.displayPinState();
    assert(testState.getNumericValue()==112);
    pinState.update(testState);

    assert(pinState.getNumericValue()==116);


}


StepperDriver testStepperDriver()
{
    if(DISPLAY)
	cout<<"Start with testStepperDriver()"<<endl;

    StepperDriver stepperDriver(vector<int>{2,3,4});

    if(DISPLAY)
	cout<<"Created stepperdriver"<<endl;

    PinState pinState=stepperDriver.getCurrentPinState();

    assert(pinState.getNumericValue()==28);
    pinState.update(2,0);
    assert(pinState.getNumericValue()==24);
    pinState.update(2,1);
    pinState.update(3,0);
    assert(pinState.getNumericValue()==20);
    
    stepperDriver.setCurrentPinState(pinState);


    PinStateSequence pinStateSequence;
    stepperDriver.setPins({2,3,4});
    pinStateSequence.push_back(stepperDriver.getCurrentPinState());
    stepperDriver.setEnable(true);
        
    stepperDriver.moveStep("CCW",pinStateSequence);

    vector<int> testSequence{28,16,0,16,20};

    
    if(DISPLAY)
	{
	    cout<<"Display low pin step sequence:"<<endl;
	    stepperDriver.displayPinStateSequence(pinStateSequence);
	    cout<<"Done!"<<endl;
	}

    stepperDriver.setPins({5,6,7});
    pinStateSequence={};
    pinStateSequence.push_back(stepperDriver.getCurrentPinState());
    stepperDriver.moveStep("CW",pinStateSequence);

    testSequence={224, 192, 64, 192, 224};
    assert(validateSequence(pinStateSequence,testSequence));   


    if(DISPLAY)
	{
	    cout<<"Display high pin step sequence:"<<endl;
	    stepperDriver.displayPinStateSequence(pinStateSequence);
	    cout<<"Done!"<<endl;
	}

    testSequence={224, 192, 64, 192, 224};
    assert(validateSequence(pinStateSequence,testSequence));
    
    vector<std::string> y(10);
    for(int i=0;i<10;i++)
	{
	    y[i]="CW";
	}
    pinStateSequence={};
    stepperDriver.moveSteps(&y,pinStateSequence);
    if(DISPLAY)
	stepperDriver.displayPinStateSequence(pinStateSequence);

    testSequence={192, 64, 192, 64, 192, 64, 192, 64, 192, 64, 192, 64, 192, 64, 192, 64, 192, 64, 192, 64, 192, 224};
    validateSequence(pinStateSequence,testSequence);
    stepperDriver.setDefaultDirection("CCW");
    stepperDriver.setPins({2,3,4});

    if(DISPLAY)
	{
	    stepperDriver.displayPinState();
	    cout<<"End with testStepperDriver()"<<endl;
	}
    
    return stepperDriver;
}


std::vector<std::shared_ptr<BaseJoint>> testJoint(StepperDriver& i_driver)
{
    if(DISPLAY)
	cout<<"start with Joint test"<<endl;
    
    float position,perStep;
    position=0;
    perStep=0.1;

    RotationalJoint<StepperDriver> joint(position, perStep, i_driver);

    joint.setPosition(90);
    std::vector<float> range={0.0,180.0};
    joint.setRange(range);
    Point2D point(0,50);

    if(DISPLAY)
	std::cout<<(joint.getRange()[0])<<", "<<
	    (joint.getRange()[1])<<std::endl;

    joint.predictSteps(point,"CCW",100);
    
    assert(verifyPoint(Point2D(-8.68241, 49.2404),point));
    
    StepperDriver stepperDriver({5,6,7});
    
    DirectionConversionMap map{{"IN","CCW"},{"OUT","CW"}};
    
    TranslationalJoint<StepperDriver> joint2(position,
					     perStep,
					     map,
					     stepperDriver);
    range[0]=50.0;
    range[1]=200.0;
    joint2.setRange(range);

    if(DISPLAY)
	std::cout<<(joint2.getRange()[0])<<", "<<
	    (joint2.getRange()[1])<<std::endl;
    
    joint2.predictSteps(point,"IN",100);

    assert(verifyPoint(Point2D(-6.94593, 39.3923),point));


    JointPointer jointPointer=joint.clone();

    assert(jointPointer->getMovementPerStep()==joint.getMovementPerStep());
    assert(jointPointer->getPosition()==joint.getPosition());
    assert(jointPointer->convertDirection("CCW")==joint.convertDirection("CCW"));

    assert(joint2.clone()->getMotor()->getPins().size()!=0);
    assert(jointPointer->getMotor()->getPins()==joint.getMotor()->getPins());

    assert(joint2.clone()->getMotor()->getPins()==joint2.getMotor()->getPins());
    assert(joint2.clone()->getMotor()->getPins()!=jointPointer->getMotor()->getPins());
    
     std::vector<std::shared_ptr<BaseJoint>> jointVector{joint.clone(),joint2.clone()};

     if(DISPLAY)
	 cout<<"Done with Joint test"<<endl;

     return jointVector;
}


JointController testJointController(std::vector<std::shared_ptr<BaseJoint>> i_jointVector)
{
    if(DISPLAY)
	cout<<"Start with JointController test"<<endl;
    JointController jointController;
    

    for(std::vector<std::shared_ptr<BaseJoint>>::iterator itr=i_jointVector.begin();
	itr!=i_jointVector.end();
	itr++)
	{
	    if(DISPLAY)
		{
		    PinVector pins=(*itr)->getMotor()->getPins();
		    for(PinVector::const_iterator pinItr=pins.begin();
			pinItr!=pins.end();
			pinItr++)
			cout<<*pinItr<<", ";
		    cout<<endl;
		}
	    jointController.addJoint((*itr));

	}
    
    if(DISPLAY)
	cout<<"Done with creation of the joint controller!"<<endl;

    JointPointerVector rotationalJoints=jointController.getJoints(Rotational);

    if(DISPLAY)
	std::cout<<(jointController.getJoint(Rotational)->getRange()[0])<<", "<<
	    (jointController.getJoint(Rotational)->getRange()[1])<<std::endl;


    assert(i_jointVector[0]->getMotor()->getPins()==rotationalJoints[0]->getMotor()->getPins());

    assert(jointController.getJoint(Translational)->getMovementPerStep()>0.099999);
    assert(jointController.getJoint(Translational)->getMovementPerStep()<0.100001);

    bool hasThrown(false);
    try 
	{
	    jointController.getJoint(Translational)->convertDirection("CCW");	
	} 
    catch (std::runtime_error)
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

    rotationalJoint->setPosition(90.0);
    translationalJoint->setPosition(50);

    jointController.resetPinStateSequence();

    if(DISPLAY)
	cout<<"start steps!"<<endl;

    jointController.moveStep(rotationalJoints[0],"CCW",true);
    
    std::vector<int> testSequence{248,232,248,252};
    assert(validateSequence(jointController.getPinStateSequence(),testSequence));

    if(DISPLAY)
	jointController.getJoint(Rotational)->getMotor()->displayPinStateSequence(jointController.getPinStateSequence());

    jointController.resetPinStateSequence();
    testSequence={252};
    assert(validateSequence(jointController.getPinStateSequence(),testSequence));

    jointController.moveStep(rotationalJoints[0],"CW",false);
    
    if(DISPLAY)
	jointController.getJoint(Rotational)->getMotor()->displayPinStateSequence(jointController.getPinStateSequence());
	
    testSequence={252, 240, 224, 240, 244};
    assert(validateSequence(jointController.getPinStateSequence(),testSequence));

    if(DISPLAY)
	{
	    jointController.getJoint(Rotational)->getMotor()->displayPinStateSequence(jointController.getPinStateSequence());
	    cout<<"first one is done!"<<endl<<endl<<endl;
	}
    jointController.resetPinStateSequence();

    jointController.moveSteps(translationalJoint,"OUT",5);
    testSequence={252, 220, 92, 220, 92, 220, 92, 220, 92, 220, 92, 220, 252};
    assert(validateSequence(jointController.getPinStateSequence(),testSequence));

    jointController.moveStep(rotationalJoint,"CW",false);
    testSequence={252, 220, 92, 220, 92, 220, 92, 220, 92, 220, 92, 220, 252, 240, 224, 240, 244};
    
    jointController.getJoint(Rotational)->getMotor()->displayPinStateSequence(jointController.getPinStateSequence());

    PinStateSequence pinStateSequence=jointController.getPinStateSequence();
    assert(validateSequence(jointController.getPinStateSequence(),testSequence));

    jointController.moveSteps(translationalJoint,"IN",5);

    testSequence={252, 220, 92, 220, 92, 220, 92,
		  220, 92, 220, 92, 220, 252, 240,
		  224, 240, 244, 212, 148, 20, 148,
		  20, 148, 20, 148, 20, 148, 20,
		  148, 180};

    assert(validateSequence(jointController.getPinStateSequence(),testSequence));


    if(DISPLAY)
    {
	jointController.getJoint(Translational)->getMotor()->displayPinStateSequence(jointController.getPinStateSequence());
	jointController.getJoint(Rotational)->getMotor()->displayPinStateSequence(jointController.getPinStateSequence());
	
	cout<<"moved steps!"<<endl;
	cout<<"Done with JointController test"<<endl;
    }

    jointController.resetPinStateSequence();
    return jointController;
}


void testTraceCalculation(JointController& i_jointController)
{
    Trace trace;
    Point2D startPoint(0,50);
    trace.setStartPosition(startPoint);
    trace.setEndPosition(Point2D(-1500,50));
    
    i_jointController.getJoint(Translational)->setMovementPerStep(0.01);
    i_jointController.getJoint(Rotational)->setMovementPerStep(0.01);

    trace.setRotationTolerance(i_jointController.getJoint(Rotational)->getMovementPerStep()*1.5);
    trace.setTranslationTolerance(i_jointController.getJoint(Translational)->getMovementPerStep()*1.5);

    i_jointController.getJoint(Translational)->getMotor()->setHoldMotor(true);
    i_jointController.getJoint(Rotational)->getMotor()->setHoldMotor(true);
    
    LineTraceCalculator lineTraceCalculator(&i_jointController);
    lineTraceCalculator.setWriteLog(true);
    lineTraceCalculator.calculateTrace(&trace,startPoint);
    
    cout<<i_jointController.getPinStateSequence().size();
    assert(297458==i_jointController.getPinStateSequence().size());

    JointPointer rotationalJoint=i_jointController.getJoints(Rotational)[0];
    JointPointer translationalJoint=i_jointController.getJoints(Translational)[0];

    assert((rotationalJoint->getMotor()->getPins()==std::vector<int>{2,3,4}));
    assert(translationalJoint->getDirectionConversionMap().find("IN")->second=="CCW");
    
    //    i_jointController.actuate();
}


int main()
{
    //  try{
    for(int i=0;
	i<1;
	i++)
	{
	    testPinState();
	    StepperDriver driver=testStepperDriver();
	    std::vector<std::shared_ptr<BaseJoint>> jointVector=testJoint(driver);
	    JointController jointController=testJointController(jointVector);
	    testTraceCalculation(jointController);
	}
}
