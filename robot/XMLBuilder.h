#ifndef XMLBuilder_H
#define XMLBuilder_H

class XMLBuilder{
 private:
    GETSET(std::string,m_fileName,FileName);
    GETSET(bool,m_hasLoaded,HasLoaded);
    GETSET(pugi::xml_node,m_node,Node);

    pugi::xml_document* m_documentPointer;

    template<class T>
		std::vector<T> getList(const pugi::xml_node&,
													 const std::string&,
													 const int&,
													 T (XMLBuilder::*f)(const pugi::xml_text&)const) const;

    //need to wrap the pugi::xml_text methods, they behave rediculus as function pointers....
    float as_float(const pugi::xml_text& i_text) const {return i_text.as_float();};
    int as_int(const pugi::xml_text& i_text) const {return i_text.as_int();};
    std::string as_string(const pugi::xml_text& i_text) const {return i_text.as_string();};
    bool as_bool(const pugi::xml_text& i_text) const {return i_text.as_bool();};

    XMLBuilder();

 public:
    //Build they objects
    virtual void build();
    virtual void displayTree();

    //Get value lists    
    std::vector<int> getIntList(const pugi::xml_node& i_node,
																const std::string& i_xmlPath,
																const int& i_numberOfEntries) const;
		
    std::vector<float> getFloatList(const pugi::xml_node& i_node,
																		const std::string& i_xmlPath,
																		const int& i_numberOfEntries) const;
		
    std::vector<std::string> getStringList(const pugi::xml_node& i_node,
																					 const std::string& i_xmlPath,
																					 const int& i_numberOfEntries) const;
    
    std::vector<bool> getBoolList(const pugi::xml_node& i_node,
																	const std::string& i_xmlPath,
																	const int& i_numberOfEntries) const;
		
    pugi::xml_node loadXMLFile();

    pugi::xml_node getNodeFromPath(const pugi::xml_node&,
																	 const std::string&) const;

    pugi::xml_node getNodeFromPath(const std::string&) const;

		virtual bool store(const std::string& i_fileName);

    XMLBuilder(const std::string&);
    XMLBuilder(const pugi::xml_node&);
    ~XMLBuilder();
};

#endif // XMLBuilder






