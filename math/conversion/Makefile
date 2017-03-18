# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

TOROOT= ../..

# define the C source files
DSRCS= debug.cpp
SRCS := $(filter-out $(DSRCS), $(wildcard *.cpp))

include $(TOROOT)/Makefile.config

