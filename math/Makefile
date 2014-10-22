# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

include ../Makefile.config

# define the C source files
SRCS= Vector2D.cpp Point2D.cpp Line2D.cpp
#SRCS:=$(SRCS) BaseTraceCalculator.cpp LineTraceCalculator.cpp #TraceCalculator.cpp
DSRCS= debug.cpp

OBJS = $(SRCS:.cpp=.o)
DOBJS= $(DSRCS:.cpp=.o)
OBJECTFOLDER = ../bin/objects/
LIBFOLDER= ../bin/lib/
CURRENTPATHFROMLIBFOLDER=../../math

unexport OBJECTFOLDER LIBFOLDER

# define the executable file 
MAIN = MathInterface
DEBUG= debug

#	
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#
.PHONY: depend all debug shared

all:    $(OBJS)
	@echo "Compiling all object files for this dir!"

debug: $(OBJS) $(DOBJS)
	@echo "Creating executable for debug purposes"
	$(CC) $(CFLAGS) $(INCLUDES) -o $(DEBUG) $(OBJS) $(DOBJS) $(LFLAGS) $(LIBS)

shared: $(OBJS)
	@echo "creating shared object lib for actuators"
	$(CC) $(CFLAGS) $(INCLUDES) -shared -o lib$(MAIN).so $(OBJS)  $(LFLAGS) $(LIBS)
	cd $(LIBFOLDER); $(LINK) $(CURRENTPATHFROMLIBFOLDER)/lib$(MAIN).so

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

