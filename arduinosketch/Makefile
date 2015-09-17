########################################################################
#
# Makefile for compiling Arduino sketches from command line
# System part (i.e. project independent)
#
# Copyright (C) 2012 Sudar <http://sudarmuthu.com>, based on
# M J Oldfield work: https://github.com/mjoldfield/Arduino-Makefile
#
# Copyright (C) 2010,2011,2012 Martin Oldfield <m@mjo.tc>, based on
# work that is copyright Nicholas Zambetti, David A. Mellis & Hernando
# Barragan.
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.
#
# Adapted from Arduino 0011 Makefile by M J Oldfield
#
# Original Arduino adaptation by mellis, eighthave, oli.keller
#
# Current version: 1.3.1
#
# Refer to HISTORY.md file for complete history of changes
#
########################################################################
#
# PATHS YOU NEED TO SET UP
#
# We need to worry about three different sorts of file:
#
# 1. The directory where the *.mk files are stored
#    => ARDMK_DIR
#
# 2. Things which are always in the Arduino distribution e.g.
#    boards.txt, libraries, &c.
#    => ARDUINO_DIR
#
# 3. Things which might be bundled with the Arduino distribution, but
#    might come from the system. Most of the toolchain is like this:
#    on Linux it's supplied by the system.
#    => AVR_TOOLS_DIR
#
# Having set these three variables, we can work out the rest assuming
# that things are canonically arranged beneath the directories defined
# above.
#
# On the Mac you might want to set:
#
#   ARDUINO_DIR   = /Applications/Arduino.app/Contents/Resources/Java
#   ARDMK_DIR     = /usr/local
#
# On Linux, you might prefer:
#
 ARDUINO_DIR   = /usr/share/arduino
 ARDMK_DIR     = /usr/share/arduino
 AVR_TOOLS_DIR = /usr
#
# You can either set these up in the Makefile, or put them in your
# environment e.g. in your .bashrc
#
# If you don't specify these, we can try to guess, but that might not work
# or work the way you want it to.
#
# If you'd rather not see the configuration output, define ARDUINO_QUIET.
#
########################################################################
#
# DEPENDENCIES
#
# The Perl programs need a couple of libraries:
#    Device::SerialPort
#
########################################################################
#
# STANDARD ARDUINO WORKFLOW
#
# Given a normal sketch directory, all you need to do is to create
# a small Makefile which defines a few things, and then includes this one.
#
# For example:
#
 ARDUINO_LIBS = SdFat
 BOARD_TAG    = uno
 MONITOR_PORT = /dev/ttyUSB*
#
 include /usr/share/arduino/Arduino.mk
#
# Hopefully these will be self-explanatory but in case they're not:
#
#    ARDUINO_LIBS - A list of any libraries used by the sketch (we
#                   assume these are in $(ARDUINO_DIR)/hardware/libraries
#                   or your sketchbook's libraries directory)
#
#    MONITOR_PORT - The port where the Arduino can be found (only needed
#                   when uploading)
#
#    BOARD_TAG    - The tag for the board e.g. uno or mega
#                   'make show_boards' shows a list
#
# If you have your additional libraries relative to your source, rather
# than in your "sketchbook", also set USER_LIB_PATH, like this example:
#
#        USER_LIB_PATH := $(realpath ../../libraries)
#
# If you've added the Arduino-Makefile repository to your git repo as a
# submodule (or other similar arrangement), you might have lines like this
# in your Makefile:
#
#        ARDMK_DIR := $(realpath ../../tools/Arduino-Makefile)
#        include $(ARDMK_DIR)/Arduino.mk
#
# In any case, once this file has been created the typical workflow is just
#
#   $ make upload
#
# All of the object files are created in the build-{BOARD_TAG} subdirectory
# All sources should be in the current directory and can include:
#  - at most one .pde or .ino file which will be treated as C++ after
#    the standard Arduino header and footer have been affixed.
#  - any number of .c, .cpp, .s and .h files
#
# Included libraries are built in the build-{BOARD_TAG}/libs subdirectory.
#
# Besides make upload, there are a couple of other targets that are available.
# Do make help to get the complete list of targets and their description
#
########################################################################
#
# SERIAL MONITOR
#
# The serial monitor just invokes the GNU screen program with suitable
# options. For more information see screen (1) and search for
# 'character special device'.
#
# The really useful thing to know is that ^A-k gets you out!
#
# The fairly useful thing to know is that you can bind another key to
# escape too, by creating $HOME{.screenrc} containing e.g.
#
#    bindkey ^C kill
#
# If you want to change the baudrate, just set MONITOR_BAUDRATE. If you
# don't set it, it tries to read from the sketch. If it couldn't read
# from the sketch, then it defaults to 9600 baud.
#
########################################################################
#
# ARDUINO WITH ISP
#
# You need to specify some details of your ISP programmer and might
# also need to specify the fuse values:
#
#     ISP_PROG	   = stk500v2
#     ISP_PORT     = /dev/ttyACM0
#
# You might also need to set the fuse bits, but typically they'll be
# read from boards.txt, based on the BOARD_TAG variable:
#
#     ISP_LOCK_FUSE_PRE  = 0x3f
#     ISP_LOCK_FUSE_POST = 0xcf
#     ISP_HIGH_FUSE      = 0xdf
#     ISP_LOW_FUSE       = 0xff
#     ISP_EXT_FUSE       = 0x01
#
# You can specify to also upload the EEPROM file:
#     ISP_EEPROM   = 1
#
# I think the fuses here are fine for uploading to the ATmega168
# without bootloader.
#
# To actually do this upload use the ispload target:
#
#    make ispload
#
#
########################################################################
#
# ALTERNATIVE CORES
#
# To use alternative cores for platforms such as ATtiny, you need to
# specify a few more variables, depending on the core in use.
#
# The HLT (attiny-master) core can be used just by specifying
# ALTERNATE_CORE, assuming your core is in your ~/sketchbook/hardware
# directory. For example:
#
# ISP_PORT          = /dev/ttyACM0
# BOARD_TAG         = attiny85
# ALTERNATE_CORE    = attiny-master
#
# To use the more complex arduino-tiny and TinyCore2 cores, you must
# also set ARDUINO_CORE_PATH and ARDUINO_VAR_PATH to the core
# directory, as these cores essentially replace the main Arduino core.
# For example:
#
# ISP_PORT          = /dev/ttyACM0
# BOARD_TAG         = attiny85at8
# ALTERNATE_CORE    = arduino-tiny
# ARDUINO_VAR_PATH  = ~/sketchbook/hardware/arduino-tiny/cores/tiny
# ARDUINO_CORE_PATH = ~/sketchbook/hardware/arduino-tiny/cores/tiny
#
# or....
#
# ISP_PORT          = /dev/ttyACM0
# BOARD_TAG         = attiny861at8
# ALTERNATE_CORE    = tiny2
# ARDUINO_VAR_PATH  = ~/sketchbook/hardware/tiny2/cores/tiny
# ARDUINO_CORE_PATH = ~/sketchbook/hardware/tiny2/cores/tiny
#
########################################################################
