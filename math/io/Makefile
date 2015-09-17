# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#


TOROOT= ../..

# Debug source file
DSRCS= debug.cpp
# get all the source except debug source
SRCS := $(filter-out $(DSRCS), $(wildcard *.cpp))

#debug dependencies of other modules
DEPENDENCY = $(filter-out $(DOBJS), $(notdir $(wildcard $(TOROOT)/$(OBJECTFOLDER)/*.o)))

# define the executable file 
DEBUG = debug
include $(TOROOT)/Makefile.config
DEPENDENCY := $(filter-out $(DOBJS), $(DEPENDENCY))
