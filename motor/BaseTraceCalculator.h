#ifndef BaseTraceCalculator_H
#define BaseTraceCalculator_H

#include <macroHeader.h>
#include <Point2D.h>
#include <Trace.h>
#include <JointController.h>
#include <vector>

class BaseTraceCalculator
{
 protected:
    //Calculation variables
    GETSET(float,m_tolerance,Tolerance);
    GETSETPOINTER(JointController,m_jointController,JointController);

    GETSET(float,m_translationTolerance,TranslationTolerance);
    GETSET(float,m_rotationTolerance,RotationTolerance);
    
    GETSET(bool,m_writeLog,WriteLog);
    GETSET(std::string,m_logFileName,LogFileName);
    
    //See if the joint controller pointer has been set
    bool hasJointController();

    virtual void writeToStepLog(const std::string, //direction
				 const int) const;  //number of steps

		bool shouldRotate(const Trace&,
			    const Point2D&) const;

    bool shouldTranslate(const Trace&,
			       const Point2D&) const;

    void setTolerances();

 public:
    //getter and setter
    virtual std::vector<int> getNumberOfSteps(const Trace*,
																										const Point2D&) const ;
    
    virtual void calculateTrace(const Trace*,Point2D&);

    //Constructors
    BaseTraceCalculator();

    BaseTraceCalculator(JointController*);
    
    BaseTraceCalculator(JointController*,
			const float& i_tolerance);
    
    //assign operator
    BaseTraceCalculator(const BaseTraceCalculator& obj);
};

#endif // BaseTraceCalculator
