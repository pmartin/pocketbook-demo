PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function


all: demo08.app

demo08.app: demo08.cpp
	$(CC) demo08.cpp $(CFLAGS) -o demo08.app -linkview -lpthread -g -gdwarf-3

clean:
	rm -f demo08.app
