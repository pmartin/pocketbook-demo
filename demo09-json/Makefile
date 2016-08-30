PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -std=gnu++11 -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function


all: demo09.app

demo09.app: demo09.cpp
	$(CC) demo09.cpp $(CFLAGS) -o demo09.app -linkview -ljson-c -g -gdwarf-3

clean:
	rm -f demo09.app
