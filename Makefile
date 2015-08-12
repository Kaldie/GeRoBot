# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#



# Main makefile
TOROOT= $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
PUGISRC = ./pugixml/src
SHAREDFOLDERS = ./math ./arduino ./motor ./motor/io ./util/xml $(PUGISRC)
UPLOADEDFOLDERS = ./arduinosketch
MAIN = GeoRobot
include Makefile.config

OBJS := $(wildcard bin/objects/*.o)

.PHONY: CLEAN ALL INSTALL CREATENECESSARYFOLDERS LINKHEADERS clean MACROHEADER PUGI

ALL: CREATENECESSARYFOLDERS PUGI LINKHEADERS MACROHEADER SHAREDTARGET MACROHEADER SHAREDTARGET UPLOADEDTARGET

CREATENECESSARYFOLDERS:
	@echo "Creating folders for objects and shared libs"
	$(MKDIR_P) $(INCLUDEFOLDER)
	$(MKDIR_P) $(OBJECTFOLDER)
	$(MKDIR_P) $(LIBFOLDER)
	$(MKDIR_P) $(DEPDIR)

MACROHEADER: $(TOROOT)/$(INCLUDEFOLDER)/macroHeader.h.gch
	@echo "yay"

LINKHEADERS:
	@echo "Creating symbolic links for all .h files in the project"
	$(foreach FOLDER,$(SHAREDFOLDERS), $(foreach HFILE,$(wildcard $(FOLDER)/*.h),$(LINK) .$(HFILE) $(INCLUDEFOLDER);))
	$(foreach FOLDER,$(SHAREDFOLDERS), $(foreach HFILE,$(wildcard $(FOLDER)/*.hpp),$(LINK) .$(HFILE) $(INCLUDEFOLDER);))

SHAREDTARGET:
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); $(MAKE); cd $(TOROOT);)
	$(MAKE) shared

UNITTEST:
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); cd unit_test; $(MAKE) unitTest; cd ../..;)

PUGI:
	git submodule init
	git submodule update
	git submodule foreach 'git fetch origin; git checkout $(git rev-parse --abbrev-ref HEAD); git reset --hard origin/$$(git rev-parse --abbrev-ref HEAD); git submodule update --recursive; git clean -dfx'

UPLOADEDTARGET:
	$(foreach FOLDER,$(UPLOADEDFOLDERS), cd $(FOLDER); make -i upload; cd ..; )

CLEAN:
	$(foreach FOLDER,$(UPLOADEDFOLDERS), cd $(FOLDER); make clean; cd $(TOROOT);)
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); make clean; cd $(TOROOT);)
	$(RM) $(INCLUDEFOLDER)/macroHeader.h.gch

#clean:
#	@echo "Dumb fuck...no clean in the root or it will be like realy clean!"

.DEFAULT_GOAL := SHAREDTARGET
# End of the main math makefile

