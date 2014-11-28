// Copyright [2014] Ruud Cools

#include <macroHeader.h>

PinStateSequence::PinStateSequence()
  : m_speed(50),
    m_numberOfSteps(0),
    m_numberOfRepetitions(0)
{}


PinStateSequence::PinStateSequence(const int& i_speed,
                                   const int& i_numberOfSteps,
                                   const int& i_numberOfRepetitions,
                                   const vector<int>& i_integerSequence)
  : m_speed(i_speed),
    m_numberOfSteps(i_numberOfStep),
    m_numberOfRepetitions(0),
    m_integerSequence(i_integerSequence) {
  
        
    if (i_integerSequence.size()!= m_numberOfSteps) {
      LOG_ERROR("Number of steps in the vector: " << i_integerSequence.size() <<
                "does not corrispond to the indicated number: " << m_numberOfSteps);      
    }
      
      
    if (i_numberOfSteps > 99) {
      LOG_ERROR("Number of steps should be smaller then 99, not : " << i_numberOfSteps);
    }
      
    if (
} 


PinStateSequence::createArduinoString() const {
  std::stringstream stream;
  std::string resultingString,finalString;
  
  // set the speed
  stream<< std::setw(2) << std::setfill('0') << m_speed << ",";
  stream>>resultingString;
  finalString+=resultString;
  
  // set the number of steps
  stream<< std::setw(2) << std::setfill('0') << m_numberOfSteps << ",";
  stream>>resultingString;
  finalString+=resultString;
  
        