# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#


TOROOT= ..

# define the C source files
DSRCS= debug.cpp debug2.cpp
SRCS := $(filter-out $(DSRCS), $(wildcard *.cpp))

LIBS=-lMathInterface -lMotorInterface -lArduinoInterface

# define the executable file 
MAIN = RobotInterface
DEBUG = debug
include ../Makefile.config
OBJS := $(OBJS) $(TOROOT)/$(OBJECTFOLDER)/pugixml.o
