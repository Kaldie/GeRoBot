// Copyright [2015] Ruud Cools

#ifndef MATH_IO_TRACEIO_H_
#define MATH_IO_TRACEIO_H_

#include <XMLBuilder.h>
#include <Trace.h>

typedef std::shared_ptr<Trace> TracePointer;

class TraceIO: public XMLBuilder {
  GETSET(TracePointer, m_tracePointer, TracePointer);

 private:
  // No default contructor!!
  TraceIO();

 public:
  virtual void build();
  explicit TraceIO(const pugi::xml_node&);
  explicit TraceIO(const std::string i_fileName);
  virtual bool update(const TracePointer& i_tracePointer);
};

#endif  // MATH_IO_TRACEIO_H_
