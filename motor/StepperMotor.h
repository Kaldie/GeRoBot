/*Header file for the StepperMotor
 Will also define the direction enum
*/

#ifndef StepperMotor_h
#define StepperMotor_h

#include <string>
#include <vector>
#include "BaseMotor.h"


class StepperMotor:public BaseActuator
{
 private:
  
  //step the motor is in now!
  int m_currentStep=0;

  //step array where the steps are defined in.
  static const int m_stepArray[m_numberOfSteps][m_numberOfPins];

  //set the state of the motor, if in incremention is used the motor will step!
  void setState(const int*);
  
  //Given a string, CCW or CW, identify if the stepArray needs to be transversed left of right
  const int getDirectionMultiplier(const std::string&);

  //Update the current step given the direction
  void updateCurrentStep(const std::string&);
  
 protected:
  bool m_calculateSecundaryDelay=true;
  std::vector<int> m_PrimaryDelayVector;
  std::vector<int> m_SecundaryDelayVector;
  std::string m_defaultDirection="CW";
  
 public:
  void printProposedDelay(std::vector<int>*);
  void printProposedDelay(int*);

  void calculateSecundaryDelay(int&, int&);
  void calculateSecundaryDelay(std::vector<int>*);  
 
  const std::string getDefaultDirection() const {return m_defaultDirection;};
  void setDefaultDirection(const std::string&);

  //Methods
  const std::vector<int> getPins()const;
  const int* getCurrentStep()const {return &m_currentStep;};

  //Move steps
  virtual void moveSteps(int,int,std::string) const ;
  virutal void moveSteps(std::vector<int>*,std::vector<std::string>*) const;
  
  //Constructors
  StepperMotor();
  StepperMotor(int,int,int,int);
  StepperMotor(const StepperMotor&);
};
 
#endif 
