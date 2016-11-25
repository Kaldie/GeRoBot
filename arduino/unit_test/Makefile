# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#


TOROOT= ../..

# define the C source files
DSRCS= ../debug.cpp ../debug2.cpp
UnitTests = $(wildcard *.h)
DEPENDENCY := $(patsubst %.cpp, %.o, $(foreach dep, $(filter-out $(DSRCS), $(wildcard ../*.cpp)), $(notdir $(dep))))
DEPENDENCY := $(DEPENDENCY) Point2D.o Vector2D.o Trace.o RobotIO.o Robot.o SequenceVector.o StateSequence.o PinState.o#submodule dependency
DEPENDENCY := $(DEPENDENCY) RotationTrace.o Arc2D.o Circle2D.o
DEPENDENCY := $(DEPENDENCY) XMLBuilder.o pugixml.o JointController.o JointControllerIO.o SpeedController.o RotationTraceCalculator.o LineTraceCalculator.o
DEPENDENCY := $(DEPENDENCY) BaseTraceCalculator.o Line2D.o JointIO.o StepperDriver.o StepperDriverIO.o BaseMotor.o BaseJoint.o Condensor.o ConstantSpeedController.o
#LIBS=-lMathInterface -lArduinoInterface
#LIBS = -lArduinoInterface -lMathInterface
include $(TOROOT)/Makefile.config
.DEFAULT_GOAL := unitTest
