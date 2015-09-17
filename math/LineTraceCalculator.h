// Copyright [2015] Ruud Cools
#ifndef MATH_LINETRACECALCULATOR_H_
#define MATH_LINETRACECALCULATOR_H_

#include <BaseTraceCalculator.h>

class Point2D;
class Trace;
class JointController;

class LineTraceCalculator: public BaseTraceCalculator {
 protected:
  // Calculation variables
  bool m_hasRotated = true;
  bool m_hasTranslated = true;
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
                          Point2D&) const;
  bool hasStepped();
  void prepareNextStep();

 public:
  // Calculation method
  virtual void calculateTrace(const Trace*,
                              Point2D&);
  // Constructors
  LineTraceCalculator();

  explicit LineTraceCalculator
    (const JointController::JointControllerPointer& i_jointControlerPointer);

  LineTraceCalculator(const JointController::JointControllerPointer&,
                      const traceType& i_tolerance);
};

#endif  // MATH_LINETRACECALCULATOR_H_
