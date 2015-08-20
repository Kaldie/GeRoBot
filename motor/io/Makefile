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
DEPENDENCY = $(filter-out $(DOBJS), $(notdir $(wildcard $(TOROOT)/$(OBJECTFOLDER)/*.o)))

# define the executable file 
DEBUG = debug

include $(TOROOT)/Makefile.config

DEPENDENCY := $(filter-out $(DOBJS), $(DEPENDENCY))

