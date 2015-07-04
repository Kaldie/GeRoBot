# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#


TOROOT= ../..

# define the C source files
DSRCS= ../debug.cpp
SRCS := $(filter-out $(DSRCS), $(wildcard ../*.cpp))

UnitTests = $(wildcard *.h)

#LIBS=-lMathInterface -lArduinoInterface
LIBS = -lArduinoInterface -lMathInterface
include $(TOROOT)/Makefile.config

.DEFAULT_GOAL := unitTest
