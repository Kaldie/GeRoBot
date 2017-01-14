# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#


TOROOT= ../..

# define the C source files
DSRCS= debug.cpp
SRCS := $(filter-out $(DSRCS), $(wildcard *.cpp))

#dependencies of other modules
DEPENDENCY = ArduinoSerialConnection.o ArduinoMotorDriver.o Point2D.o Vector2D.o

# define the executable file 
MAIN = MotorInterface
DEBUG = debug

include $(TOROOT)/Makefile.config

