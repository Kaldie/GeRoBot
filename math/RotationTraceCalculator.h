#ifndef RotationTraceCalculator_H
#define RotationTraceCalculator_H

#include <LineTraceCalculator.h>

class RotationTrace;
class Point2D;
class JointController;

class RotationTraceCalculator:public LineTraceCalculator{


 public:
	//Calculation method
	virtual void calculateTrace(const RotationTrace*,
															Point2D&);
	
	RotationTraceCalculator();
	
	RotationTraceCalculator(JointController* i_jointControllerPointer);
  
	RotationTraceCalculator(JointController* i_jointControllerPointer,
													const float& i_tolerance);
	
};

#endif // RotationTraceCalculator
