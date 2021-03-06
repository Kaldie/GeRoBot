
// Copyright [2015] Ruud Cools

#ifndef MATH_IO_TRACEIO_H_
#define MATH_IO_TRACEIO_H_

#include <XMLBuilder.h>
#include <Trace.h>

class TraceIO: public XMLBuilder {
  GETSETPROTECTED(Trace::TracePointer, m_tracePointer, TracePointer);

 private:
  // No default contructor!!
  TraceIO();

 protected:
  /**
   * Returns if the node is a line trace
   * The is determined based on the first child of the trace node which hold its type
   */
  bool isLineTrace() const;
  bool isCurveTrace() const;
 public:
  virtual void build();
  explicit TraceIO(const pugi::xml_node&);
  explicit TraceIO(const std::string& i_fileName);
  virtual bool update(const Trace::TracePointer& i_tracePointer);
};

#endif  // MATH_IO_TRACEIO_H_
