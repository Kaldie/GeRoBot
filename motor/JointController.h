// Copyright [2014] Ruud Cools
#ifndef MOTOR_JOINTCONTROLLER_H_
#define MOTOR_JOINTCONTROLLER_H_

#include <ArduinoMotorDriver.h>
#include <StateSequence.h>
#include <SequenceVector.h>
#include <BaseJoint.h>

class JointController {
 public:
  typedef std::vector<BaseJoint::JointPointer> JointPointerVector;
  JointController();
  ~JointController();

  /// empty out the pin state sequence
  void resetPinStateSequence();

  /// Register a joint so it will be controled by the controller
  bool addJoint(const BaseJoint::JointPointer&);

  /**
   * Pepare to move a step
   * @param[in] jointPointer Joint that will move
   * @param[in] direction Direction string of the movement
   * @param[in] appendToPrevious try to append to the last stateSequence
   */
  void moveStep(BaseJoint::JointPointer& jointPointer,
                const std::string& direction,
                const bool& appendToPrevious);

  /**
   * Prepare to move steps
   * @param[in] i_jointPointer pointer to the joint that needs to move
   * @param[in] direction string with the direction of movements
   * @param[in] numberOfSteps number of steps that will be set
   * @param[in] i_append try to append the steps to the last StateSequence of the SequenceVector
   */
  void moveSteps(BaseJoint::JointPointer& jointPointer,
                 const std::string& direction,
                 const int& numberOfSteps,
                 const bool i_append = false);

  /// Methods to retrieve the joint(s) of a specific type
  JointPointerVector getJoints(const MovementType&);
  BaseJoint::JointPointer& getJoint(const MovementType&);

  /// Method to the Arduino actuator pointer
  ArduinoMotorDriver* getActuatorPointer() {return &m_actuator;}

  /// function which will make the robot move!
  void actuate();

  /**
   * Reducing the size of the vector
   * @paramp[in] i_removeElements remove elements from the vector
   * relay the call to the sequence vector SequenceVector::condenseVector()
   */
  bool condenseVector(bool i_removeElements = false) {
    return m_sequenceVector.condenseVector(i_removeElements);};

 private:
  /// vector with the joints
  GETSET(JointPointerVector, m_jointPointerVector, JointPointerVector);
  GETSET(ArduinoMotorDriver, m_actuator, Actuator);

  /// Vector where all the sequences will be stored in!
  GET(SequenceVector,
      m_sequenceVector,
      SequenceVector);

  /**
   * returns the number of joints currently registed
   */
   int getNumberOfJoints() {return m_jointPointerVector.size();}

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
};

#endif  // MOTOR_JOINTCONTROLLER_H_
