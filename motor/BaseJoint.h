// copyright [2015] Ruud Cools

#ifndef MOTOR_BASEJOINT_H_
#define MOTOR_BASEJOINT_H_

class Point2D;
class SequenceVector;
class BaseMotor;


class BaseJoint {
 public:
    // shared pointer type def
    typedef std::shared_ptr<BaseJoint> JointPointer;
    // weak pointer type def
    typedef std::weak_ptr<BaseJoint> WeakJointPointer;
    // declaration of the movement type
    enum MovementType {None, Rotational, Translational};

    /// Current position of the joint
    GETSETPROTECTED(traceType, m_currentPosition, Position);

    // Movement of the joint per step
    GETSETPROTECTED(traceType, m_movementPerStep, MovementPerStep);

    // Range in which the joint can move
    GET(std::vector<traceType>, m_range, Range);

    // Type of the movement of the Joint
    GETSET(MovementType, m_movementType, MovementType);

    // Direction conversion map of the joint
    GETSET(DirectionConversionMap,
           m_directionConversion,
           DirectionConversionMap);

    // Pointer to the child of this joint
    GETSETPROTECTED(JointPointer, m_child, Child);

    // pointer to the parent of this joint
    GETSETPROTECTED(JointPointer, m_parent, Parent);

 public:
    /**
     * Set the range of the joint
     * @param[in] i_rangeVector Vector<traceType> 2 entries, start and end of the range of the joint
     */
    void setRange(const std::vector<traceType>& i_rangeVector);

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
     * Predict the final position after setting steps
     * @param[in] io_robotPosition position of the robot
     * @param[in] i_direction Direction which the joint will move
     * @param[in] i_number Number of steps to be taken
     */
    virtual void predictSteps(Point2D* io_robotPosition,
                              const std::string& i_direction,
                              const int& i_number) = 0;

    /// Return the distance traveled with one step
    virtual traceType distancePerStep() const = 0;

    /**
     * Method to clone the joint and return a smart point to the cloned BaseJoint
     * Method call cloneImpl() which is pure virtual method.
     * implemented by derived classes.
     */
    std::shared_ptr<BaseJoint> clone() const {
      return std::shared_ptr<BaseJoint>( this->cloneImpl() ); }

    /// default constructor
    BaseJoint();

    /// Mandatory virtual destructor
    virtual ~BaseJoint(){};

 protected:
    /// Pure virtual implementation of the clone implementation
    virtual BaseJoint* cloneImpl() const = 0;

    /// Is in range of the joint.
    void isInRange(traceType i_jointPosition) const;

    // return child position
    Point2D childPosition() const;

    void childPosition(Point2D* o_position, traceType* o_angle) const;

    // return length of the Joint
    virtual traceType getLength() const = 0;

    // return the angle of the Joint
    virtual traceType getAngle() const = 0;

 private:
    /// Given a direction, give the modifier: does the position in or descrease
    virtual int getPositionModifier(const std::string&) const = 0;

    /**
     * Convert the direction the joint moves to the direction the motor has to move.
     * @param[in] i_direction direction the joint has to move
     */
    const std::string convertDirection(const std::string i_direction)const;
};
#endif  // MOTOR_BASEJOINT_H_
