# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# Main math makefile

PROG = math
TRGTS = MATH ARDUINO ARDUINOSKETCH MOTOR PUGIXML ROBOT

$(PROG): $(TRGTS)

all: $(TRGTS)

MATH:
	cd ./math ; make shared

MOTOR:
	cd ./motor ; make shared

ARDUINO:
	cd ./arduino ; make shared

ARDUINOSKETCH:
	cd ./arduinosketch ; make -i upload

ROBOT:
	cd ./robot ; make shared

PUGIXML:
	cd ./pugixml-1.4 ; make shared

clean:
	rm -f *.o *~
	cd ./motor ; make clean
	cd ./math ; make clean
	cd ./arduino ; make clean
	cd ./arduinosketch ; make clean
	cd ./robot ; make clean

# End of the main math makefile

