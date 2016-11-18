// Copyright [2015] Ruud Cools
#ifndef MATH_BASETRACECALCULATOR_H_
#define MATH_BASETRACECALCULATOR_H_

class Robot;
class Point2D;
class Trace;

class BaseTraceCalculator {
  // tolerance of the calculations
  GETSET(traceType, m_tolerance, Tolerance);
  // shared pointer to a controller
  GETSETPOINTER(Robot, m_robot, Robot);
  // translation tolerance
  GETSET(traceType, m_translationTolerance, TranslationTolerance);
  // rotation tolerance
  GETSET(traceType, m_rotationTolerance, RotationTolerance);
  // When step, write it in the log
  GETSET(bool, m_writeLog, WriteLog);
  // Filename of the log
  GETSET(std::string, m_logFileName, LogFileName);
 public:
  /*
    Get an estimate of the number of steps needed to create the specified trace.
    Use this to update the PinStateVector of the joint controller to
    reduce the number of reallocations use in this specific vector
    If the estimate is very inacurate this can lead to:
    many reallocations
    too much memory use
  */
  virtual std::vector<int> getNumberOfSteps(const Trace&,
                                            const Point2D&) const ;

  /*
    Calculate the necessary steps to conduct the trace.
    The base trace calculate simply makes the robot go to the specific point,
    override this function when implementing new types of trace calculators
  */
  virtual void calculateTrace(const Trace*);

  /*
     To re-create the steps taking during the calculation
     a file where all the steps are specified can be created.
     The location can be set with setFileLocation(const std::string&)
     The necessary switch is setWriteLog(true)
  */
  void writeToStepLog(const std::string&,  // direction
                      const int&,
                      const Point2D& i_newPosition) const;  // number of steps

  // Constructors
  BaseTraceCalculator();

  BaseTraceCalculator(Robot* i_robotPointer);

  BaseTraceCalculator(Robot* i_robotPointer,
                      const traceType& i_tolerance);
  virtual ~BaseTraceCalculator() {};

 protected:
  // See if the joint controller pointer has been set
  bool hasRobot() const;

  /**
   * Determine of the given point is within the set
   * translation tolerances of the endpoin of the trace
   * @param[in] i_trace EndPoint of this trace will be
   * used to determine if it is within tolerance
   * @param[in] i_position Current position
   */
  bool shouldRotate(const Trace& i_trace,
                    const Point2D& i_position) const;
  /**
   * Determine of the given point is within the set
   * rotation tolerances of the endpoin of the trace
   * @param[in] i_trace EndPoint of this trace will be
   * used to determine if it is within tolerance
   * @param[in] i_position Current position
   */
  bool shouldTranslate(const Trace&,
                       const Point2D&) const;
  void setTolerances();
 };

#endif  // MATH_BASETRACECALCULATOR_H_
