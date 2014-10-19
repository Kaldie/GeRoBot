#include <algorithm>
#include <stdlib.h>
#include <BaseJoint.h>
#include <JointController.h>



const bool JointController::validateJoint(const JointPointer& i_baseJoint) const{
  //pins should not be other then 2 t/m 7
  std::vector<int> pins=i_baseJoint->getMotor()->getPins();
  std::sort(pins.begin(),pins.end());
  
  int prevsPin=pins[0];
  for(std::vector<int>::const_iterator itr=pins.begin();
      itr!=pins.end();
      itr++)
    {
      //Pin should be between 2 and 7...make sense for arduino only!
      if(((*itr)>7) |((*itr) <2))
	{
	  LOG_INFO("Pin: "<<(*itr)<<" is not within range!!");
	  return false;
	}
      
      if(prevsPin-(*itr)>1){
	LOG_INFO(prevsPin);
	LOG_INFO(*itr);
	return false;
      }
      
      prevsPin=(*itr);
    }
  return true;
}

const bool JointController::validateJointVector(const JointPointerVector& i_jointVector) const
{
    //determines if the vector is able to grow into a good JointVector
    for(JointPointerVector::const_iterator itr=i_jointVector.begin();itr!=i_jointVector.end();itr++)
	{
	    //All joints should be valid
	    if(!validateJoint(*itr))
		return false;

	    //Number of joints should not be other then 2....or max 2...
	    if(i_jointVector.size()>2)
		return false;

	    //If the number of joints is 1 we're done!
	    else if(i_jointVector.size()==1)
		return true;
	    
	    //Movement type of the joints should not be equal
	    if(i_jointVector[0]->getMovementType()==i_jointVector[1]->getMovementType())
		return false;
	}	    
    return true;
}

void JointController::addJoint(const JointPointer& i_joint)
		
{
    /*
      This gonna be fun!!
      We want to store the joints in a vector, however they are defined as base joints
      If we want to use the derived stuff, we need to:
      clone it
      put the clone on the heap 
      put a pointer in the m_jointPointerVector
    */
    if(!hasJoint(i_joint))
	{
	  m_jointPointerVector.push_back(i_joint);

	    LOG_INFO("Current size: "<<m_jointPointerVector.size());

	    if(!validateJointVector(m_jointPointerVector))
		  LOG_ERROR("Joint is not valid!");
	}
    LOG_DEBUG("Resetting pin state sequence");
    resetPinStateSequence();
}


JointPointerVector JointController::getJoints(const MovementType &i_movementType)
{
    JointPointerVector requiredJoints;
    for(JointPointerVector::iterator itr=m_jointPointerVector.begin();
	itr!=m_jointPointerVector.end();
	itr++)
	{
	    if ((*itr)->getMovementType() == i_movementType)
		{
		    requiredJoints.push_back((*itr));
		}
	}

    for(JointPointerVector::iterator itr=requiredJoints.begin();
	itr!=requiredJoints.end();
	itr++)
	{
	    LOG_INFO("movement after insert into vector: "<<(*itr)->getMovementPerStep());
	}

    
     return requiredJoints;
}


JointPointer& JointController::getJoint(const MovementType &i_movementType)
{
    bool found;
    int index=0;
    int i=0;
    for(JointPointerVector::iterator itr=m_jointPointerVector.begin();
	itr!=m_jointPointerVector.end();
	itr++,i++)
	{
	    if ((*itr)->getMovementType() == i_movementType)
		{
		    index=i;
		    found=true;
		}
	}
    if(found)
	return m_jointPointerVector[index];
    else
	{
	    std::cerr<<"No joint found!"<<std::endl;
	    throw 101;
	}
}

void JointController::resetPinStateSequence()
{
    PinStateSequence emptyPinStateSequence;
    PinState cleanPinState;
    for(JointPointerVector::iterator itr=m_jointPointerVector.begin();
	itr!=m_jointPointerVector.end();
	itr++)
	{
	    cleanPinState.update((*itr)->getMotor()->getCurrentPinState());
	    cleanPinState.setPinsToDefault();
	    LOG_INFO("Pins: ");
	    
	    #ifndef NDEBUG
	    const PinVector pins=cleanPinState.getPinVector();	\
	    for(PinVector::const_iterator itr=pins.begin();	\
		itr!=pins.end();				\
		itr++)						\
		LOG_INFO(*itr<<", ");				
	    #endif
	}
    emptyPinStateSequence.push_back(cleanPinState);
    m_pinStateSequence.swap(emptyPinStateSequence);
}


const bool JointController::hasJoint(const JointPointer& i_jointPointer) const
{
    for(JointPointerVector::const_iterator itr=m_jointPointerVector.begin();
	itr!=m_jointPointerVector.end();
	itr++)
	{
	    if (i_jointPointer==(*itr))
		return true;
	}
	
    return false;
}

void JointController::addEmptyPinStatesToSequence(const int& i_numberOfStates)
{
    LOG_INFO("Adding "<<i_numberOfStates<<" pin states");
    //    (*(m_pinStateSequence.end()-1)).displayPinState();
    const PinState lastState=*(m_pinStateSequence.end()-1);
    for(int i=i_numberOfStates;i>0;i--)
	    m_pinStateSequence.insert(m_pinStateSequence.end(),lastState);
}


void JointController::appendPinStateSequence(const PinStateSequence& i_newPinStateSequence,
					     const bool& i_merge)
{   

    unsigned int numberOfNewPinStates=i_newPinStateSequence.size();
    unsigned int currentSize=m_pinStateSequence.size();
    unsigned int mergePosition;

    LOG_INFO("Number Of New Pin States: "<<numberOfNewPinStates);
    
    if(i_merge)
	{
	    if(currentSize<numberOfNewPinStates)
		{
		    addEmptyPinStatesToSequence(numberOfNewPinStates-currentSize);
		    currentSize=m_pinStateSequence.size();
		}
	    LOG_INFO("current size: "<<currentSize);
	    LOG_INFO("Number of new pin states: "<<numberOfNewPinStates);
	    mergePosition=currentSize-numberOfNewPinStates;
	}
    else
	{
	    mergePosition=currentSize;
	    addEmptyPinStatesToSequence(numberOfNewPinStates);
	}
    
    LOG_INFO("Merge position is: "<<mergePosition);
    PinStateSequence::iterator currentPinStateItr=m_pinStateSequence.begin()+mergePosition;
    PinStateSequence::const_iterator newPinStateItr=i_newPinStateSequence.begin();

    for(;
	newPinStateItr!=i_newPinStateSequence.end();
	newPinStateItr++,currentPinStateItr++)
	currentPinStateItr->update(*newPinStateItr);

}


void JointController::addStepToPrevious(const JointPointer& i_jointPointer,
					const std::string& i_motorDirection)
{
    PinStateSequence tmpPinStateSequence;
    i_jointPointer->getMotor()->moveStep(i_motorDirection,
					 tmpPinStateSequence);

    appendPinStateSequence(tmpPinStateSequence,
			   true);

    //    i_jointPointer->getMotor().displayPinStateSequence(tmpPinStateSequence);
}


void JointController::moveStep(JointPointer& i_jointPointer,
			       const std::string& i_directionString,
			       const bool& i_appendToPreviousStep)
{
    if(!hasJoint(i_jointPointer))
	{
	    std::cerr<<"Joint not avalaible in this controller!"<<std::endl;
	    throw 101;
	}
    
    std::string motorDirection=i_jointPointer->convertDirection(i_directionString);
        
    if(i_appendToPreviousStep)
	{
	    addStepToPrevious(i_jointPointer, motorDirection);
	}
    else
	{
	    PinStateSequence pinStateSequence;
	    i_jointPointer->getMotor()->moveStep(motorDirection,pinStateSequence);
	    appendPinStateSequence(pinStateSequence,false);
	}
}


void JointController::moveSteps(JointPointer& i_jointPointer,
				const std::string& i_directionString,  
				const int& i_numberOfSteps)
{
    //Check if the joint controller has this joint
    if(!hasJoint(i_jointPointer))
	{
	    std::cerr<<"Joint not avalaible in this controller!"<<std::endl;
	    throw 101;
	}


    std::string motorDirection=i_jointPointer->convertDirection(i_directionString);
    std::vector<std::string> directionVector(i_numberOfSteps,motorDirection);
    LOG_INFO(i_jointPointer->getMotor()->getPins()[2]);

    PinStateSequence pinStateSequence;
    i_jointPointer->getMotor()->moveSteps(&directionVector, pinStateSequence);
    
    appendPinStateSequence(pinStateSequence,false);
}

//  Brief: Actuate the robot from the given pin state sequence
void JointController::actuate()
{
    std::vector<int> buffer;
    for(PinStateSequence::iterator itr=m_pinStateSequence.begin();
	itr!=m_pinStateSequence.end();
	itr++)
	buffer.push_back(itr->getNumericValue());
    m_actuator.setReducedSpeed(true);
    m_actuator.setBuffer(buffer);
    m_actuator.actuate();

}




/*
void JointController::getUndefinedPins(PinState& o_undefinedPinState,
				       const JointPointer& i_jointPointer) const
{
    PinState lastPinState=*(m_pinStateSequence.end()-1);
    PinVector jointPins=i_jointPointer->getMotor()->getPins();
    
    for(PinVector::const_iterator pinIterator=lastPinState.getPinVector().begin();
	pinIterator!=lastPinState.getPinVector().end();
	pinIterator++)
	{
	    //if the pin is not defined in the joint, update the o_undefinedPinState to have it
	    if(std::find(jointPins.begin(), 
			 jointPins.end(),
			 *pinIterator)==jointPins.end())
		o_undefinedPinState.update(*pinIterator,
					   lastPinState.getPinState(*pinIterator));
	} 
}
*/

JointController::JointController()
    :m_actuator(ArduinoMotorDriver("/dev/ttyUSB*"))
{}

JointController::~JointController()
{}
