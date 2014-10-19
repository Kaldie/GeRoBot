#include "BaseTraceCalculator.h"
#include "Robot.h"
#include <Point2D.h>


class TraceCalculator: BaseTraceCalculator
{
 private:
    //Calculation variables
    bool m_hasRotated=true;
    bool m_hasTranslated=true;

    int m_currentTranslationDelay=0;
    int m_currentRotationDelay=0;

    //no public default constructor
    TraceCalculator();
  
    void calculateStep(DelayList*,
		     DelayList*);
    
    void prepareRotation(DelayList*,DelayList*);
    void prepareTranslation(DelayList*,DelayList*);
  
    int correctRotation(DelayList*);
    int correctTranslation(DelayList*);
  
    bool hasStepped();
    void prepareNextStep();
    
 public:

    //Calculation method
    virtual void determineDelayLists(Trace*);

    //Constructors
    TraceCalculator(Robot&);
    
    TraceCalculator(Robot&,
		    const float& i_tolerance);
    
    //assign operator
    TraceCalculator(const TraceCalculator& obj);
};
