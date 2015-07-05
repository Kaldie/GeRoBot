// Copyright [2015] Ruud Cools
#ifndef MATH_BASETRACECALCULATOR_H_
#define MATH_BASETRACECALCULATOR_H_

class Point2D;
class JointController;
class Trace;

class BaseTraceCalculator {
  GETSET(float, m_tolerance, Tolerance);
  GETSETPOINTER(JointController, m_jointController, JointController);
  GETSET(float, m_translationTolerance, TranslationTolerance);
  GETSET(float, m_rotationTolerance, RotationTolerance);
  GETSET(bool, m_writeLog, WriteLog);
  GETSET(std::string, m_logFileName, LogFileName);

 private:
  // assign operator
  //  BaseTraceCalculator& operator=(const BaseTraceCalculator& obj);
  // copy constructor
  BaseTraceCalculator(const BaseTraceCalculator& obj);
 protected:
  // See if the joint controller pointer has been set
  bool hasJointController() const;
  bool shouldRotate(const Trace&,
                    const Point2D&) const;
  bool shouldTranslate(const Trace&,
                       const Point2D&) const;
  void setTolerances();

 public:
  /*
    Get an estimate of the number of steps needed to create the specified trace.
    Use this to update the PinStateVector of the joint controller to 
    reduce the number of reallocations use in this specific vector
    If the estimate is very inacurate this can lead to:
    many reallocations
    too much memory use
  */
  virtual std::vector<int> getNumberOfSteps(const Trace*,
                                            const Point2D&) const ;

  /*
    Calculate the necessary steps to conduct the trace.
    The base trace calculate simply makes the robot go to the specific point,
    override this function when implementing new types of trace calculators
  */			
  virtual void calculateTrace(const Trace*, Point2D&);
		
  /* 
     To re-create the steps taking during the calculation 
     a file where all the steps are specified can be created.
     The location can be set with setFileLocation(const std::string&)
     The necessary switch is setWriteLog(true)
  */
  void writeToStepLog(const std::string, // direction
                      const int) const;  // number of steps

  // Constructors
  BaseTraceCalculator();
		
  BaseTraceCalculator(JointController*);
    
  BaseTraceCalculator(JointController*,
                      const float& i_tolerance);
  virtual ~BaseTraceCalculator() {};
};

#endif  // MATH_BASETRACECALCULATOR_H_
