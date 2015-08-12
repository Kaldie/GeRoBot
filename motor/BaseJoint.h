// copyright [2015] Ruud Cools

#ifndef MOTOR_BASEJOINT_H_
#define MOTOR_BASEJOINT_H_

class Point2D;
// forward declare the class so we can create a type def JointPointer
class BaseJoint;


class BaseJoint {
 private:
    // Pure virtual call!
    virtual int getPositionModifier(const std::string&) const =0;

 protected:
    GETSET(double, m_currentPosition, Position)
    GETSET(double, m_movementPerStep, MovementPerStep)
    GET(std::vector<double>, m_range, Range)
    GETSET(MovementType, m_movementType, MovementType)
    GETSET(DirectionConversionMap,
           m_directionConversion,
           DirectionConversionMap)

    virtual BaseJoint* cloneImpl() const = 0;
    void isInRange(double i_jointPosition);

 public:
    /**
     * Set the range of the joint
     * @param[in] i_rangeVector Vector<double> 2 entries, start and end of the range of the joint
     */
    void setRange(const std::vector<double>& i_rangeVector);

    /**
     * Convert the direction the joint moves to the direction the motor has to move.
     * @param[in] i_direction direction the joint has to move
     * @param[out] direction the motor has to move to do it
     */
    const std::string convertDirection(const std::string i_direction)const;

    /**
     * Method to give acces to the motor of the joint.
     * Can be used as a left and right argument
     */
    virtual BaseMotor* getMotor() = 0;
    /*
      This doesnt work becaus the basemotor pointer is not in the heap so will be lost.
      Using a normal variable doesnt work either, because it cannot be cast to other motors
      virtual void setMotor(const BaseMotor*) = 0;
    */

    /**
     * Predict a step
     * method calls predictSteps with number of steps = 1
     * @param[in/out] RobotPosition
     * @param[in] Direction of movement
     */
    virtual void predictStep(Point2D& io_currentPosition,
                             const std::string& i_direction);
    /**
     * Predict a step
     * method calls predictSteps with number of steps = 1
     * @param[in/out] RobotPosition
     * @param[in] Direction of movement
     * @param[in] i_number number of steps
     */
    virtual void predictSteps(Point2D& io_robotPosition,
                              const std::string& i_direction,
                              const int& i_number) = 0;

    /**
     * Method to clone the joint and return a smart point to the cloned BaseJoint
     * Method call cloneImpl() which is pure virtual method.
     * implemented by derived classes.
     */
    std::shared_ptr<BaseJoint> clone() const {
      return std::shared_ptr<BaseJoint>( this->cloneImpl() ); }

    BaseJoint();
    
    BaseJoint(const double& i_currentPosition,
              const double& i_movementPerStep);

    BaseJoint(const double& i_currentPosition,
              const double& i_movementType,
              const DirectionConversionMap& i_directionConverionMap);

    BaseJoint(const double& i_currentPosition,
              const double& i_movementPerStep,
              const std::vector<double> i_rangeVector,
              const MovementType& i_movementType,
              const DirectionConversionMap& i_directionConverionMap);

    virtual ~BaseJoint(){};
};
#endif  // MOTOR_BASEJOINT_H_
