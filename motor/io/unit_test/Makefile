
TOROOT= ../../..

# define the C source files
DSRCS= ../debug.cpp
UnitTests = $(wildcard *.h)
DEPENDENCY := $(patsubst %.cpp, %.o, $(foreach dep, $(filter-out $(DSRCS), $(wildcard ../*.cpp)), $(notdir $(dep))))
DEPENDENCY := $(DEPENDENCY) ArduinoSerialConnection.o ArduinoMotorDriver.o Point2D.o Vector2D.o Trace.o #submodule dependency
DEPENDENCY := $(DEPENDENCY) BaseTraceCalculator.o Line2D.o
#LIBS=-lMathInterface -lArduinoInterface
#LIBS = -lArduinoInterface -lMathInterface
include $(TOROOT)/Makefile.config
.DEFAULT_GOAL := unitTest
