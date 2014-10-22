# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# Main math makefile

SHAREDFOLDERS = ./math ./arduino ./motor ./pugixml-1.4  ./robot
UPLOADEDFOLDERS = ./arduinosketch

all: SHAREDTARGET UPLOADEDTARGET

INCLUDEDIR:
	$(foreach FOLDER,$(FOLDERS), cd $(FOLDER); make include; )
  
SHAREDTARGET:
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); make shared;)

UPLOADEDTARGET:
	$(foreach FOLDER,$(UPLOADEDFOLDERS), cd $(FOLDER); make -i upload; )
  
clean:
	rm -f *.o *~
	$(foreach FOLDER,$(UPLOADEDFOLDERS), cd $(FOLDER); make clean;)
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); make clean;)

# End of the main math makefile

