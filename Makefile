# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#



# Main math makefile

SHAREDFOLDERS = ./math ./arduino ./motor ./robot
UPLOADEDFOLDERS = ./arduinosketch

.PHONY: CLEAN ALL INSTALL CREATENECESSERYFOLDERS LINKHEADERS clean


ALL: SHAREDTARGET UPLOADEDTARGET

INSTALL: CREATENECESSERYFOLDERS LINKHEADERS SHAREDTARGET UPLOADEDTARGET

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
  
CLEAN:
	$(foreach FOLDER,$(UPLOADEDFOLDERS), cd $(FOLDER); make clean; cd ..;)
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); make clean; cd ..;)

include Makefile.config

clean:
	@echo "Dumb fuck...no clean in the root or it will be like realy clean!"

# End of the main math makefile

