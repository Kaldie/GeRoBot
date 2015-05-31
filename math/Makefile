# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

#Folder dependend configuration
TOROOT= ..

# define the C source files
SRCS= Vector2D.cpp Point2D.cpp Line2D.cpp Circle2D.cpp Arc2D.cpp
SRCS:=$(SRCS) Trace.cpp RotationTrace.cpp
SRCS:=$(SRCS) BaseTraceCalculator.cpp LineTraceCalculator.cpp RotationTraceCalculator.cpp 
DSRCS= debug.cpp

# define the executable file 
MAIN = MathInterface
DEBUG= debug

LIBS=-lMotorInterface


include ../Makefile.config
