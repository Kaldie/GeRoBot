// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include <EndStopIO.h>

void EndStopIO::build() {
  LOG_DEBUG("Building a EndStop");
  m_endStop.reset(new EndStop);
  m_endStop->setPosition(getNodeFromPath(m_node, "./POSITION").text().as_double());
  m_endStop->setPinNumber(getNodeFromPath(m_node, "./PINNUMBER").text().as_int());
  m_endStop->setActivationState(getNodeFromPath(m_node, "./ACTIVATIONSTATE").text().as_bool());
  m_endStop->setActivationDirection(getNodeFromPath(m_node, "./ACTIVATIONDIRECTION").text().as_string());
}


EndStopIO::EndStopIO(const pugi::xml_node& i_node) {
  setNode(i_node);
}


bool EndStopIO::update(const std::shared_ptr<EndStop>& i_endStop) {
  getNodeFromPath(m_node, "./POSITION").text().set(i_endStop->getPosition());
  getNodeFromPath(m_node, "./PINNUMBER").text().set(i_endStop->getPinNumber());
  getNodeFromPath(m_node, "./ACTIVATIONSTATE").text().set(i_endStop->getActivationState());
  getNodeFromPath(m_node, "./ACTIVATIONDIRECTION").text().set(i_endStop->getActivationDirection().c_str());
  return true;
}


bool EndStopIO::createNode(pugi::xml_node* i_parent) {
  pugi::xml_node endStopNode = i_parent->append_child("ENDSTOP");
  endStopNode.append_child("POSITION")
    .append_child(pugi::node_pcdata).set_value("0.0");
  endStopNode.append_child("PINNUMBER")
    .append_child(pugi::node_pcdata).set_value("0");
  endStopNode.append_child("ACTIVATIONDIRECTION")
    .append_child(pugi::node_pcdata).set_value("CCW");
  endStopNode.append_child("ACTIVATIONSTATE")
    .append_child(pugi::node_pcdata).set_value("false");
  return true;
}
