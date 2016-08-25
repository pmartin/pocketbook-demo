PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function


all: demo05.app

demo05.app: demo05.cpp
	$(CC) demo05.cpp $(CFLAGS) -o demo05.app -linkview -lcurl -g -gdwarf-3

clean:
	rm -f demo05.app
