
TOROOT= ../../..

# define the C source files
DSRCS= ../debug.cpp
UnitTests = $(wildcard *.h)
DEPENDENCY := $(patsubst %.cpp, %.o, $(foreach dep, $(filter-out $(DSRCS), $(wildcard ../*.cpp)), $(notdir $(dep))))
DEPENDENCY := $(DEPENDENCY) ArduinoSerialConnection.o ArduinoMotorDriver.o Point2D.o Vector2D.o Trace.o #submodule dependency
DEPENDENCY := $(DEPENDENCY) BaseTraceCalculator.o Line2D.o Robot.o XMLBuilder.o pugixml.o SequenceVector.o
DEPENDENCY := $(DEPENDENCY) JointController.o BaseJoint.o StateSequence.o PinState.o Condensor.o
DEPENDENCY := $(DEPENDENCY) StepperDriver.o BaseMotor.o SpeedController.o ConstantSpeedController.o
DEPENDENCY := $(DEPENDENCY) LineTraceCalculator.o RotationTraceCalculator.o RotationTrace.o Circle2D.o Arc2D.o EndStop.o
DEPENDENCY := $(DEPENDENCY) PrescribedSpeedController.o RelativeSpeedController.o
#LIBS=-lMathInterface -lArduinoInterface
#LIBS = -lArduinoInterface -lMathInterface
include $(TOROOT)/Makefile.config
.DEFAULT_GOAL := unitTest
