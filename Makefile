# Main makefile
TOROOT= $(patsubst %/,%,$(abspath $(dir $(lastword $(MAKEFILE_LIST)))))
PUGISRC = pugixml/src
SHAREDFOLDERS = $(TOROOT)/math $(TOROOT)/math/io $(TOROOT)/arduino $(TOROOT)/motor $(TOROOT)/motor/io $(TOROOT)/util/xml
UPLOADEDFOLDERS = ./arduinosketch
MAIN = GeRobot
include Makefile.config

OBJS := $(wildcard bin/objects/*.o)

.PHONY: CLEAN ALL INSTALL CREATENECESSARYFOLDERS LINKHEADERS clean MACROHEADER PUGI CXX

ALL: CREATENECESSARYFOLDERS PUGI CXX LINKHEADERS MACROHEADER SHAREDTARGET MACROHEADER SHAREDTARGET UPLOADEDTARGET

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
	@echo $(SHAREDFOLDERS)
	$(LINK) ../$(MACROHEADER) $(INCLUDEFOLDER)
	$(foreach FOLDER,$(SHAREDFOLDERS), $(foreach HFILE,$(wildcard $(FOLDER)/*.h),$(LINK) .$(HFILE) $(INCLUDEFOLDER);))
	$(foreach FOLDER,$(SHAREDFOLDERS), $(foreach HFILE,$(wildcard $(FOLDER)/*.hpp),$(LINK) .$(HFILE) $(INCLUDEFOLDER);))

SHAREDTARGET:
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); $(MAKE);)
	$(MAKE) shared

UNITTEST:
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); cd unit_test; $(MAKE) unitTest; cd ../..;)

PUGI:
	git submodule init
	git submodule update
	git submodule foreach 'git fetch origin; git checkout $(git rev-parse --abbrev-ref HEAD); git reset --hard origin/$$(git rev-parse --abbrev-ref HEAD);     git submodule update --recursive; git clean -dfx'
	$(foreach HFILE,$(wildcard $(PUGISRC)/*.hpp),$(LINK) ../$(HFILE) $(INCLUDEFOLDER);)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(PUGISRC)/pugixml.cpp -o $(TOROOT)/$(OBJECTFOLDER)/pugixml.o

CXX:
	wget -O $(TOROOT)/cxx.tar.gz 'http://sourceforge.net/projects/cxxtest/files/latest/download?source=files'
	$(MKDIR_P) $(CXX)
	tar -xvzf $(TOROOT)/cxx.tar.gz --strip=1 -C $(CXX)
	$(MKDIR_P) $(INCLUDEFOLDER)/cxxtest
	$(foreach HFILE,$(wildcard $(CXXSRC)/*.h),$(LINK) ../../$(HFILE) $(INCLUDEFOLDER)/cxxtest;)
	$(foreach HFILE,$(wildcard $(CXXSRC)/*.h),$(LINK) ../../$(HFILE) $(INCLUDEFOLDER)/cxxtest;)



UPLOADEDTARGET:
	$(foreach FOLDER,$(UPLOADEDFOLDERS), cd $(FOLDER); make -i upload; cd ..; )

CLEAN:
	$(foreach FOLDER,$(UPLOADEDFOLDERS), cd $(FOLDER); make clean; cd $(TOROOT);)
	$(foreach FOLDER,$(SHAREDFOLDERS), cd $(FOLDER); make clean; cd $(TOROOT);)
	$(RM) $(INCLUDEFOLDER)/macroHeader.h.gch


.DEFAULT_GOAL := SHAREDTARGET
# End of the main math makefile

