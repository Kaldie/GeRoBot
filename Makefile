# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

include Makefile.config

# Main math makefile

SHAREDFOLDERS = ./math #/arduino ./motor ./robot
UPLOADEDFOLDERS = #/arduinosketch

.PHONY: clean install CREATENECESSERYFOLDERS LINKHEADERS


all: SHAREDTARGET UPLOADEDTARGET

install: CREATENECESSERYFOLDERS LINKHEADERS

CREATENECESSERYFOLDERS:
	@echo "Creating folders for objects and shared libs"
	$(MKDIR_P) $(INCLUDEFOLDER)
	$(MKDIR_P) $(OBJECTFOLDER)
	$(MKDIR_P) $(LIBFOLDER)

LINKHEADERS:
	@echo "Creating symbolic links for all .h files in the project"
	$(foreach FOLDER,$(SHAREDFOLDERS), $(foreach HFILE,$(wildcard $(FOLDER)/*.h),$(LINK) .$(HFILE) $(INCLUDEFOLDER); ))

SHAREDTARGET:
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); make shared; cd ..; )

UPLOADEDTARGET:
	$(foreach FOLDER,$(UPLOADEDFOLDERS), cd $(FOLDER); make -i upload; cd ..; )
  
clean:
	rm -f *.o *~
	$(foreach FOLDER,$(UPLOADEDFOLDERS), cd $(FOLDER); make clean; cd ..;)
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); make clean; cd ..;)

# End of the main math makefile

