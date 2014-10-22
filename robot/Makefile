# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

TOROOT= ..

# define the C source files
SRCS=Robot.cpp XMLBuilder.cpp RobotBuilder.cpp JointControllerBuilder.cpp JointBuilder.cpp
SRCS:=$(SRCS) StepperDriverBuilder.cpp
DSRCS= debug.cpp

LIBS=-lMathInterface -lMotorInterface -lArduinoInterface -lPugiXML

# define the executable file 
MAIN = RobotInterface
DEBUG = debug

include ./../Makefile.config