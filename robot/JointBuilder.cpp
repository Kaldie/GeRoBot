#include "JointBuilder.h"
#include "StepperDriverBuilder.h"
#include <StepperDriver.h>
#include <RotationalJoint.h>
#include <TranslationalJoint.h>

void JointBuilder::build()
{
  LOG_DEBUG("Building a joint");
  pugi::xml_node jointNode=getNode();

  for(pugi::xml_attribute attribute=jointNode.first_attribute();
      attribute;
      attribute=attribute.next_attribute()){
    if(std::string(attribute.name())=="type" and std::string(attribute.value())=="StepperDriver"){
      LOG_DEBUG("Found 1");

      JointPointer jointPointer;

      std::string movementType=getNodeFromPath(jointNode,"./MOVEMENT_TYPE").text().as_string();

      if(movementType=="ROTATIONAL"){
	LOG_DEBUG("Building a rotational joint!");
	RotationalJoint<StepperDriver> joint;
	jointPointer=joint.clone();
      }
      
      else if(movementType=="TRANSLATIONAL"){
	LOG_DEBUG("Building a translational joint!");
	TranslationalJoint<StepperDriver> joint;
	jointPointer=joint.clone();
      }
      
      else{
	LOG_ERROR("Movement type: "<<movementType<<" is not correct!");
      }
      

      
      jointPointer->setMovementPerStep(getNodeFromPath(jointNode,"./MOVEMENT_PER_STEP").text().as_float());
      jointPointer->setPosition(getNodeFromPath(jointNode,"./DEFAULT_POSITION").text().as_float());
    
      LOG_DEBUG("Joint type: "<<jointPointer->getMovementType());
      LOG_DEBUG("Default position: "<<jointPointer->getPosition());
    

      *(jointPointer->getMotor())=parseStepperDriver(getNodeFromPath(jointNode,"./ACTUATOR"));
      
      LOG_DEBUG("hoi");


      std::vector<float> range=getFloatList(jointNode,"./RANGE",2);
      jointPointer->setRange(range);
	    
      std::vector<std::string> directionConversionStringVector=getStringList(jointNode,
									     "./DIRECTION_CONVERSION_MAP",
									     2);
      DirectionConversionMap directionMap;
      int delimPosition;
      for(std::vector<std::string>::iterator itr=directionConversionStringVector.begin();
	  itr!=directionConversionStringVector.end();
	  itr++){
	delimPosition=(*itr).find_first_of(",");
	LOG_DEBUG("Direction "<<(*itr).substr(0,delimPosition)<<
		  " will be converted to "<<(*itr).substr(delimPosition+1));
	directionMap[(*itr).substr(0,delimPosition)]=(*itr).substr(delimPosition+1);
      }

      jointPointer->setDirectionConversionMap(directionMap);

      
      
      setJointPointer(jointPointer);
      
      LOG_DEBUG("Construction of the pointer is finished!");
      
    }
    
    LOG_DEBUG("Joint Build is finshed!");
  }
}

StepperDriver JointBuilder::parseStepperDriver(const pugi::xml_node& i_node)
{
  StepperDriverBuilder stepperDriverBuilder(i_node);
  stepperDriverBuilder.build();
  return stepperDriverBuilder.getStepperDriver();
}


JointBuilder::JointBuilder(const pugi::xml_node& i_node)
{
  setNode(i_node);
}
