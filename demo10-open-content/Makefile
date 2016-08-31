PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -std=gnu++11 -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function


all: demo10.app

demo10.app: demo10.cpp
	$(CC) demo10.cpp $(CFLAGS) -o demo10.app -linkview -g -gdwarf-3

clean:
	rm -f demo10.app
