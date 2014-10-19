#include "StepperDriverBuilder.h"

void StepperDriverBuilder::build()
{
  LOG_DEBUG("Buildiing a stepperdriver!");
  std::string actuatorType=getNode().first_child().text().as_string();
  if(actuatorType!="STEPPERDRIVER")
    LOG_ERROR("Actuator type is only stepper driver!!!!");
  
  std::vector<int> pinVector=getIntList(getNode(),
					"./PINS",
					3);
				
  StepperDriver step(pinVector);
  step.setDefaultDirection(getNode().first_element_by_path("./DEFAULT_DIRECTION").text().as_string());
  
  LOG_DEBUG("Default direction is: "<<step.getDefaultDirection());
  
  step.setHoldMotor(getNode().first_element_by_path("./HOLD_MOTOR").text().as_bool());
  
  LOG_DEBUG("Hold motor: "<<step.getHoldMotor());
  
  setStepperDriver(step);
}


StepperDriverBuilder::StepperDriverBuilder(const pugi::xml_node& i_node):
  XMLBuilder(i_node)
{}
