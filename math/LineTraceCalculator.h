// Copyright [2015] Ruud Cools
#ifndef MATH_LINETRACECALCULATOR_H_
#define MATH_LINETRACECALCULATOR_H_

#include <BaseTraceCalculator.h>

class Point2D;
class Trace;
class Robot;

class LineTraceCalculator: public BaseTraceCalculator {
 public:
  // Calculation method
  virtual void calculateTrace(const Trace&);

  // Constructors
  LineTraceCalculator();

  explicit LineTraceCalculator(Robot* i_robotPointer);

  LineTraceCalculator(Robot* i_robotPointer,
                      const traceType& i_tolerance);

 protected:
  /// Calculation variables
  bool calculateStep(const Trace&) const;
  void setRotationStep();
  void setTranslationStep();
  void prepareTranslation(const Trace&) const;
  void prepareRotation(const Trace&) const;
  bool correctRotation(const Trace&) const;
  bool correctTranslation(const Trace&) const;
  void prepareNextStep();
};

#endif  // MATH_LINETRACECALCULATOR_H_
