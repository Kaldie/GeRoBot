// Copyright [2017] Ruud Cools

#ifndef MOTOR_MOVMENTREGISTRATOR_H_
#define MOTOR_MOVMENTREGISTRATOR_H_

class BaseJoint;

/**
 * Class registrers and calculated specifics about movement that occured
 * Movement is recorded as a function of a weak joint pointer
 * Speed controllers, and calibration modules can register them as they are necessary for there calculations
 * The registrator counts 2 types, overall steps taken and steps taking during 1 activation
 * These are call global and local steps respectively
 * Steps are stored as joint info's
 * Both can be reset as wanted.
 */
class MovementRegistrator {
 protected:
  // map in which joint info resides. Inheired this if u want to extent it
  typedef std::map<std::weak_ptr<BaseJoint>,
    int,
    std::owner_less<std::weak_ptr<BaseJoint>>> JointMap;

  /// Movement information which continues counting after/ inbetween actuations
  GETSET(JointMap, m_globalMap, GlobalMap);

  /// Movement information which resets counting after actuations
  GETSET(JointMap, m_localMap, LocalMap);

 private:
  virtual void notifySteps(JointMap* io_jointMap,
			   const std::weak_ptr<BaseJoint>& i_joint,
			   const int& i_numberOfSteps);

  virtual void addElementToMap(JointMap* io_jointMap,
			       std::weak_ptr<BaseJoint> i_joint);

  void resetJointMap(JointMap* o_map);

 public:
  /// Notify the registrator that steps have been taken
  void notifySteps(const std::shared_ptr<BaseJoint>& i_joint,
		   const int& i_numberOfSteps);

  /**
   * return the number of steps set by this joint
   * return 0 if this joint is not moved
   */
  virtual int numberOfStepsSet(const std::weak_ptr<BaseJoint>& i_joint,
		       const bool& i_isLocal = true);

  // if a shared pointer is used this called is relayed to the weak ptr version
  virtual int numberOfStepsSet(const std::shared_ptr<BaseJoint>& i_joint,
		       const bool& i_isLocal = true);
  
  void notifyActuation();
  /// Reset the local join map. Default, this is done at each activation.
  void resetLocal();
  /// Reset the global joint map. Default, this is never done.
  void resetGlobal();  
};

#endif  // MOTOR_MOVMENTREGISTRATOR_H_
