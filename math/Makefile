# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#


TOROOT= ..

# define the C source files
DSRCS= debug.cpp
SRCS := $(filter-out $(DSRCS), $(wildcard *.cpp))


#LIBS=-lMathInterface -lArduinoInterface
LIBS = -lMotorInterface
# define the executable file 
MAIN = MathInterface
DEBUG = debug

include ../Makefile.config

