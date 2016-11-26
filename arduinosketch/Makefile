BOARD_TAG    = uno
ARDUINO_LIBS = SdFat SPI
MONITOR_PORT = /dev/ttyUSB*
ARDUINO_DIR = /home/ruud/ArduinoIDE/arduino-1.6.13

include ./Arduino-Makefile/Arduino.mk

SdFat:
	git clone https://github.com/greiman/SdFat
	cp -r SdFat/SdFat $(ARDUINO_DIR)/libraries/

Arduino-MakeFile:
	git clone https://github.com/sudar/Arduino-Makefile


#####
# Automating this is a hassle, and i wont bother
# This is a simple guide to get you though it
# step -1: comment the include thingy above h
# step 0: run make Arduino-MakeFile
# step 1: download the latest arduino stuff
# step 1.1: go to https://www.arduino.cc/en/Main/Software
# step 1.2: download the appropite package and place it somewhere
# step 1.3: copy the folder to which you unziped the package
# step 2: get the SdFat repository
# step 2.1: paste the justafore mentioned path to the ARDINO_DIR of this make file
# step 2.3: run make SdFat
# step 2.4: uncomment the include thingy
# step 3: run "make upload" to compile and upload
# step 4: enjoy and make babies
