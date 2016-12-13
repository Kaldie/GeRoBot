#include <macroHeader.h>
#include <EndStop.h>
#include <BaseJoint.h>
#include <Point2D.h>


EndStop::EndStop()
  : EndStop(BaseJoint::JointPointer(),
	    0.0,
	    0) {
}


EndStop::EndStop(const BaseJoint::JointPointer& i_joint,
		 const traceType& i_position)
  : EndStop(i_joint, i_position, 0) {
}


EndStop::EndStop(const BaseJoint::JointPointer& i_joint,
		 const traceType& i_position,
		 const int& i_pinNumber)
  : m_joint(i_joint),
    m_position(i_position),
    m_pinNumber(i_pinNumber) {
}
