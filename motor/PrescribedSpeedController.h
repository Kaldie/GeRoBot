// Copyright [2015] Ruud Cools

#ifndef MOTOR_PRESCRIBEDSPEEDCONTROLLER_H_
#define MOTOR_PRESCRIBEDSPEEDCONTROLLER_H_

#include "./BaseJoint.h"
#include <forward_list>
#include "./SpeedController.h"

class JointController;
class StateSequence;

class PrescribedSpeedController : public SpeedController {

  struct JointInfo
  {
    int m_numberOfSteps;
    std::forward_list<float> m_frequency;
    std::forward_list<float> m_missedSteps;
    int m_driverFrequency;
    bool m_missedStepIsUpdated;
  };

  /// Current speed of the joints
  typedef std::map<BaseJoint::WeakJointPointer,
                   JointInfo,
                   std::owner_less<BaseJoint::WeakJointPointer>> JointMap;

  /// shared pointer of the class
  typedef std::shared_ptr<SpeedController> SpeedControllerPointer;

  /// Mapping the number of consecutive steps with the joint
  GETSET(JointMap, m_jointMap, StepMap);

  /// The current frequency with which the actuator is currently setting steps
  GETSET(int, m_motorFrequency, MotorFrequency);

  /// Wanted speed of the robot
  GETSET(traceType , m_robotSpeed, RobotSpeed);

  /// Speeds the robot moved recently
  GETSET(std::forward_list<float>, m_achievedSpeeds, AchievedSpeeds);

  /// number of elements to average over
  GETSET(int, m_averageElements, AverageElements);

  /// Set the speed constroller to a conservative mode
  GETSET(bool, m_isConservative, IsConservative);


  /**
   * Notify the controller that steps have been taken
   * @param [in] i_joint the joint that has taken the step
   * @param [in] i_direction the direction it has been taken in
   * @param [in] i_numberOfSteps the number of steps
   */
  void notifyStep(const BaseJoint::JointPointer& i_joint,
                  const unsigned int& i_numberOfSteps);

  /// Request speed
  bool adviseSpeed(int* o_speed) const;

  /**
   * Let the controller know a speed is selected
   * The controller will hanlde setting the speeds to the joint actuators
   */
  virtual void acknowledgeSpeed(const unsigned int& i_frequency,
                                SequenceVector* i_sequenceVector);

  /// calculate the current speed of the robot
  float estimateCurrentSpeed() const;

  /// get the distance steped in this session
  float getCurrentStepedDistance() const;

  /// easy constructor
  PrescribedSpeedController();

  /// Fully fledged
  PrescribedSpeedController(const traceType& i_speed);

 private:
  ///
  float adviceAction(const float& i_currentSpeed,
                     const bool& i_accelerate) const;

  /// get average of the iterator
  template <class T>
  static void average(const std::forward_list<T>& i_forwardList, T* o_average);

  template <class T>
  static T average(const std::forward_list<T>& i_forwardList);

  /// trim a forward list to the requested size if needed
  template <class T>
  static void trimList(std::forward_list<T>* i_forwardList,
                const int& i_size);

  /// use the previous results to estimate the upcoming value
  template <class T>
  float estimateNextElement(const std::forward_list<T>& i_forwardList,
                            const int& i_numberOfDerivatives = 3) const;

  /// get the number if steps currently under decision
  int currentNumberOfSteps() const;

  /// Update an element of the JointMap based on the number of steps
  void updateJointMapElement(JointInfo* i_info,
                             const int i_numberOfSteps);

  /// Get the maximum frequency of all the joints
  float getMaximumFrequency(const bool& i_excludeIdle = false) const;

  /// Get the minimum frequency of all the joints
  float getMinimumFrequency(const bool& i_excludeIdle = false) const;

  /// get the frequency bounds of the current joints
  bool getFrequencyBounds(int* o_lowerBound,
                          int* o_upperBound) const;


  /// calculate the frequency bounds of a working motor
  void boundsOfWorkingMotor(float* o_lowerBound,
                            float* o_upperBound,
                            const JointInfo& i_jointInfo,
                            const BaseMotor* i_motor) const;

  /// calculate the frequency bounds of an idle motor
  void boundsOfIdleMotor(float* o_lowerBound,
                         float* o_upperBound,
                         const JointInfo& i_jointInfo,
                         BaseMotor* i_motor) const;

  /// Based on the current frequency, calculate the current speed
  float calculateCurrentSpeed() const;

  /// initialise the motor frequence based on the first joint
  void initialiseMotorFrequency(const BaseJoint::JointPointer& i_joint);
};

#endif  // MOTOR_PRESCRIBEDSPEEDCONTROLLER_H_
