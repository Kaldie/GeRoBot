// Copyright [2015] Ruud Cools

#ifndef MOTOR_BASEJOINT_H_
#define MOTOR_BASEJOINT_H_

class Point2D;
class SequenceVector;
class BaseMotor;
class PinState;
class EndStop;

class BaseJoint {
 public:
    // shared pointer type def
    typedef std::shared_ptr<BaseJoint> JointPointer;

    // weak pointer type def
    typedef std::weak_ptr<BaseJoint> WeakJointPointer;

    // vector of end stops
    typedef std::vector<std::shared_ptr<EndStop>> EndStopVector;

    // declaration of the movement type
    enum MovementType : short {None, Rotational, Translational};

    /// Current position of the joint
    GETSETPROTECTED(traceType, m_currentPosition, Position);

    // Movement of the joint per step
    GETSETPROTECTED(traceType, m_movementPerStep, MovementPerStep);

    // Range in which the joint can move
    GET(std::vector<traceType>, m_range, Range);

    // Type of the movement of the Joint
    GETSET(MovementType, m_movementType, MovementType);

    /// Direction conversion map of the joint
    GETSET(DirectionConversionMap,
	   m_directionConversion,
	   DirectionConversionMap);

    // Pointer to the child of this joint
    GETSETPROTECTED(WeakJointPointer, m_child, Child);

    // Pointer to the parent of this Joint
    GETSETPROTECTED(WeakJointPointer, m_parent, Parent);

    /// shared pointer to motor
    GETSETPROTECTED(std::shared_ptr<BaseMotor>, m_motor, Motor);

    //End points
    GETSET(EndStopVector, m_endStops, EndStops);

 public:
    /**
     * Set the range of the joint
     * @param[in] i_rangeVector Vector<traceType> 2 entries, start and end of the range of the joint
     */
    void setRange(const std::vector<traceType>& i_rangeVector);

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

    bool getJointStatus(const PinState& i_pinState,
			std::string* i_direction) const;

    virtual const std::shared_ptr<EndStop> getEndStop
      (const PinState& i_pinState, const std::string& i_direction) const;

    virtual void updateJointOnEndStopHit(const std::shared_ptr<EndStop> i_endStop);

    BaseJoint(const std::shared_ptr<BaseMotor>& i_motor);
    
    /// Constructor which is kind of nice
    BaseJoint(const std::shared_ptr<BaseMotor>& i_motor,
	      const traceType& i_currentPosition,
	      const traceType& i_movementPerStep,
	      const MovementType i_type,
	      const DirectionConversionMap& i_conversionMap);

    /// Full fledged constructor
    BaseJoint(const std::shared_ptr<BaseMotor>& i_motor,
	      const traceType& i_currentPosition,
	      const traceType& i_movementPerStep,
	      const std::vector<traceType>& i_rangeVector,
	      const MovementType i_type,
	      const DirectionConversionMap& i_conversionMap,
	      const WeakJointPointer& i_child,
	      const WeakJointPointer& i_parent);

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
    
    /// no default constructor
    BaseJoint();
};
#endif  // MOTOR_BASEJOINT_H_
