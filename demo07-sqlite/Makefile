PBSDK ?= /home/squale/developpement/PBSDK
CC = $(PBSDK)/bin/arm-obreey-linux-gnueabi-g++

CFLAGS = -Wall -Wextra -Wvla -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -Wno-unused-function


all: demo07.app

demo07.app: demo07.cpp
	$(CC) demo07.cpp $(CFLAGS) -o demo07.app -linkview -lsqlite3 -g -gdwarf-3

clean:
	rm -f demo07.app
