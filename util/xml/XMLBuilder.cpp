// Copyright [2015] Ruud Cools
#include <macroHeader.h>
#include "./XMLBuilder.h"

#ifdef QT
#include <QtCore/QByteArray>
#include <QtCore/QIODevice>
#endif

XMLBuilder::XMLBuilder()
  :  XMLBuilder("", false, pugi::xml_node())
{}


XMLBuilder::XMLBuilder(const std::string& i_fileName)
  :  XMLBuilder(i_fileName, false, pugi::xml_node())
{}


XMLBuilder::XMLBuilder(const pugi::xml_node& i_node)
  : XMLBuilder("", false, i_node)
{}

#ifdef QT
XMLBuilder::XMLBuilder(QFile* i_QFile)
  :m_fileName(""),
   m_hasLoaded(false),
   m_node(pugi::xml_node()),
   m_QFile(i_QFile)
{}
#endif


XMLBuilder::XMLBuilder(const std::string& i_fileName,
		       const bool& i_isLoaded,
		       const pugi::xml_node& i_node)
  : m_fileName(i_fileName),
    m_hasLoaded(i_isLoaded),
    m_node(i_node),
    m_QFile(NULL)
{}   


XMLBuilder::~XMLBuilder() {
  if (m_hasLoaded) {
    //delete m_documentPointer;
    LOG_DEBUG("Deleted document pointer!");
  } else {
    LOG_DEBUG("No document to be deleted!");
  }
}


void XMLBuilder::build() {
  // set the node to document pointer by the file name.
  setNode(loadXMLFile());
}


pugi::xml_node XMLBuilder::loadXMLFile() {
  pugi::xml_parse_result result;
  if (m_fileName != "") {
    loadFromFile(&result);
  }
#ifdef QT
  if (m_QFile) {
    if (m_QFile->exists()) {
      loadFromQFile(&result);
    }
  }
#endif

  if (!result) {
    LOG_ERROR("Load result: " << result.description());
  }
  if (!m_documentPointer->first_child()) {
    LOG_ERROR("No first child found!!");
  }
  if (std::string(m_documentPointer->first_child().name()) != "ROBOTBUILDER") {
    LOG_ERROR("Not a Robot builder xml file!");
  }
  m_hasLoaded = true;
  return m_documentPointer->first_child();
}
  

void XMLBuilder::loadFromFile(pugi::xml_parse_result* o_result) {
  m_documentPointer = std::make_shared<pugi::xml_document>();
  *o_result =
      m_documentPointer->load_file(m_fileName.c_str());
}


#ifdef QT
void XMLBuilder::loadFromQFile(pugi::xml_parse_result* o_result) {
  m_documentPointer = std::make_shared<pugi::xml_document>();
  m_QFile->open(QIODevice::ReadOnly);
  QByteArray buffer = m_QFile->readAll();
  *o_result =
    m_documentPointer->load_buffer(static_cast<const void*>(buffer.constData()),
				  buffer.size());
}
#endif

pugi::xml_node XMLBuilder::getNodeFromPath(const pugi::xml_node& i_parrentNode,
                                           const std::string& i_path) {
  pugi::xml_node childNode =
      i_parrentNode.first_element_by_path(i_path.c_str());
  if (childNode) {
    return childNode;
  } else {
    LOG_ERROR("Could not find a child node from: "
              << i_path << " starting at node: " << i_parrentNode.name());
  }
}


pugi::xml_node XMLBuilder::getNodeFromPath(const std::string& i_path) const {
  return getNodeFromPath(m_node, i_path);
}


template<class T>
std::vector<T> XMLBuilder::getList(const pugi::xml_node& i_node,
                                   const std::string& i_path,
                                   const int& i_expectedNumberOfValues,
                                   T (*f)(const pugi::xml_text&)) const {
  std::vector<T> list;
  pugi::xml_node parrentNode = i_node.first_element_by_path(i_path.c_str());
  for (pugi::xml_node node = parrentNode.first_child();
       node;
       node = node.next_sibling()) {
    list.push_back((*f)(node.text()));
  }
  if (static_cast<int>(list.size()) != i_expectedNumberOfValues) {
    LOG_ERROR("Number of expected values: " << i_expectedNumberOfValues <<
              " is not equal to: " << list.size());
  }
  return list;
}


std::vector<int> XMLBuilder::getIntList(const pugi::xml_node& i_node,
                                        const std::string& i_xmlPath,
                                        const int& i_numberOfEntries) const {
  return getList<int>(i_node,
                      i_xmlPath,
                      i_numberOfEntries,
                      &XMLBuilder::as_int);
}


std::vector<double> XMLBuilder::getDoubleList(const pugi::xml_node& i_node,
                                           const std::string& i_xmlPath,
                                           const int& i_numberOfEntries) const {
  return getList<double>(i_node,
                         i_xmlPath,
                         i_numberOfEntries,
                         &XMLBuilder::as_double);
}


std::vector<float> XMLBuilder::getFloatList(const pugi::xml_node& i_node,
                                            const std::string& i_xmlPath,
                                            const int& i_numberOfEntries) const {
  return getList<float>(i_node,
                        i_xmlPath,
                        i_numberOfEntries,
                        &XMLBuilder::as_float);
}


std::vector<std::string> XMLBuilder::getStringList(const pugi::xml_node& i_node,
                                                   const std::string& i_xmlPath,
                                                   const int& i_numberOfEntries) const {
  return getList<std::string>(i_node,
                              i_xmlPath,
                              i_numberOfEntries,
                              &XMLBuilder::as_string);
}


std::vector<bool> XMLBuilder::getBoolList(const pugi::xml_node& i_node,
                                          const std::string& i_xmlPath,
                                          const int& i_numberOfEntries) const {
  return getList<bool>(i_node,
                       i_xmlPath,
                       i_numberOfEntries,
                       &XMLBuilder::as_bool);
}


void XMLBuilder::displayTree() {
  LOG_DEBUG("Finding root node!");
  pugi::xml_node node = getNode();
  LOG_DEBUG("Root node found!");
  LOG_DEBUG("Name root node: " << node.name());
  if (node.first_child())
    LOG_DEBUG("First child found: " << node.first_child().name());
  else
    LOG_DEBUG("No first child found!");

  for (pugi::xml_node tool =node.first_child();
       tool;
       tool = tool.first_child()) {
      LOG_DEBUG("Node info: ");
      LOG_DEBUG(tool.name());
      LOG_DEBUG(tool.child_value());
    }
}


bool XMLBuilder::store(const std::string& i_fileName) {
  return m_documentPointer->save_file(i_fileName.c_str());
}
