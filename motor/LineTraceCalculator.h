#ifndef LinetraceCalculator_H
#define LinetraceCalculator_H

#include "BaseTraceCalculator.h"

class Point2D;
class Trace;
class JointController;

class LineTraceCalculator: public BaseTraceCalculator
{
 protected:

    //Calculation variables
    bool m_hasRotated=true;

    bool m_hasTranslated=true;

    bool calculateStep(const Trace*,
											 Point2D&) const;
    
    void setRotationStep();

    void setTranslationStep();
		
    void prepareTranslation(const Trace*,
														Point2D&)const;
		
    void prepareRotation(const Trace*,
												 Point2D&)const;
		
		bool correctRotation(const Trace*,
															 Point2D&) const;

    bool correctTranslation(const Trace*,
																	Point2D&)const;
		
    bool hasStepped();

    void prepareNextStep();
    
 public:

    //Calculation method
    virtual void calculateTrace(const Trace*,
																Point2D&);
    //Constructors
    LineTraceCalculator();

    LineTraceCalculator(JointController*);
    
    LineTraceCalculator(JointController*,
												const float& i_tolerance);
    
    //Copy constructor
    LineTraceCalculator(const LineTraceCalculator& obj);
};

#endif // LinetraceCalculator
