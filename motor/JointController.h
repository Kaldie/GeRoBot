// Copyright [2014] Ruud Cools
#ifndef MOTOR_JOINTCONTROLLER_H_
#define MOTOR_JOINTCONTROLLER_H_

#include <ArduinoMotorDriver.h>
#include <SequenceVector.h>
#include <BaseJoint.h>

class EndStop;

class JointController {
 public:
  typedef std::vector<std::shared_ptr<BaseJoint>> JointPointerVector;
  typedef std::shared_ptr<JointController> JointControllerPointer;

  /// vector with the shared pointers to joints
  GETSET(JointPointerVector, m_jointPointerVector, JointPointerVector);

  /// The actuator
  GETSET(ArduinoMotorDriver, m_actuator, Actuator);

  /// Vector where all the sequences will be stored in!
  GETSET(SequenceVector, m_sequenceVector, SequenceVector);

 public:
  JointController();
  ~JointController();

  /// empty out the pin state sequence
  void resetPinStateSequence();

  /// Register a joint so it will be controled by the controller
  bool addJoint(const std::shared_ptr<BaseJoint>&);

  /**
   * Prepare to move steps
   * @param[in] i_direction string with the direction of movements
   * @param[in] i_numberOfSteps number of steps that will be set
   */
  void moveSteps(const std::string& i_direction,
                 const int& i_numberOfSteps);

  /// Method to the Arduino actuator pointer
  ArduinoMotorDriver* getActuatorPointer() {return &m_actuator;}

  /// Method to retrieve the sequence vector pointer
  SequenceVector* getSequenceVectorPointer() {return &m_sequenceVector;}

  /**
   * Method which uploads the current sequence to the actuator
   * @param[in] i_condenseSequence Should condense the vector before upload
   */
  void uploadSequence(const bool& i_condenseSequence);

  /// function which will make the robot move!
  void actuate();

  /**
   * Method will resolve the end stop call
   * It will tell the actuator to reset the driver
   * From that call it will receive the pin state of the joints and end stops
   * Base from these it will find the end stop triggered 
   * and set the position of the corrisponding joint.
   */
  std::shared_ptr<EndStop> resolveEndStopHit();

  /**
   * Reducing the size of the vector
   * @param[in] i_removeElements remove elements from the vector
   * relay the call to the sequence vector SequenceVector::condenseVector()
   */
  bool condenseVector(bool i_removeElements = false) {
    return m_sequenceVector.condenseVector(i_removeElements);};

  /**
   * Resolve the joint based on the direction specified
   * The method determines which joint is necesary based on the movement direction
   * Means that each joint produces a specific motion
   */
  BaseJoint::JointPointer resolveJoint(const std::string& i_movementDirection) const;
  /**
   * Resolve the joint based on the movement type
   * The method determines which joint is necesary based on the movement direction
   * Means that each joint produces a specific motion
   */
  BaseJoint::JointPointer resolveJoint(const BaseJoint::MovementType& i_movementType) const;

  /**
   * Returns all the joints which have the specific movment type
   */
  JointController::JointPointerVector resolveJoints(const BaseJoint::MovementType& i_type) const;

  /**
   * returns the number of joints currently registed
   */
   int getNumberOfJoints() const {return m_jointPointerVector.size();}

   /**
    * returns the number of joints with the specific movementType
    */
   int getNumberOfJoints(const BaseJoint::MovementType& i_type) const;

   /**
    * Get the root joint, this joint is therefor the base of the robot
    */
   const BaseJoint::JointPointer getRootJoint() const;

 private:
   /**
   * returns if the joint is ready to be added to the robot
   */
   bool validateJoint(const BaseJoint::JointPointer&) const;

  /**
   * returns if the current joints will make a fine robot
   */
  bool validateJointVector(const JointPointerVector&) const;

  /**
   * identifies if the joint is already registered
   */
  bool hasJoint(const BaseJoint::JointPointer&) const;

  /// Method to retrieve the joint of the type
  BaseJoint::JointPointer getJoint(const BaseJoint::MovementType&) const;

  int resolveEndStopHit(JointPointerVector* o_jointVector,
			std::vector<std::string>* o_direction);
};

#endif  // MOTOR_JOINTCONTROLLER_H_
