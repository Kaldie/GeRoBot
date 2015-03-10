# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

#Folder dependend configuration
TOROOT= ..

# define the C source files
SRCS=ArduinoMotorDriver.cpp
SRCS:= $(SRCS) ArduinoSerialConnection.cpp
DSRCS= debug2.cpp

# define the executable file 
MAIN = ArduinoInterface
DEBUG = debug

include ../Makefile.config
