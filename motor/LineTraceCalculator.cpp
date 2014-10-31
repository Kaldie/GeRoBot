#include <cmath>
#include <algorithm>
#include <vector>
#include <Vector2D.h>
#include <Point2D.h>
#include <BaseJoint.h>
#include <JointController.h>
#include <LineTraceCalculator.h>


LineTraceCalculator::LineTraceCalculator():
    BaseTraceCalculator(),
    m_hasRotated(true),
    m_hasTranslated(true)
{}


LineTraceCalculator::LineTraceCalculator(JointController* i_jointController):
    BaseTraceCalculator(i_jointController),
    m_hasRotated(true),
    m_hasTranslated(true)
{}


LineTraceCalculator::LineTraceCalculator(JointController* i_jointController,
					 const float& i_tolerance):
    BaseTraceCalculator(i_jointController,
			i_tolerance),
    m_hasRotated(true),
    m_hasTranslated(true)
{}

  
//assign operator
LineTraceCalculator::LineTraceCalculator(const LineTraceCalculator& obj)
{
    LOG_INFO("Copy constructor");

    this->setJointController(obj.getJointController());
    this->setTolerance(obj.getTolerance());
}


void LineTraceCalculator::calculateTrace(const Trace* i_trace,
					 Point2D& i_point)
{
    //Setting the capacity of the vector to hold all the data
    std::vector<int> numberOfSteps=getNumberOfSteps(*i_trace,i_point);
    std::vector<int>::iterator maximumNumberOfSteps=std::max_element(numberOfSteps.begin(),numberOfSteps.end());
    PinStateSequence pinStateSequencePointer=getJointController()->getPinStateSequence();
    LOG_INFO("Reserving: "<<pinStateSequencePointer.size()+*maximumNumberOfSteps);
    
    pinStateSequencePointer.reserve(pinStateSequencePointer.size()+(*maximumNumberOfSteps)+5);
    
    int i=0;
    bool hasStepped(true);
    do
	{
	    hasStepped=calculateStep(i_trace,i_point);
	    i++;
	}while(hasStepped);// and i<1000);
}


bool LineTraceCalculator::calculateStep(const Trace* i_trace,
					Point2D& i_point2D) const
{
    //keeping track if the robot is orded to translate or rotated
    bool hasStepped=false;
    //should rotate?
    if(shouldRotate(*i_trace,i_point2D))
	{
	    //add rotation to the delay list and correct translations
	    prepareRotation(i_trace,
			    i_point2D);
	    hasStepped=true;
	}
    else if(shouldTranslate(*i_trace,i_point2D))
	{
	    prepareTranslation(i_trace,i_point2D);
	    hasStepped=true;
	}
    return hasStepped;
}


void LineTraceCalculator::prepareRotation(const Trace* i_trace,
					  Point2D& i_point2D)const
{
    //Rotating
    //    m_hasRotated=true;
    //Rotation direction
    std::string direction=i_trace->getRotationDirectionToEndPoint(i_point2D);
    
    //See where we end up after rotation
    getJointController()->getJoint(Rotational)->predictStep(i_point2D,direction);

    if(getWriteLog())
	writeToStepLog(direction,1);

    //user needs to know :)
    LOG_INFO("Rotating: "<<direction);

    //Correct the rotation
    bool hasCorrectionSteps=correctRotation(i_trace,
					    i_point2D);

    getJointController()->moveStep(getJointController()->getJoint(Rotational),
				   direction,
				   hasCorrectionSteps);
}

void LineTraceCalculator::prepareTranslation(const Trace* i_trace,
					     Point2D& i_point2D) const
{
    //    m_hasTranslated=true;
    //identify the direction to move to
    std::string direction=i_trace->getTranslationDirectionToEndPoint(i_point2D);
  
    //Predict the step
    getJointController()->getJoint(Translational)->predictStep(i_point2D,direction);
    
    if(getWriteLog())
	writeToStepLog(direction,1);

    LOG_INFO("Translating: "<<direction);

    //Correct the rotation, if nessesary!
    bool hasCorrectionSteps=correctTranslation(i_trace,i_point2D);

    getJointController()->moveStep(getJointController()->getJoint(Translational),
				                           direction,
				                           hasCorrectionSteps);
}


const bool LineTraceCalculator::correctRotation(const Trace* i_trace,
						Point2D& i_point2D) const
{
  float jointPositionDifference;
  const Point2D* destinationPoint;

  Point2D intersectingPoint=i_trace->getTraceLine().getIntersectingPoint(i_point2D);
    
  /* The distance to the enpoint after the correction is applied*/
  double distenceEndPointIntersectingPoint=Magnitude(intersectingPoint-i_trace->getEndPosition());
    
  /* The distance traveled in this correction*/
  double distanceBeginPointIntersectingPoint=Magnitude(intersectingPoint-i_point2D);
    
  if(distenceEndPointIntersectingPoint<distanceBeginPointIntersectingPoint){
    /*if the distance needed in the next step is smaller then the distance we travel in this correction step
      Don't overshoot
    */
    jointPositionDifference=Magnitude(i_trace->getEndPosition()-i_point2D);
    destinationPoint=&(i_trace->getEndPosition());
  }
  else{
    /*
      Otherwise we assume that there is enough distance, 
      so overshooting the correction decrease the absolute distance between the wanted and final result
      This is done by spreading out the error over both the positive as negative side of the line
    */
    jointPositionDifference=distanceBeginPointIntersectingPoint;
    destinationPoint=&intersectingPoint;
    jointPositionDifference*=2.0;
  }

    LOG_INFO("Destination point is: "<<destinationPoint->x<<", "<<destinationPoint->y);

    int numberOfSteps=std::floor((jointPositionDifference)/getJointController()->getJoint(Translational)->getMovementPerStep());

    
  if(numberOfSteps>0){
    std::string translationDirection=i_trace->getTranslationDirection(i_point2D, *destinationPoint);
	
    getJointController()->getJoint(Translational)->predictSteps(i_point2D,translationDirection,numberOfSteps);
	
    getJointController()->moveSteps(getJointController()->getJoint(Translational),
                                    translationDirection,
                                    numberOfSteps);
    
    LOG_INFO("Number of correction steps: "<<numberOfSteps<<" in the "<<translationDirection<<" direction");
  
	
    if(getWriteLog())
	  
      writeToStepLog(translationDirection,numberOfSteps);
	
	
    return true;
  }
  
  else  
    return false;
}


const bool LineTraceCalculator::correctTranslation(const Trace* i_trace,
						   Point2D& i_point2D) const {
    float currentRotation=i_point2D.getAlpha();

    Point2D destinationPoint=i_trace->getTraceLine().getIntersectingPoint(i_point2D);
    
    if (destinationPoint!=Point2D(0,0))
	return false;
    
    LOG_INFO("Distination point: "<<destinationPoint.x<<", "<<destinationPoint.y); 
    LOG_INFO("Current robot rotation: "<<currentRotation*(180.0/PI));
    LOG_INFO("DistinationPoint rotation: "<<destinationPoint.getAlpha()*(180.0/PI));
    float distinationPointRotation=destinationPoint.getAlpha();
    
    float jointAngleDifference=currentRotation-distinationPointRotation;
    
    LOG_INFO("Joint angle difference: "<<jointAngleDifference*(PI/180));

    float multiplicationFactor;
    if(destinationPoint.getAlpha()>i_trace->getEndPosition().getAlpha())
	multiplicationFactor=1.0;
    else
	multiplicationFactor=2.0;
    
    int numberOfSteps=std::floor((std::abs(jointAngleDifference)*multiplicationFactor)/
				 getJointController()->getJoint(Rotational)->getMovementPerStep()*(PI/180.0));
    
    if(numberOfSteps>0)
	{
	    std::string rotationDirection=i_trace->getRotationDirection(i_point2D,destinationPoint);
	    
	    getJointController()->getJoint(Rotational)->predictSteps(i_point2D,rotationDirection,numberOfSteps);
	    
	    getJointController()->moveSteps(getJointController()->getJoint(Rotational),
					    rotationDirection,
					    numberOfSteps);
	    
	    if(getWriteLog())
        writeToStepLog(rotationDirection,numberOfSteps);

      return true;
	}
  return false;
}

bool LineTraceCalculator::hasStepped()
{
    return (m_hasTranslated or m_hasRotated);
}

