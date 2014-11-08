#include <macroHeader.h>
#include <RotationTraceCalculator.h>
#include <LineTraceCalculator.h>
#include <RotationTrace.h>

RotationTraceCalculator::RotationTraceCalculator():
	LineTraceCalculator()
{}
	
RotationTraceCalculator::RotationTraceCalculator(JointController* i_jointControllerPointer):
		LineTraceCalculator(i_jointControllerPointer)
{}

RotationTraceCalculator::RotationTraceCalculator(JointController* i_jointControllerPointer,
																								 const float& i_tolerance):
	LineTraceCalculator(i_jointControllerPointer,i_tolerance)
{}

void RotationTraceCalculator::calculateTrace(const RotationTrace* i_rotationTrace,
																						 Point2D& i_currentPosition)
{
	if(i_rotationTrace->getTraceType()!=Curve)
		LOG_ERROR("Rotational trace calculator only works on rotational traces!"<<
							"\nvalue: "<<i_rotationTrace->getTraceType()<<" curve value: "<<Curve);
	
	std::vector<RotationTrace> traces=i_rotationTrace->getNecessaryTraces();
	LOG_DEBUG("Number of traces: "<<traces.size());

	for(std::vector<RotationTrace>::const_iterator itr=traces.begin();
			itr!=traces.end();
			itr++)
		{
			LOG_DEBUG("Arc endPoint x, y: "<<itr->getArc().getEndPoint().x<<", "<<itr->getArc().getEndPoint().y);
			LineTraceCalculator::calculateTrace(&(*itr),i_currentPosition);
		}
}
	
