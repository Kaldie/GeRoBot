// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <XMLBuilder.h>
#include "./TraceIO.h"
#include "./Point2DIO.h"
#include "./TraceListIO.h"
#include "./RotationTraceIO.h"
#ifdef QT
#include <QtCore/QFile>
#endif

void TraceListIO::build() {
  pugi::xml_node traceListNode = getNodeFromPath("./TraceList");
  for (pugi::xml_node node = traceListNode.first_child();
       node;
       node = node.next_sibling()) {
    if (strcmp(node.name(),"Trace") == 0) {
      LOG_DEBUG("Fond a trace.");
      m_vector.push_back(handleTrace(node));
    } else {
      LOG_DEBUG("Did not find a trace node!" <<std::endl<<
                "Found: " << node.name());
    }
  }
}


Trace::TracePointer TraceListIO::handleTrace(const pugi::xml_node& i_node) const {
  TraceIO traceIO(i_node);
  traceIO.build();
  Trace::TracePointer traceP = traceIO.getTracePointer();
  if (traceP->getTraceType() == Trace::Curve) {
    LOG_DEBUG("This trace is a rotation trace");
    RotationTraceIO rotationTraceIO(i_node);
    rotationTraceIO.build();
    traceP = rotationTraceIO.getTracePointer();
  }
  return traceP;
}


TraceListIO::TraceListIO(const std::string& i_fileName)
  : XMLBuilder(i_fileName) {
  XMLBuilder::build();
}


TraceListIO::TraceListIO(QFile* i_file)
  : XMLBuilder(i_file) {
  XMLBuilder::build();
}


bool TraceListIO::update(const Trace::TracePointerVector& i_traceVector) {
  bool hasSucceded = true;
  pugi::xml_node traceNode = getNodeFromPath("./TraceList").first_child();
  for (auto trace : i_traceVector) {
    if (!traceNode) {
      LOG_DEBUG("Add trace node to the document!");
      traceNode = addTraceNode(trace);
    }
    LOG_DEBUG("Update a trace from TraceListIO");
    updateNode(&traceNode, trace);
    traceNode = traceNode.next_sibling();
  }

  pugi::xpath_query query("./Trace");
  pugi::xpath_node_set traces = query.evaluate_node_set(getNodeFromPath("./TraceList"));
  while (traces.size() > i_traceVector.size()) {
    LOG_DEBUG("Current number of Traces: " << traces.size() <<
              "Number of new Traces: " << i_traceVector.size());
    LOG_DEBUG("Found an unwanted trace!");
    pugi::xpath_node badNode = traces[i_traceVector.size()];
    getNodeFromPath("./TraceList").remove_child(badNode.node());
    traces = query.evaluate_node_set(getNodeFromPath("./TraceList"));
  }
  return hasSucceded;
}



bool TraceListIO::removeNode(pugi::xml_node i_traceNode) {
  pugi::xml_node documentNode = getNode();
  bool hasWorked = documentNode.remove_child(i_traceNode);
  setNode(documentNode);
  return hasWorked;
}


pugi::xml_node TraceListIO::addTraceNode(const Trace::TracePointer& i_pointer) {

  pugi::xml_node documentNode = getNodeFromPath("./TraceList");
  pugi::xml_node traceNode = documentNode.append_child("Trace");
  traceNode.append_child("Type").append_child(pugi::node_pcdata);
  Point2DIO point2DIO(traceNode);
  point2DIO.addPointNode("StartPoint");
  point2DIO.addPointNode("EndPoint");
  if (i_pointer->getTraceType() == Trace::Curve) {
    point2DIO.addPointNode("CenterPoint");
  }
  return traceNode;
}


bool TraceListIO::updateNode(pugi::xml_node* i_node,
                             const Trace::TracePointer i_pointer) {
  bool hasSucceded = true;
  if (i_pointer->getTraceType() == Trace::Curve) {
    RotationTraceIO rotationTraceIO(*i_node);
    hasSucceded &= rotationTraceIO.update(i_pointer);
  } else {
    TraceIO traceIO(*i_node);
    hasSucceded &= traceIO.update(i_pointer);
  }
  return hasSucceded;
}


bool TraceListIO::store (std::string i_fileName){
  return XMLBuilder::store(i_fileName.c_str());
}
