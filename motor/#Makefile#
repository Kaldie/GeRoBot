# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#


TOROOT= ..

# define the C source files
SRCS= PinState.cpp PinStateSequence.cpp
SRCS:=$(SRCS) BaseMotor.cpp StepperDriver.cpp #StepperMotor.cpp
SRCS:=$(SRCS) BaseJoint.cpp
SRCS:=$(SRCS) JointController.cpp
DSRCS= debug.cpp

#LIBS=-lMathInterface -lArduinoInterface
LIBS = -lArduinoInterface
# define the executable file 
MAIN = MotorInterface
DEBUG = debug

include ../Makefile.config

