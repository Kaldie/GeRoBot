// Copyright [2015] Ruud Cools

#ifndef MATH_IO_ROTATIONTRACEIO_H_
#define MATH_IO_ROTATIONTRACEIO_H_
#include <RotationTrace.h>
#include "./TraceIO.h"

typedef std::shared_ptr<RotationTrace> RotationTracePointer;

class RotationTraceIO: public TraceIO {
 private:
  // No default contructor!!
  RotationTraceIO();

 public:
  virtual void build();
  explicit RotationTraceIO(const pugi::xml_node&);
  virtual bool update(const TracePointer& i_tracePointer);
};

#endif  // MATH_IO_ROTATIONTRACEIO_H_
