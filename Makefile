BOARD_TAG    = nano328

NO_CORE = Yes

MCU = atmega328p
F_CPU = 16000000L

ISP_PROG   = stk500v1
#AVRDUDE_ISP_BAUDRATE = 19200
AVRDUDE_ISP_BAUDRATE = 57600
AVRDUDE_ARD_PROGRAMMER = arduino
AVRDUDE_ARD_BAUDRATE = 57600
HEX_MAXIMUM_SIZE = 30720

#MONITOR_PORT = /dev/ttyUSB0
MONITOR_CMD = screen

CXXFLAGS = -std=c++11

#include ../../Arduino.mk
include /usr/share/arduino/Arduino.mk

# TODO: does this apply?:
# !!! Important. You have to use make ispload to upload when using ISP programmer
