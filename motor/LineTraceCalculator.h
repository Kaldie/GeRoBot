#include "BaseTraceCalculator.h"
#include <JointController.h>
#include <Point2D.h>


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

    const bool correctRotation(const Trace*,
			       Point2D&) const;
    const bool correctTranslation(const Trace*,
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
    
    //assign operator
    LineTraceCalculator(const LineTraceCalculator& obj);
};
