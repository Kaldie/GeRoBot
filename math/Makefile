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


# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
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
.PHONY: depend clean all debug shared

all:    $(OBJS)
	@echo "Compiling all object files for this dir!"

debug: $(OBJS) $(DOBJS)
	@echo "Creating executable for debug purposes"
	$(CC) $(CFLAGS) $(INCLUDES) -o $(DEBUG) $(OBJS) $(DOBJS) $(LFLAGS) $(LIBS)

shared: $(OBJS)
	@echo "creating shared object lib for actuators"
	$(CC) $(CFLAGS) $(INCLUDES) -shared -o lib$(MAIN).so $(OBJS)  $(LFLAGS) $(LIBS)
	cd $(LIBFOLDER); $(LINK) $(CURRENTPATHFROMLIBFOLDER)/lib$(MAIN).so

clean:
	$(RM) *.o
	$(RM) *.so
	$(RM) *$(MAIN)*
	$(RM) $(DEBUG)
	$(RM) $(addprefix $(OBJECTFOLDER),$(OBJS))
	$(RM) $(addprefix $(LIBFOLDER),*$(MAIN)*)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@
	$(LINK) $@ $(OBJECTFOLDER)$@

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

