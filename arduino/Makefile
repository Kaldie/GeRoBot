# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

#Folder dependend configuration
TOROOT= ..

# define the C source files
DSRCS= debug2.cpp
SRCS := $(filter-out $(DSRCS), $(wildcard *.cpp))
#dependencies of other modules
DEPENDENCY = $(filter-out $(DSRCS), $(notdir $(wildcard $(TOROOT)/$(OBJECTFOLDER)/*.o)))
# define the executable file 
MAIN = ArduinoInterface
DEBUG = debug

include $(TOROOT)/Makefile.config
DEPENDENCY := $(filter-out $(DOBJS), $(DEPENDENCY))
