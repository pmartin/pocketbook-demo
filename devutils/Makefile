PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function


all: devutils.app

devutils.app: devutils.cpp devutils.h wifi.cpp
	$(CC) devutils.cpp wifi.cpp $(CFLAGS) -o devutils.app -linkview -g -gdwarf-3

clean:
	rm -f devutils.app
