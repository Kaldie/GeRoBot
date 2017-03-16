//Copyright Ruud Cools [2017]

#ifndef MATH_CONVERSION_BASECONVERSION_H_
#define MATH_CONVERSION_BASECONVERSION_H_

class Trace;

/// Class is an interface/ base class for all conversions
class BaseConversion {
  /// vector with the converted traces
  GET(std::vector<Trace>, m_traceVector, TraceVector);
  /// indication that it has worked
  GETSET(bool, m_succes, Succes);

  
 public:
  /// Constructor
  BaseConversion();

  /// Default destructor
  ~BaseConversion();
  
  /// Method which will convert the currently loaded stuff to the shizzle
  virtual void convert() = 0;

 private:
  /// Method that will be called before the actual conversion
  virtual void preCoversion() = 0;

  /// Method that will be called after conversion, clean up
  virtual void postConversion() = 0;
  
};

#endif  // MATH_CONVERSION_BASECONVERSION_H_
