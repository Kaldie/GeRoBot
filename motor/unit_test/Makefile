# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#


TOROOT= ../..

# define the C source files
UnitTests = PinStateUnitTest.h PinStateSequenceUnitTest.h StepperDriverUnitTest.h
UnitTests := $(UnitTests) RotationalJointUnitTest.h TranslationalJointUnitTest.h
UnitTests := $(UnitTests) JointControllerUnitTest.h

#LIBS=-lMathInterface -lArduinoInterface
LIBS = -lArduinoInterface -lMotorInterface -lMathInterface
include $(TOROOT)/Makefile.config

