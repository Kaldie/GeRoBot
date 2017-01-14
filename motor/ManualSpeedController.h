#ifndef MOTOR_MANUALSPEEDCONTROLLER_H_
#define MOTOR_MANUALSPEEDCONTROLLER_H_

#include <SpeedController.h>

class ManualSpeedController : public SpeedController {
  GETSET(std::vector<int>, m_speedVector, SpeedVector);
  GETSET(int, m_vectorPosition, VectorPosition);
  
 public:
  ManualSpeedController();
  
  ManualSpeedController(const float& i_robotSpeed);
  
  ManualSpeedController(const float& i_robotSpeed,
			const int& i_currentSequenceVectorPosition);

  /// This method will be called at the moment the robot needs to decied on some speed
  virtual bool adviseSpeed(int* o_speed) const override;

  virtual void acknowledgeSpeed(const unsigned int& i_speed,
                                SequenceVector* i_sequenceVector) override;


  /// This method will load a nice speed vector which has some acceleration
  bool loadVector(const int& i_startSpeed,
		  const int& i_endSpeed,
		  const int& acceleration);
  
};

#endif  // MOTOR_MANUALSPEEDCONTROLLER_H_
