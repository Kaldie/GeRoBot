// Copyright [2015] Ruud Cools

#ifndef MATH_IO_TRACELISTIO_H_
#define MATH_IO_TRACELISTIO_H_

#include <XMLBuilder.h>
#include "./TraceIO.h"
#include "./RotationTraceIO.h"

typedef std::vector<TracePointer> TracePointerVector;

class TraceListIO: public XMLBuilder {
 GETSET(TracePointerVector, m_vector, Vector);

 private:
  /// No default contructor!!
  TraceListIO(){};
  TracePointer handleTrace(const pugi::xml_node&) const;
  bool updateNode(pugi::xml_node* i_node,
                  const TracePointer i_pointer);
 public:
  TraceListIO(const std::string&);
  /**
   * Remove the xml node from the document
   * This function is used to remove unwanted nodes after update
   * @param[in] i_node Node to be removed
   */
  virtual bool removeNode(pugi::xml_node i_node);

  /**
   * Remove the xml node from the document
   * This function is used to add a trace node based on the pointer
   * The node needs to be still updated to the pointer
   */
  virtual pugi::xml_node addTraceNode(const TracePointer& i_pointer);


  /**
   * From the xml file,
   * retreve all the traces and store them in the the vector
   */
  virtual void build();

  /**
   * Update the xml document with all the traces in the list
   */
  virtual bool update(const TracePointerVector& i_vector);

  /**
   * Stores the current Trace list into a xml file with the given file name
   * @param[in] i_fileName File name of the to be written xml file!
   */
  virtual bool store(std::string i_fileName);

};
#endif  // MATH_IO_TRACELISTIO_H_
