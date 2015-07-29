# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#


TOROOT= ../..

# define the C source files
SRCS := $(filter-out $(DSRCS), $(wildcard *.cpp))

#dependencies of other modules
DEPENDENCY = 

# define the executable file 
MAIN = XML
DEBUG = debug

include $(TOROOT)/Makefile.config

