#include <iostream>
#include <stdio.h>
#include <string.h>
#include <memory>
#include <vector>
#include <pugixml.hpp>


std::shared_ptr<std::vector<int>> createBigSharedPointer(){
  std::shared_ptr<std::vector<int>> sharedPointer;
  for(int i=0;
      i>10000;
      i++)
    sharedPointer->push_back(100);
  return sharedPointer;
}



void displayAttributes(const pugi::xml_node& i_node){
  for(pugi::xml_attribute attribute=i_node.first_attribute();
      attribute;
      attribute=attribute.next_attribute()){
    std::cout<<"Attribute: "<<attribute.name()<<std::endl;
  }
}


void displayChilds(const pugi::xml_node& i_parrentNode)
{
  displayAttributes(i_parrentNode);
  for(pugi::xml_node child=i_parrentNode.first_child();
      child;
      child=child.next_sibling())
    {
      std::cout<<child.name()<<"\t"<<child.text().as_string()<<std::endl;
      displayAttributes(child);
      displayChilds(child);
    }
  std::shared_ptr<std::vector<int>> pointer=createBigSharedPointer();
}

pugi::xml_node getFirstChild(pugi::xml_document& i_document,
			     const std::string i_fileName){

  
  pugi::xml_parse_result result = i_document.load_file(i_fileName.c_str());
  
  if(result)
    std::cout<<"Load result: " << result.description()<<std::endl;
  else
    std::cout<<"Load result: " << result.description()<<std::endl;
  
  if(i_document.first_child()==NULL){
    std::cerr<<"No first child found!!"<<std::endl;
    throw 100;
  }
  
  return i_document.first_child();
}


void showError(const std::string& i_fileName){

  pugi::xml_document document;
  pugi::xml_node jointController=getFirstChild(document,i_fileName);
  
  for(pugi::xml_node joint=jointController.first_child();
      joint;
      joint=joint.next_sibling()){
    std::cout<<joint.name()<<std::endl;      

    displayChilds(joint);
  }
  std::cout<<jointController.name()<<std::endl;
  
}


int main()
{
   showError("tmp.xml");
  

}
