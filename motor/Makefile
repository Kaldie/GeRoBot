# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#


TOROOT= ..

# define the C source files
SRCS= PinState.cpp
SRCS:=$(SRCS) BaseMotor.cpp StepperDriver.cpp #StepperMotor.cpp
SRCS:=$(SRCS) BaseJoint.cpp
SRCS:=$(SRCS) JointController.cpp
SRCS:=$(SRCS) Trace.cpp RotationTrace.cpp
SRCS:=$(SRCS) BaseTraceCalculator.cpp LineTraceCalculator.cpp 
DSRCS= debug.cpp

LIBS=-lMathInterface -lArduinoInterface

# define the executable file 
MAIN = MotorInterface
DEBUG = debug

include ../Makefile.config

