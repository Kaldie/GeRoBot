// Copyright [2015] Ruud Cools

#ifndef MATH_ROTATIONTRACECALCULATOR_H_
#define MATH_ROTATIONTRACECALCULATOR_H_

#include <LineTraceCalculator.h>

class RotationTrace;
class Point2D;
class JointController;
class Robot;

class RotationTraceCalculator: public LineTraceCalculator {
 public:
  /**
   * Calculate the trace
   * @param[in] i_trace i_trace trace that needs to be evaluated
   */
  virtual void calculateTrace(const Trace* i_trace);
  /// Default constructor
  RotationTraceCalculator();

  /// Constructor with defined joint controller pointer
  explicit RotationTraceCalculator(Robot* i_robot);

  /// Constructor with tolerance and joint controller
  RotationTraceCalculator(Robot* i_robot, const traceType& i_tolerance);

  private:
  /**
   * Ensure the proper trace is calculated.
   * An RotationTrace is calculated by splitting it up in several pieces.
   * This will avoid sortcuts.
   * However it is still possible that the incorrect trace is calculated
   * When the robot is just before this the endpoint of this intermeditate trace
   * the robot will walk back and will take a shortcut.
   * This lead to very gross errors.
   * By moving the robot before entering a new intermeditate trace this is effect is avoided
   */
  void ensureProperMovement(const std::vector<RotationTrace>& i_traces,
                            const std::vector<RotationTrace>::const_iterator& i_traceIterator,
                            const Point2D& i_startPosition);
  /// Prepare rotation movement based on the actual position and necessity to move
  bool ensureRotationMovement(const std::vector<RotationTrace>& i_traces,
                              const std::vector<RotationTrace>::const_iterator& i_trace);
  /// prepare translation movement to ensure the robot is behind the endpoint of the intermediate trace
  void ensureTranslationMovement(const std::vector<RotationTrace>::const_iterator& i_trace,
                                 const std::string& i_requiredDirection);
  /// Returns if during this trace the robot needs to rotate
  bool shouldRotateAtAll(const std::vector<RotationTrace>& i_traces,
                    const std::vector<RotationTrace>::const_iterator& i_currentTrace,
                    const Point2D& i_currentPosition);


};

#endif  // MATH_ROTATIONTRACECALCULATOR_H_
