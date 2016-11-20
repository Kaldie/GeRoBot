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
    GETSETPROTECTED(WeakJointPointer, m_child, Child);

    // Pointer to the parent of this Joint
    GETSETPROTECTED(WeakJointPointer, m_parent, Parent);

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
     * Updates the Sequencevector so when actuate is called it does this
     */
    void moveSteps(const std::string& i_directionString,
                   const int& i_numberOfSteps,
                   SequenceVector* i_vector);

    /**
     * Predict the final position after setting steps
     * @param[in] io_currentPosition current position of the robot
     * @param[in] i_direction Direction which the joint will move
     * @param[in] i_number Number of steps to be taken
     */
    virtual void predictSteps(Point2D* io_currentPosition,
                              const std::string& i_direction,
                              const int& i_number) const = 0;

    /// Return the distance traveled with one step
    virtual traceType distancePerStep() const = 0;

    /**
     * Method to clone the joint and return a smart point to the cloned BaseJoint
     * Method call cloneImpl() which is pure virtual method.
     * implemented by derived classes.
     */
    std::shared_ptr<BaseJoint> clone() const {
      return std::shared_ptr<BaseJoint>( this->cloneImpl() ); }

    /// from this joint, walk down the tree and get the extension of the farthest child
    const Point2D childPosition() const;

    /// default constructor
    BaseJoint();

    /// Mandatory virtual destructor
    virtual ~BaseJoint(){};

 protected:
    /// Pure virtual implementation of the clone implementation
    virtual BaseJoint* cloneImpl() const = 0;

    /// Is in range of the joint.
    void isInRange(traceType i_jointPosition) const;

    // return the angle and position of the parent
    void parentPosition(Point2D* o_position, traceType* o_angle) const;

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


    void childPosition(Point2D* o_position, traceType* o_angle) const;
};
#endif  // MOTOR_BASEJOINT_H_
