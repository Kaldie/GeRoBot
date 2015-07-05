# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#



# Main math makefile
TOROOT=.
PUGISRC = ./pugixml/src
SHAREDFOLDERS = ./math ./arduino ./motor ./robot $(PUGISRC)
UPLOADEDFOLDERS = ./arduinosketch
include Makefile.config

.PHONY: CLEAN ALL INSTALL CREATENECESSARYFOLDERS LINKHEADERS clean MACROHEADER

ALL: INSTALL MACROHEADER SHAREDTARGET UPLOADEDTARGET

INSTALL: CREATENECESSARYFOLDERS LINKHEADERS MACROHEADER SHAREDTARGET UPLOADEDTARGET

CREATENECESSARYFOLDERS:
	@echo "Creating folders for objects and shared libs"
	$(MKDIR_P) $(INCLUDEFOLDER)
	$(MKDIR_P) $(OBJECTFOLDER)
	$(MKDIR_P) $(LIBFOLDER)

MACROHEADER: $(TOROOT)/$(INCLUDEFOLDER)/macroHeader.h.gch
	@echo "yay"

LINKHEADERS:
	@echo "Creating symbolic links for all .h files in the project"
	$(foreach FOLDER,$(SHAREDFOLDERS), $(foreach HFILE,$(wildcard $(FOLDER)/*.h),$(LINK) .$(HFILE) $(INCLUDEFOLDER); ))
	$(foreach FOLDER,$(SHAREDFOLDERS), $(foreach HFILE,$(wildcard $(FOLDER)/*.hpp),$(LINK) .$(HFILE) $(INCLUDEFOLDER); ))

SHAREDTARGET:
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); make shared; cd ..; )

UNITTEST:
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); cd unit_test; make unitTest; cd ../..;)

PUGI:
	SRCS := $(filter-out $(DSRCS), $(wildcard $(PUGISRC)/*.cpp))
	shared

UPLOADEDTARGET:
	$(foreach FOLDER,$(UPLOADEDFOLDERS), cd $(FOLDER); make -i upload; cd ..; )

CLEAN:
	$(foreach FOLDER,$(UPLOADEDFOLDERS), cd $(FOLDER); make clean; cd ..;)
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); make clean; cd ..;)
	$(RM) macroHeader.h.gch

clean:
	@echo "Dumb fuck...no clean in the root or it will be like realy clean!"

# End of the main math makefile

