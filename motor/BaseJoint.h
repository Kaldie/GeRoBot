// copyright [2015] Ruud Cools

#ifndef MOTOR_BASEJOINT_H_
#define MOTOR_BASEJOINT_H_

class Point2D;
class SequenceVector;
class BaseMotor;

class BaseJoint {
    GETSET(double, m_currentPosition, Position)
    GETSET(double, m_movementPerStep, MovementPerStep)
    GET(std::vector<double>, m_range, Range)
    GETSET(MovementType, m_movementType, MovementType)
    GETSET(DirectionConversionMap,
           m_directionConversion,
           DirectionConversionMap)
 public:
    typedef std::shared_ptr<BaseJoint> JointPointer;

    /**
     * Set the range of the joint
     * @param[in] i_rangeVector Vector<double> 2 entries, start and end of the range of the joint
     */
    void setRange(const std::vector<double>& i_rangeVector);

    /**
     * Convert the direction the joint moves to the direction the motor has to move.
     * @param[in] i_direction direction the joint has to move
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
     * Set actual steps
     * This function updates the position of the joint
     * Call this function when you have determined
     * that setting the step is a great idea and want to actualise it
     */
    void moveSteps(const std::string& i_directionString,
                   const int& i_numberOfSteps,
                   SequenceVector* i_vector);

    /**
     * Predict a step
     * method calls predictSteps with number of steps = 1
     * @param[in] io_robotPosition Current position of the robot
     * @param[in] i_direction Direction of movement
     * @param[in] i_number number of steps
     */
    // Actual methods!
    virtual void predictSteps(Point2D* io_robotPosition,
                              const std::string& i_direction,
                              const int& i_number) const = 0;
    /**
     * Method to clone the joint and return a smart point to the cloned BaseJoint
     * Method call cloneImpl() which is pure virtual method.
     * implemented by derived classes.
     */
    std::shared_ptr<BaseJoint> clone() const {
      return std::shared_ptr<BaseJoint>( this->cloneImpl() ); }

    /// default constructor
    BaseJoint();
    /// constructor with position and step size
    BaseJoint(const double& i_currentPosition,
              const double& i_movementPerStep);
    /// Constructor with position, step size and direction conversion map
    BaseJoint(const double& i_currentPosition,
              const double& i_movementPerStep,
              const DirectionConversionMap& i_directionConverionMap);
    /// fully fledged constructor
    BaseJoint(const double& i_currentPosition,
              const double& i_movementPerStep,
              const std::vector<double> i_rangeVector,
              const MovementType& i_movementType,
              const DirectionConversionMap& i_directionConverionMap);
    /// Mandatory virtual destructor
    virtual ~BaseJoint(){};
 protected:
    /// Pure virtual implementation of the clone implementation
    virtual BaseJoint* cloneImpl() const = 0;
    /// Is in range of the joint.
    void isInRange(double i_jointPosition) const;
 private:
    /// Given a direction, give the modifier: does the position in or descrease
    virtual int getPositionModifier(const std::string&) const = 0;
};
#endif  // MOTOR_BASEJOINT_H_
