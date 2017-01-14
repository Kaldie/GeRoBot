// Copyright Ruud Cools [2017]
#include <macroHeader.h>
#include <ManualSpeedController.h>

ManualSpeedController::ManualSpeedController()
  : ManualSpeedController(0,0) {
}

ManualSpeedController::ManualSpeedController(const float& i_robotSpeed)
  : ManualSpeedController(i_robotSpeed,0) {
}

ManualSpeedController::ManualSpeedController(const float& i_robotSpeed,
					     const int& i_currentSequenceVectorPosition)
  : SpeedController(Manual, i_robotSpeed, i_currentSequenceVectorPosition),
  m_speedVector(),
  m_vectorPosition(0) {
}


/// This method will be called at the moment the robot needs to decied on some speed
bool ManualSpeedController::adviseSpeed(int* o_speed) const {
  if (m_speedVector.size() == 0) {
    *o_speed = m_robotSpeed;
    return true;
  }
  *o_speed = m_speedVector[m_vectorPosition];
  return true;
}


void ManualSpeedController::acknowledgeSpeed(const unsigned int& i_speed,
					     SequenceVector* i_sequenceVector) {
  SpeedController::acknowledgeSpeed(i_speed, i_sequenceVector);
  ++m_vectorPosition;
  if (m_vectorPosition >= static_cast<int>(m_speedVector.size())) {
    m_vectorPosition = 0;
  }
}


/// This method will load a nice speed vector which has some acceleration
bool ManualSpeedController::loadVector(const int& i_startSpeed,
				       const int& i_endSpeed,
				       const int& i_acceleration) {
  m_speedVector.clear();
  m_vectorPosition = 0;
  int currentSpeed(i_startSpeed);
  while (currentSpeed < i_endSpeed) {
    m_speedVector.push_back(currentSpeed);
    currentSpeed += i_acceleration;
  }
  return true;
}

