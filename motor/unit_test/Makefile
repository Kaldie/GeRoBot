# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#


TOROOT= ../..

# define the C source files
DSRCS= ../debug.cpp
UnitTests = $(wildcard *.h)
DEPENDENCY := $(patsubst %.cpp, %.o, $(foreach dep, $(filter-out $(DSRCS), $(wildcard ../*.cpp)), $(notdir $(dep))))
DEPENDENCY := $(DEPENDENCY) ArduinoSerialConnection.o ArduinoMotorDriver.o Point2D.o Vector2D.o Trace.o #submodule dependency
DEPENDENCY := $(DEPENDENCY) BaseTraceCalculator.o Line2D.o LineTraceCalculator.o RotationTraceCalculator.o RotationTrace.o Circle2D.o Arc2D.o XMLBuilder.o pugixml.o RobotIO.o JointControllerIO.o JointIO.o StepperDriverIO.o
DEPENDENCY := $(DEPENDENCY) Quadrant2D.o SpeedControllerIO.o EndStopIO.o
#LIBS=-lMathInterface -lArduinoInterface
#LIBS = -lArduinoInterface -lMathInterface
include $(TOROOT)/Makefile.config
.DEFAULT_GOAL := unitTest
